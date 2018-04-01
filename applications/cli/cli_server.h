#ifndef __CLI_SERVER_H__
#define __CLI_SERVER_H__
#include "cli_types.h"


#define CLI_RETURN(server, errno, msg)  do{cli_server_perror(server, errno, msg);return errno;}while(0)

int cli_server_write(struct cli_server *server, const char *buf, int len);
int cli_server_read(struct cli_server *server, char *buf, int len);
void cli_server_register_commands_node(command_node_t *node);
void cli_server_register_commands(command_node_t *node, const cli_command_t *cmdlist);
void cli_server_command_usage_show(struct cli_server *server);
int cli_server_count_parameter(struct cli_server *server);
int cli_get_parameter(struct cli_server *server, cli_token_t *token);
int cli_server_parser_parameter(struct cli_server *server, cli_type_t type, const char *pattern, void *data);
const char *cli_get_command_usage(struct cli_server *server, const char *header, int len);
int cli_server_perror(struct cli_server *server, int errno, const char *errmsg);
int cli_server_init(struct cli_server *server, const char *name, int stacksz);
#endif