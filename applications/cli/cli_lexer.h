#ifndef __CLI_LEXER_H__
#define __CLI_LEXER_H__
#include "cli_types.h"


int lex_whitespace(lex_state_t *state, cli_token_t *token);

int lex_program_header(lex_state_t *state, cli_token_t *token);

int lex_parameter(lex_state_t *state, cli_token_t *token);

int lex_newline(lex_state_t *state, cli_token_t *token);

int lex_iseos(lex_state_t *state);

#endif