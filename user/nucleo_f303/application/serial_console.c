#include "serial_console.h"
#include "cli/cli_types.h"
#include "cli/cli_server.h"


#if CONFIG_USING_RTOS==1
static int console_server_write(struct cli_server *server, const char *buf, int len)
{
    return kernel_console_write((const kl_char_t *)buf, len);
}

static int console_server_read(struct cli_server *server, char *buf, int len)
{
    return kernel_console_read((kl_char_t *)buf, len);
}

static const cli_interface_t console_interface={
    .write = console_server_write,
    .read = console_server_read,
};

#define CONSOLE_BUFFER_SIZE 128
static kl_u8_t console_input_buffer[CONSOLE_BUFFER_SIZE];
static kl_u8_t console_output_buffer[CONSOLE_BUFFER_SIZE];
static kl_u8_t console_parser_buffer[CONSOLE_BUFFER_SIZE];

static struct cli_server console_server = {
    .interface = &console_interface,
    .input_buffer.data = (char *)console_input_buffer,
    .input_buffer.len = CONSOLE_BUFFER_SIZE,
    .output_buffer.data = (char *)console_output_buffer,
    .output_buffer.len = CONSOLE_BUFFER_SIZE,
    .parser_buffer.data = (char *)console_parser_buffer,
    .parser_buffer.len = CONSOLE_BUFFER_SIZE,
    .promot = ">>",
};

void serial_console_init(void)
{
    cli_server_init(&console_server, "serial_console", configMINIMAL_STACK_SIZE);
}
#endif
