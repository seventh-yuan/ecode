/**
  *******************************************************
  *@file        kernel.c
  *@author      Arno
  *@version     V1.0.0
  *@date        2018/4/1
  *@brief       xtime  module.
  *             This is the kernel part of ecode.
  *******************************************************
  */
/* Includes----------------------------------------------*/
#include "kernel.h"

/*Private variables--------------------------------------*/
/*Public variables---------------------------------------*/
/*Private functions--------------------------------------*/
/*Public functions---------------------------------------*/

/**
  * @brief This function is used to init kernel.
  * @retval None
  */
void kernel_init(void)
{
    
}

/**
  * @brief This function is used to start kernel.
  * @retval None
  */
void kernel_start(void)
{
#if CONFIG_USING_RTOS==1
    osKernelStart();
#else
    for(;;)
    {
        
    }
#endif
}

/**
  * @brief This function is used to update kernel period event.
  * @retval None
  */
void kernel_periodic(void)
{
#if CONFIG_USING_RTOS==1
    osSystickHandler();
#endif
    kernel_update_wall_jiffies();

}