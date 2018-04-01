#ifndef __CLI_TYPES_H__
#define __CLI_TYPES_H__
#include "../../global/include.h"

typedef enum{
    CLI_TYPE_PATTERN,
    CLI_TYPE_INTEGER,
    CLI_TYPE_HEX,
    CLI_TYPE_FLOAT,
    CLI_TYPE_DOUBLE,
}cli_type_t;

struct _cli_buffer_t{
    kl_u16_t len;
    kl_u16_t position;
    char *data;
};
typedef struct _cli_buffer_t cli_buffer_t;

struct _lex_state_t{
    char *buffer;
    char *pos;
    int len;
};
typedef struct _lex_state_t lex_state_t;


enum _token_type_t{
    CLI_TOKEN_UNKNOW,
    CLI_TOKEN_INVALID,
    CLI_TOKEN_PROGRAM_HEADER,
    CLI_TOKEN_NL,
    CLI_TOKEN_WS,
};
typedef enum _token_type_t token_type_t;

struct _cli_token_t{
    token_type_t type;
    char *ptr;
    int len;
};
typedef struct _cli_token_t cli_token_t;

typedef cli_token_t cli_parameter_t;

enum _message_termination_t{
    MESSAGE_TERMINATION_NONE,
    MESSAGE_TERMINATION_NL,
};
typedef enum _message_termination_t message_termination_t;

struct _parser_state_t{
    cli_token_t programheader;
    cli_parameter_t parameter;
    int numparameter;
    lex_state_t lex_state;
    message_termination_t termination;
};
typedef struct _parser_state_t parser_state_t;

typedef struct _cli_interface cli_interface_t;



struct cli_server{
    const cli_interface_t *interface;
    const char *promot;
    parser_state_t parser_state;
    cli_buffer_t input_buffer;
    cli_buffer_t output_buffer;
    cli_buffer_t parser_buffer;
    void *private_data;
};

struct _cli_interface{
    int (*write)(struct cli_server *server, const char *buf, int len);
    int (*read)(struct cli_server *server, char *buf, int len);
};

typedef int (*cli_handle_t)(struct cli_server *server);

struct _cli_command_t{
    const char *pattern;
    const cli_handle_t handle;
    const char *const usage;
};
typedef struct _cli_command_t cli_command_t;

struct _command_node_t{
    void *next;
    const cli_command_t *cmdlist;
};
typedef struct _command_node_t command_node_t;



#endif