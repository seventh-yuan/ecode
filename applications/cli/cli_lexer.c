#include "cli_lexer.h"
#include <ctype.h>

static int isws(int ch)
{
    if((ch==' ')||(ch=='\t'))
        return 1;
    return 0;
}

static int iscomma(int ch)
{
    if(ch==',')
        return 1;
    else
        return 0;
}

static int isbdigit(int ch)
{
    if((ch=='0')||(ch=='1'))
        return 1;
    else
        return 0;
}

static int isqdigit(int ch)
{
    if ((ch == '0') || (ch == '1') || (ch == '2') || (ch == '3') || (ch == '4') || (ch == '5') || (ch == '6') || (ch == '7')) {
        return 1;
    }
    return 0;
}

static int ischr(lex_state_t *state, int ch)
{
    return (state->pos[0]==ch);
}

static int iseos(lex_state_t *state)
{
    if((state->buffer+state->len)<=(state->pos))
        return 1;
    else
        return 0;
}

static int skipws(lex_state_t *state)
{
    int ret = 0;
    while(!iseos(state)&&isws(state->pos[0]))
    {
        state->pos++;
        ret++;
    }
    return ret;
}

static int skipchr(lex_state_t *state, char ch)
{
    if(!iseos(state)&&ischr(state, ch))
    {
        state->pos++;
        return 0;
    }
    return -1;
}

int lex_whitespace(lex_state_t *state, cli_token_t *token)
{
    token->ptr = state->pos;
    skipws(state);
    token->len = state->pos-token->ptr;
    if(token->len>0)
    {
        token->type = CLI_TOKEN_WS;
    }
    else
    {
        token->type = CLI_TOKEN_INVALID;
    }
    return token->len;
}

int lex_program_header(lex_state_t *state, cli_token_t *token)
{
    token->ptr = state->pos;
    if(!iseos(state)&&isalpha((uint8_t)(state->pos[0])))
    {
        state->pos++;
        while(!iseos(state)&&(isalnum((uint8_t)(state->pos[0]))||ischr(state, '_')))
        {
            state->pos++;
        }
    }
    
    token->len = state->pos-token->ptr;
    if(token->len>0)
    {
        token->type = CLI_TOKEN_PROGRAM_HEADER;
    }
    else
    {
        token->type = CLI_TOKEN_INVALID;
    }
    return token->len;
}

int lex_parameter(lex_state_t *state, cli_token_t *token)
{
    token->ptr = state->pos;
    
    while(!iseos(state))
    {
        if((isws(state->pos[0]))||(!isprint((uint8_t)(state->pos[0]))))
            break;
        state->pos++;
    }
    
    token->len = state->pos-token->ptr;
    
    return token->len;
}

int lex_newline(lex_state_t *state, cli_token_t *token)
{
    token->ptr = state->pos;
    
    skipchr(state, '\r');
    skipchr(state, '\n');
    
    token->len = state->pos-token->ptr;
    
    if((token->len>0))
    {
        token->type = CLI_TOKEN_NL;
    }
    else
    {
        token->type = CLI_TOKEN_INVALID;
        state->pos = token->ptr;
        token->len = 0;
    }
    
    return token->len;
}

int lex_iseos(lex_state_t *state)
{
    return iseos(state);
}
