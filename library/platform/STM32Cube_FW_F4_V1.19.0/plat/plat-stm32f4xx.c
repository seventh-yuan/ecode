/**
  *******************************************************
  *@file        plat-stm32l4xx.c
  *@author      Arno
  *@version     V1.0.0
  *@date        2018/4/18
  *@brief       platform cpu module.
  *             This implement platform cpu function.
  *******************************************************
  */
  
/* Includes----------------------------------------------*/
#include "../../../../global/include.h"
#include "stm32f4xx_hal.h"
/*Private variables--------------------------------------*/ 
/*Public variables---------------------------------------*/
/*Private functions--------------------------------------*/
/*Public functions---------------------------------------*/


/**
  * @brief This function implement cpu us delay.
  * @param us: us to delay.
  * @retval None.
  */
void plat_udelay(kernel_time_t us)
{
    kernel_time_t last_tick = SysTick->VAL;
    kernel_time_t total_ticks = SystemCoreClock/1000000*us;
    kernel_time_t curr_tick = 0;
    kernel_time_t curr_counter = 0;
    
    for(;;)
    {
        curr_tick = SysTick->VAL;
        if(curr_tick<last_tick)
        {
            curr_counter += last_tick-curr_counter;
        }
        else if(curr_tick>last_tick)
        {
            curr_counter += last_tick+SysTick->LOAD-curr_tick;
        }
        else
            continue;
        last_tick = curr_tick;
        if(curr_counter>=total_ticks)
            return;
    }
}
  