/**
  *******************************************************
  *@file        ktime.c
  *@author      Arno
  *@version     V1.0.0
  *@date        2018/4/1
  *@brief       kernel time module.
  *             This is the kernel part of ecode.
  *******************************************************
  */
  
/* Includes----------------------------------------------*/
#include "ktime.h"

/*Private variables--------------------------------------*/
static __kl_volatile unsigned long _kernel_wall_jiffies = 0;
/*Public variables---------------------------------------*/
/*Private functions--------------------------------------*/
/*Public functions---------------------------------------*/

/**
  * @brief This function is used to update wall jiffies.
  * @retval None
  */
void kernel_update_wall_jiffies(void)
{
    _kernel_wall_jiffies++;
}

/**
  * @brief This function is used to get kernel timeval.
  * @param tv_val: destination to save timeval.
  * @retval 0: ok, -1: failed.
  */
int kernel_gettime(kernel_timeval_t *tv_val)
{
    unsigned long temp = _kernel_wall_jiffies;
    
    if(tv_val==NULL)
        return -1;
    tv_val->tv_sec = _kernel_wall_jiffies/KERNEL_HZ;
    
    tv_val->tv_usec = (_kernel_wall_jiffies-tv_val->tv_sec*KERNEL_HZ)*1000000/KERNEL_HZ;
    
    return 0;
}

/**
  * @brief This function is used to get kernel kernel millisecond time.
  * @retval milliseconds of kernel.
  */
kernel_msec_t kernel_get_millis(void)
{
    return _kernel_wall_jiffies*1000/KERNEL_HZ;
}

/**
  * @brief This function is used to get kernel kernel second time.
  * @retval seconds of kernel.
  */
kernel_sec_t kernel_get_second(void)
{
    return _kernel_wall_jiffies*1.0/KERNEL_HZ;
}

/**
  * @brief This function is used to delay milliseconds.
  * @param milliseconds to delay.
  * @retval None.
  */
void kernel_delay_ms(kernel_msec_t millis)
{
    unsigned long timeout = _kernel_wall_jiffies + millis/1000/KERNEL_HZ;
    
    while(kernel_time_before(_kernel_wall_jiffies, timeout));
        
}

/**
  * @brief This function is used to sleep milliseconds.
  * @param millis milliseconds to sleep.
  * @retval None.
  */
void kernel_sleep_ms(kernel_msec_t millis)
{
#if CONFIG_USING_RTOS==1
    osDelay(millis);
#else
    kernel_delay_ms(millis);
#endif
}

/**
  * @brief This function is used to sleep seconds.
  * @param seconds to sleep.
  * @retval None.
  */
void kernel_sleep(kernel_sec_t sec)
{
    kernel_sleep_ms((kernel_msec_t)sec*1000);
}