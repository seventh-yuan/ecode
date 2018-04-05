/**
  *******************************************************
  *@file        print.c
  *@author      Arno
  *@version     V1.0.0
  *@date        2018/4/1
  *@brief       print  module.
  *             This is the kernel part of ecode.
  *******************************************************
  */

/* Includes----------------------------------------------*/
#include "print.h"

#if CONFIG_USING_PRINT==1
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

/*Macro variables----------------------------------------*/
#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_fputchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

#ifndef CONFIG_PRINT_BUF_SIZE
#define CONFIG_PRINT_BUF_SIZE   100
#endif
/*Private variables--------------------------------------*/
#if CONFIG_USING_PRINT_DEVICE
static kernel_device_t _kernel_print_device = KL_NULL;
#endif

/*Public variables---------------------------------------*/
/*Private functions--------------------------------------*/
/*Public functions---------------------------------------*/
extern int console_hw_write(const kl_u8_t *buf, kl_size_t len);
extern int console_hw_read(kl_u8_t *buf, kl_size_t len);

#if CONFIG_USING_PRINT_DEVICE==1
/**
  * @brief This function is used to set console device.
  * @param name: device name.
  * @retval 0:ok, -1:failed.
  */
int kernel_console_set_device(const char *name)
{
    _kernel_print_device = device_open(name, 0);
    
    return 0;
}
#endif
  
/**
  * @brief This function is used to write data to console.
  * @param buf: data to be writed.
  * @param len: length of data to writed.
  * @retval size has been writed.
  */
int kernel_console_write(const kl_char_t *buf, kl_size_t len)
{
    int ret = 0;
#if CONFIG_USING_PRINT_DEVICE==1
    if(_kernel_print_device==KL_NULL)
    {
        ret = console_hw_write((const kl_u8_t *)buf, len);
    }
    else
    {
        ret = device_write(_kernel_print_device, (const kl_u8_t *)buf, len);
    }
#else
    ret = console_hw_write((const kl_u8_t *)buf, len);
#endif
    
    return ret;
}
/**
  * @brief This function is used to read data from console.
  * @param buf: data to be read.
  * @param len: length of data to read.
  * @retval size has been read.
  */
int kernel_console_read(const kl_char_t *buf, kl_size_t len)
{
    int ret = 0;
#if CONFIG_USING_PRINT_DEVICE==1
    if(_kernel_print_device==KL_NULL)
    {
        ret = console_hw_read((kl_u8_t *)buf, len);
    }
    else
    {
        ret = device_read(_kernel_print_device, (kl_u8_t*)buf, len);
    }
#else
    ret = console_hw_read((kl_u8_t *)buf, len);
#endif
    
    return ret;
}
/**
  * @brief This function is used to put char to console.
  * @param ch: ch to be writed to console.
  * @retval 0: ok, 1:failed.
  */
int kernel_console_putc(int ch)
{
    return kernel_console_write((const kl_char_t *)&ch, 1);
}
/**
  * @brief This function is used to get char from console.
  * @retval char read from console, -1:failed..
  */
int kernel_console_getc(void)
{
    int ch = 0;
    if(kernel_console_read((kl_char_t *)&ch, 1)<=0)
        return -1;
    return 0;
}
/**
  * @brief This function is used to print format string.
  * @param fmt: format string
  * @retval number of char has been print, -1:failed.
  */
int kprintf(const char *fmt, ...)
{
    static kl_char_t _kernel_print_buf[CONFIG_PRINT_BUF_SIZE];
    
    int ret = 0;
    va_list args;
    va_start(args, fmt);
    ret = vsnprintf((kl_char_t *)_kernel_print_buf, CONFIG_PRINT_BUF_SIZE-1, fmt, args);
    va_end(args);
    
    ret = kernel_console_write((kl_char_t *)_kernel_print_buf, ret);
    
    return ret;
}

/**
  * @brief This function is used to print format string with new line.
  * @param fmt: format string
  * @retval number of char has been print, -1:failed.
  */
int kprintln(const char *fmt, ...)
{
    int ret = 0;
    va_list args;
    va_start(args, fmt);
    
    ret = kprintf(fmt, args);
    
    va_end(args);
    
    ret+=kprintf(KERNEL_NL);
    return ret;
}


#if CONFIG_LOG_LEVEL!=LOG_LEVEL_CLOSE
/**
  * @brief This function is used to print format string with log level.
  * @param n: log level.
  * @param fmt: format string
  * @retval number of char has been print, -1:failed.
  */
int kprint_log(int n, const char *fmt, ...)
{
    int ret = 0;
    va_list args;
    
    static kl_char_t _kernel_print_buf[CONFIG_PRINT_BUF_SIZE];
    
    va_start(args, fmt);
    ret = vsprintf((kl_char_t *)_kernel_print_buf, fmt, args);
    va_end(args);
    
    if(ret<=0)
        return -1;
    ret = 0;
#if CONFIG_USING_TERM_COLOR==1
    switch(n)
    {
    case LOG_LEVEL_ERROR:
        ret += kprintf("\033[2m\033[31m[ERROR]\033[0m");
        break;
    case LOG_LEVEL_WARN:
        ret += kprintf("\033[2m\033[33m[WARN]\033[0m");
        break;
    case LOG_LEVEL_INFO:
        ret += kprintf("[INFO]");
        break;
    case LOG_LEVEL_DEBUG:
         ret+=kprintf("\033[2m\033[32m[DEBUG]\033[0m");
        break;
    }
#else
    switch(n)
    {
    case LOG_LEVEL_ERROR:
        ret += kprintf("[ERROR]");
        break;
    case LOG_LEVEL_WARN:
        ret += kprintf("[WARN]");
        break;
    case LOG_LEVEL_INFO:
        ret += kprintf("[INFO]");
        break;
    case LOG_LEVEL_DEBUG:
         ret+=kprintf("[DEBUG]");
        break;
    }
#endif
    
    ret += kprintf((const kl_char_t *)_kernel_print_buf);
    ret += kprintf(KERNEL_NL);
    
    return ret;
}
#endif

PUTCHAR_PROTOTYPE
{
    kernel_console_putc((kl_char_t)ch);
    return ch;
}

#endif
