#include "cli_server.h"
#include "cli_parser.h"
#include "cli_string.h"
#include <string.h>
#include <ctype.h>

#if CONFIG_USING_RTOS==1
#ifndef CONFIG_CLI_USING_COLOR
#define CONFIG_CLI_USING_COLOR  1
#endif

static command_node_t *command_node=NULL;

const char * errno_message[]={
    "No error",
    "Error",
    "Timed out",
    "The resource is full",
    "The resource is empty",
    "No memory",
    "No system",
    "Busy",
    "IO error",
    "Parameter error",
};


int cli_server_write(struct cli_server *server, const char *buf, int len)
{
    if(server->interface==NULL||server->interface->write==NULL)
        return -1;
    return server->interface->write(server, buf, len);
}

int cli_server_read(struct cli_server *server, char *buf, int len)
{
    if(server->interface==NULL||server->interface->read==NULL)
        return -1;
    return server->interface->read(server, buf, len);
}

static int cli_server_getc(struct cli_server *server)
{
    int ch = 0;
    if(cli_server_read(server, (char *)&ch, 1)<=0)
        return -1;
    return ch;
}

static int cli_server_putc(struct cli_server *server, int ch)
{
    if(cli_server_write(server, (const char *)&ch, 1)<=0)
        return -1;
    return 0;
}

int cli_server_readline(struct cli_server *server, char *buf, int bufsz, kernel_sec_t timeout)
{
    int ch = 0;
    int recvpos = 0;
    kernel_sec_t last_time = kernel_get_millis();
    
    while(1)
    {
        if(timeout!=0&&kernel_get_millis()-last_time>=timeout)
            return -1;
        if((ch=cli_server_getc(server))<=0)
        {
            sleep_ms(1);
            continue;
        }

        if(recvpos>=bufsz-1)
            return -1;
        if(ch=='\b')
        {
            if(recvpos!=0)
            {
                cli_server_putc(server, '\b');
                cli_server_putc(server, ' ');
                cli_server_putc(server, '\b');
                recvpos--;
                buf[recvpos]='\0';
            }
        }
        else
        {
            buf[recvpos++]=ch;
            cli_server_putc(server, ch);
        }
        
        
        if(ch=='\n')
        {
            buf[recvpos]='\0';
            break;
        }
        else if(!isprint(ch)&&ch!='\b')
        {
            recvpos--;
            buf[recvpos]='\0';
        }
    }
    
    return recvpos;
}

int cli_server_perror(struct cli_server *server, int errno, const char *errmsg)
{
    int ret = 0;
    cli_buffer_t *output_buffer = &server->output_buffer;
    int position = output_buffer->position;
#if CONFIG_CLI_USE_COLOR==1
    position += snprintf(output_buffer->data+position, output_buffer->len-position, "\033[2m\033[31m");
#endif
    position += snprintf(output_buffer->data+position, output_buffer->len-position, "%s", errno_message[errno]);
#if CONFIG_CLI_USE_COLOR==1
    position += snprintf(output_buffer->data+position, output_buffer->len-position, "\033[0m");
#endif
    if(errmsg!=NULL)
        position += snprintf(output_buffer->data+position, output_buffer->len-position, ":%s", errmsg);
    position += snprintf(output_buffer->data+position, output_buffer->len-position, "\n");
    
    ret = position - output_buffer->position;
    output_buffer->position = position;
    return ret;
}

int cli_server_flush_output(struct cli_server *server)
{
    int ret = cli_server_write(server, server->output_buffer.data, server->output_buffer.position);
    server->output_buffer.position = 0;
    return ret;
}

static int cli_process_command(struct cli_server *server)
{
    int ret = 0;
    cli_token_t *programheader = &server->parser_state.programheader;
    
    const cli_command_t *cmd = parser_find_command(command_node, programheader->ptr, programheader->len);
    
    if(cmd!=NULL&&cmd->handle!=NULL)
    {
        ret = cmd->handle(server);
        if(ret!=ENO_OK)
        {
            cli_server_perror(server, ret,NULL);
        }
    }
    else
    {
        cli_server_perror(server, ENO_ERROR, "no command found!");
    }
    cli_server_flush_output(server);
    return 0;
}

