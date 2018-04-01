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
    
    print_debug("application init pass.");
    kprintln("system startup time: %0.3f s.", kernel_get_second());
}