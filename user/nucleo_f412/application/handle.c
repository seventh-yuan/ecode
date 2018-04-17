#include "handle.h"
#include "version.h"
#include "cli/cli_types.h"
#include "cli/cli_server.h"
#include <string.h>

extern void board_reset(void);

#define GCOMMAND_BUFFER_SIZE    128
static char _gcommand_buffer[GCOMMAND_BUFFER_SIZE];

static int help(struct cli_server *server);
static int reset(struct cli_server *server);
static int version(struct cli_server *server);
static int time(struct cli_server *server);

#if CONFIG_USING_RTOS==1
static const cli_command_t cmds[]={
    {.pattern="help", .handle = help, .usage="help:\n\t-view commands list\n"},
    {.pattern="reset", .handle = reset, .usage="reset:\n\t-reset system\n"},
    {.pattern="version", .handle = version, .usage="version:\n\t-show version information\n"},
    {.pattern="time", .handle = time, .usage ="time:\n\t-show system time.\n"},
    {NULL,NULL,NULL},
};
static command_node_t cmdhead;

void console_register_handles(void)
{
    cli_server_register_commands(&cmdhead, cmds);
}

static int help(struct cli_server *server)
{
    int ret = 0;
    int num = cli_server_count_parameter(server);
    
    if(num==0)
    {
        cli_server_command_usage_show(server);
    }
    else
    {
        cli_token_t token;
        for(int i = 0;i<num;i++)
        {
            if(cli_get_parameter(server,&token)<=0)
            {
               ret = ENO_PARAM;
               goto ret_error;
            }
            const char *usage = cli_get_command_usage(server, token.ptr, token.len);
            if(usage!=NULL)
                cli_server_write(server, usage, strlen(usage));
            else
            {
                ret = ENO_ERROR;
                goto ret_error;
            }
        }
    }
    
    return ENO_OK;
ret_error:
    return ret;
}

static int reset(struct cli_server *server)
{

    return ENO_OK;
}

static int version(struct cli_server *server)
{
    int ret = version_show(_gcommand_buffer, sizeof(_gcommand_buffer));
    cli_server_write(server, _gcommand_buffer, ret);
    return ENO_OK;
}

static int time(struct cli_server *server)
{
    kernel_sec_t second = kernel_get_second();
    int ret = sprintf(_gcommand_buffer, "%0.3fs"KERNEL_NL,second);
    cli_server_write(server, _gcommand_buffer, ret);
    return ENO_OK;
}

#else
void console_register_handles(void)
{

}
#endif