static int cli_server_parser(struct cli_server *server, char *data, int len)
{
    parser_state_t *parser_state = &server->parser_state;
    cli_buffer_t *parser_buffer = &server->parser_buffer;
    if(len>0)
    {
        int buffer_free = parser_buffer->len-parser_buffer->position;
        if(len>(buffer_free-1))
        {
            parser_buffer->position = 0;
            parser_buffer->data[parser_buffer->position]=0;
            return -1;
        }
        
        memcpy(&parser_buffer->data[parser_buffer->position], data, len);
        parser_buffer->position += len;
        parser_buffer->data[parser_buffer->position]=0;
        
        data = parser_buffer->data;
        len = parser_buffer->position;
        
        
        for(;;)
        {
            int ret = parser_detect_program_message_unit(parser_state, data, len);
            if(ret<0||(parser_state->termination!=MESSAGE_TERMINATION_NL))
            {
                memmove(parser_buffer->data, data, len);
                parser_buffer->position = 0;
                break;
            }
            cli_process_command(server);
            data +=ret;
            len -= ret;
        }
    }
    
    return 0;
}


static void cli_server_thread(void const *args)
{
    struct cli_server *server = (struct cli_server *)args;
    
    for(;;)
    {
        cli_server_write(server, server->promot, strlen(server->promot));
        int recv = cli_server_readline(server, server->input_buffer.data, server->input_buffer.len, 0);
        cli_server_parser(server, server->input_buffer.data, recv);
    }
}

void cli_server_register_commands_node(command_node_t *node)
{
    if(command_node==NULL)
    {
        node->next = NULL;
        command_node = node;
    }
    else
    {
        node->next = command_node;
        command_node = node;
    }
}

void cli_server_register_commands(command_node_t *node, const cli_command_t *cmdlist)
{
    node->cmdlist = cmdlist;
    cli_server_register_commands_node(node);
}

void cli_server_command_usage_show(struct cli_server *server)
{
    const cli_command_t *cmd;
    command_node_t *cmdnode = command_node;
    
    while(cmdnode!=NULL)
    {
        for(cmd = cmdnode->cmdlist;cmd->pattern!=NULL;cmd++)
        {
            if(cmd->usage!=NULL)
            {
                cli_server_write(server, cmd->usage, strlen(cmd->usage));
            }
        }
        cmdnode = cmdnode->next;
    }
}

int cli_server_count_parameter(struct cli_server *server)
{
    return parser_count_parameter(&server->parser_state);
}

int cli_get_parameter(struct cli_server *server, cli_token_t *token)
{
    return parser_parameter(&server->parser_state, token);
}

int cli_server_parser_parameter(struct cli_server *server, cli_type_t type, const char *pattern, void *data)
{
    cli_token_t token;
    
    if(parser_parameter(&server->parser_state, &token)<=0)
        return -1;
    
    if(pattern!=NULL&&type==CLI_TYPE_PATTERN)
    {
        if(sscanf(token.ptr, pattern, data)==0)
            return -1;
        return 0;
    }
    switch(type)
    {
    case CLI_TYPE_INTEGER:
        if(str2int(token.ptr, data, 10)<=0)
            return -1;
        break;
    case CLI_TYPE_HEX:
        if(str2int(token.ptr, data, 16)<=0)
            return -1;
    case CLI_TYPE_FLOAT:
        if(str2float(token.ptr, data)<=0)
            return -1;
        break;
    case CLI_TYPE_DOUBLE:
        if(str2double(token.ptr, data)<=0)
            return -1;
        break;
    default:
        return -1;
    }
    return 0;
}

const char *cli_get_command_usage(struct cli_server *server, const char *header, int len)
{
    return parser_find_command_usage(command_node, header, len);
}

int cli_server_init(struct cli_server *server, const char *name, int stacksz)
{    
    server->input_buffer.position = 0;
    osThreadDef_t cli_thread={
        .name = (char *)name,
        .pthread = cli_server_thread,
        .tpriority = osPriorityNormal,
        .stacksize = stacksz,
    };
    osThreadCreate(&cli_thread, server);

    return 0;
}
#endif
