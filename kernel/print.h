#ifndef __PRINT_H__
#define __PRINT_H__
#include "../global/include.h"

#ifdef __cplusplus
extern "C"{
#endif

#ifndef KERNEL_NL
#define KERNEL_NL   "\n"
#endif

#define LOG_LEVEL_CLOSE     0
#define LOG_LEVEL_ERROR     1
#define LOG_LEVEL_WARN      2
#define LOG_LEVEL_INFO      3
#define LOG_LEVEL_DEBUG     4

#if CONFIG_USING_PRINT==1

#if CONFIG_USING_DEVICE==1

#ifndef CONFIG_USING_PRINT_DEVICE
#define CONFIG_USING_PRINT_DEVICE   0
#endif

#endif

#ifndef CONFIG_LOG_LEVEL
#define CONFIG_LOG_LEVEL    LOG_LEVEL_DEBUG
#endif

#else
#define kprintf(fmt, ...)
#endif

#if CONFIG_LOG_LEVEL==LOG_LEVEL_DEBUG
#define print_error(fmt,...)    kprint_log(LOG_LEVEL_ERROR,fmt, ##__VA_ARGS__)
#define print_warn(fmt,...)     kprint_log(LOG_LEVEL_WARN, ##__VA_ARGS__)
#define print_info(fmt,...)     kprint_log(LOG_LEVEL_INFO,fmt, ##__VA_ARGS__)
#define print_debug(fmt,...)    kprint_log(LOG_LEVEL_DEBUG,fmt, ##__VA_ARGS__)
#elif CONFIG_LOG_LEVEL==LOG_LEVEL_INFO
#define print_error(fmt,...)    kprint_log(LOG_LEVEL_ERROR,fmt, ##__VA_ARGS__)
#define print_warn(fmt,...)     kprint_log(LOG_LEVEL_WARN, ##__VA_ARGS__)
#define print_info(fmt,...)     kprint_log(LOG_LEVEL_MESSAGE,fmt, ##__VA_ARGS__)
#define print_debug(fmt,...)
#elif CONFIG_LOG_LEVEL==LOG_LEVEL_WARN
#define print_error(fmt,...)    kprint_log(LOG_LEVEL_ERROR,fmt, ##__VA_ARGS__)
#define print_warn(fmt,...)     kprint_log(LOG_LEVEL_WARN, fmt, ##__VA_ARGS__)
#define print_info(fmt,...)
#define print_debug(fmt,...)
#elif CONFIG_LOG_LEVEL==LOG_LEVEL_ERROR
#define print_error(fmt,...)    kprint_log(LOG_LEVEL_ERROR,fmt, ##__VA_ARGS__)
#define print_warn(fmt,...)
#define print_info(fmt,...)
#define print_debug(fmt,...)
#elif CONFIG_LOG_LEVEL==LOG_LEVEL_CLOSE
#define print_error(fmt,...)
#define print_warn(fmt,...)
#define print_info(fmt,...)
#define print_debug(fmt,...)
#endif


int kernel_console_write(const kl_char_t *buf, kl_size_t len);

/**
  * @brief This function is used to read data from console.
  * @brief buf: data to be read.
  * @brief len: length of data to read.
  * @retval size has been read.
  */
int kernel_console_read(const kl_char_t *buf, kl_size_t len);

/**
  * @brief This function is used to put char to console.
  * @brief ch: ch to be writed to console.
  * @retval 0: ok, 1:failed.
  */
int kernel_console_putc(int ch);

/**
  * @brief This function is used to get char from console.
  * @retval char read from console, -1:failed..
  */
int kernel_console_getc(void);

/**
  * @brief This function is used to print format string.
  * @brief fmt: format string
  * @retval number of char has been print, -1:failed.
  */
int kprintf(const char *fmt, ...);

    /**
  * @brief This function is used to print format string with new line.
  * @param fmt: format string
  * @retval number of char has been print, -1:failed.
  */
int kprintln(const char *fmt, ...);

/**
  * @brief This function is used to print format string with log level.
  * @brief n: log level.
  * @brief fmt: format string
  * @retval number of char has been print, -1:failed.
  */
int kprint_log(int n, const char *fmt, ...);


#ifdef __cplusplus
}
#endif
#endif