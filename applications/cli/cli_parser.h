#ifndef __CLI_PARSER_H__
#define __CLI_PARSER_H__
#include "cli_types.h"

int parser_detect_program_message_unit(parser_state_t *state, char *data, int len);

int parser_number_parameter(parser_state_t *state);

int parser_count_parameter(parser_state_t *state);

int parser_parameter(parser_state_t *state, cli_token_t *token);

const cli_command_t * parser_find_command(command_node_t *cmdnode, const char *header, int len);

const char *parser_find_command_usage(command_node_t *cmdnode, const char *header, int len);
#endif