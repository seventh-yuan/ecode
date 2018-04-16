/**
  *******************************************************
  *@file        application.c
  *@author      Arno
  *@version     V1.0.0
  *@date        2018/4/1
  *@brief       application  module.
  *             This is the kernel part of ecode.
  *******************************************************
  */
/* Includes----------------------------------------------*/
#include "../../global/include.h"
#include "application/serial_console.h"
#include "application/handle.h"
#include "application/led_server.h"

/*Private variables--------------------------------------*/
/*Public variables---------------------------------------*/
/*Private functions--------------------------------------*/
/*Public functions---------------------------------------*/

/**
  * @brief  application init.
  * @retval None
  */

void application_init(void)
{
    console_register_handles();
    serial_console_init();
    led_server_init();
    print_debug("application init pass.");
    kprintln("system startup time: %0.3f s.", kernel_get_second());
}