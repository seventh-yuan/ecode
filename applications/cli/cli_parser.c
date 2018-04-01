#include "cli_parser.h"
#include "cli_lexer.h"
#include <string.h>


static void invalidate_token(cli_token_t *token, char *ptr)
{
    token->ptr = ptr;
    token->len = 0;
    token->type = CLI_TOKEN_INVALID;
}

int parser_detect_program_message_unit(parser_state_t *state, char *data, int len)
{
    lex_state_t lex_state;
    cli_token_t tmp;
    
    lex_state.buffer = lex_state.pos = data;
    lex_state.len = len;
    
    lex_whitespace(&lex_state, &tmp);
    
    lex_program_header(&lex_state, &tmp);
    
    if(tmp.type==CLI_TOKEN_INVALID)
        return -1;
    
    state->programheader.ptr = tmp.ptr;
    state->programheader.len = tmp.len;
    state->programheader.type = tmp.type;
    state->numparameter = 0;
    lex_whitespace(&lex_state, &tmp);
    state->parameter.ptr = lex_state.pos;
    while(!lex_iseos(&lex_state))
    {
        lex_whitespace(&lex_state, &tmp);
        lex_parameter(&lex_state, &tmp);
        if(tmp.len>0)
        {
            state->parameter.len = lex_state.pos-state->parameter.ptr;
            state->numparameter++;
        }
        lex_newline(&lex_state, &tmp);
        if(tmp.type==CLI_TOKEN_NL)
            break;
    }
    if(tmp.type!=CLI_TOKEN_NL)
    {
        invalidate_token(&state->programheader, NULL);
        state->numparameter = 0;
        state->termination = MESSAGE_TERMINATION_NONE;
    }
    else
    {
        state->lex_state.buffer = state->lex_state.pos = state->parameter.ptr;
        state->lex_state.len = state->parameter.len;
        
        state->termination = MESSAGE_TERMINATION_NL;
    }
    
    return lex_state.pos-lex_state.buffer;
}

int parser_count_parameter(parser_state_t *state)
{
    return state->numparameter;
}

int parser_parameter(parser_state_t *state, cli_token_t *token)
{
    cli_token_t tmp;
    lex_state_t *lex_state = &state->lex_state;
    
    lex_whitespace(lex_state, &tmp);
    token->ptr = lex_state->pos;
    lex_parameter(lex_state, &tmp);
    if(tmp.len<=0)
        return -1;
    token->len = lex_state->pos-token->ptr;
    return token->len;
}

int parser_match_command(const char *pattern, const char *header, int len)
{
    if(strlen(pattern)!=len)
        return 0;
    if(0==strncmp(pattern, header, len))
        return 1;
    return 0;
}

const cli_command_t * parser_find_command(command_node_t *cmdnode, const char *header, int len)
{
    const cli_command_t *cmd;
    
    while(cmdnode!=NULL)
    {
        for(cmd=cmdnode->cmdlist;cmd->pattern!=NULL;cmd++)
        {
            if(parser_match_command(cmd->pattern, header, len))
            {
                return cmd;
            }
        }
        cmdnode = cmdnode->next;
    }
    
    
    return NULL;
}

const char *parser_find_command_usage(command_node_t *cmdnode, const char *header, int len)
{
    
    const cli_command_t *cmd = parser_find_command(cmdnode, header, len);
    if(cmd==NULL)
        return NULL;
    return cmd->usage;
}





