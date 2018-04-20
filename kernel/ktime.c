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
__kernel_volatile unsigned long kernel_jiffies = 0;
static KLIST_HEAD(_kernel_timer_head); 
/*Public variables---------------------------------------*/
/*Private functions--------------------------------------*/
/*Public functions---------------------------------------*/


/**
  * @brief This function is used to get kernel timeval.
  * @param tv_val: destination to save timeval.
  * @retval 0: ok, -1: failed.
  */
int kernel_gettime(kernel_timeval_t *tv_val)
{
    unsigned long temp = kernel_jiffies;
    
    if(tv_val==NULL)
        return -1;
    tv_val->tv_sec = kernel_jiffies/KERNEL_HZ;
    
    tv_val->tv_usec = (kernel_jiffies-tv_val->tv_sec*KERNEL_HZ)*1000000/KERNEL_HZ;
    
    return 0;
}

/**
  * @brief This function is used to get kernel kernel millisecond time.
  * @retval milliseconds of kernel.
  */
kernel_millis_t kernel_get_millis(void)
{
    return kernel_jiffies*1000/KERNEL_HZ;
}

/**
  * @brief This function is used to get kernel kernel second time.
  * @retval seconds of kernel.
  */
kernel_sec_t kernel_get_second(void)
{
    return kernel_jiffies*1.0/KERNEL_HZ;
}

/**
  * @brief This function is used to delay milliseconds.
  * @param milliseconds to delay.
  * @retval None.
  */
void kernel_mdelay(kernel_millis_t millis)
{
    unsigned long timeout = kernel_jiffies + millis/1000*KERNEL_HZ;
    
    while(kernel_time_before(kernel_jiffies, timeout));
        
}
/**
  * @brief This function is used to delay micro seconds.
  * @param micro seconds to delay.
  * @retval None.
  */
void kernel_udelay(kernel_usec_t us)
{
    plat_udelay(us);
}
/**
  * @brief This function is used to sleep milliseconds.
  * @param millis milliseconds to sleep.
  * @retval None.
  */
void kernel_msleep(kernel_millis_t millis)
{
#if CONFIG_USING_RTOS==1
    osDelay(millis);
#else
    kernel_mdelay(millis);
#endif
}

/**
  * @brief This function is used to sleep seconds.
  * @param seconds to sleep.
  * @retval None.
  */
void kernel_sleep(kernel_sec_t sec)
{
    kernel_msleep((kernel_millis_t)sec*1000);
}

#if CONFIG_USING_KTIMER==1
/**
  * @brief This function is used to init a kernel timer.
  * @param timer: kernel timer to init.
  * @retval None.
  */
void kernel_timer_init(struct kernel_timer *timer)
{
    timer->entry.next = KL_NULL;
}

/**
  * @brief This function is used to setup a kernel timer.
  * @param timer: kernel timer handle.
  * @param func: kernel timer callback.
  * @param data: kernel timer callback param.
  * @retval None.
  */
void kernel_timer_setup(struct kernel_timer *timer, void (*func)(unsigned long), unsigned long data)
{
    timer->func = func;
    timer->data = data;
    
    kernel_timer_init(timer);
}

/**
  * @brief This function is used to add a timer to kernel.
  * @param timer: kernel timer handle.
  * @retval None.
  */
void kernel_timer_add(struct kernel_timer *timer)
{
    klist_add(&_kernel_timer_head, &timer->entry);
}

/**
  * @brief This function is used to start a kernel timer.
  * @param timer: kernel timer handle.
  * @param expires: kernel timer timeout.
  * @retval None.
  */
void kernel_timer_start(struct kernel_timer *timer, kernel_time_t expires)
{
    timer->expires = expires;
    kernel_timer_add(timer);
}

/**
  * @brief This function is used to modify kernel timer expires.
  * @param timer: kernel timer handle.
  * @param func: kernel timer timerout.
  * @retval 0:ok, -1:failed.
  */
int kernel_timer_mod(struct kernel_timer *timer, kernel_time_t expires)
{
    if(timer==KL_NULL)
        return -1;
    timer->expires = expires;
    return 0;
}
/**
  * @brief This function is used to delete a timer from timer.
  * @param timer: kernel timer handle.
  * @retval 0:ok, -1:failed.
  */
int kernel_timer_del(struct kernel_timer *timer)
{
    if(timer==KL_NULL)
        return -1;
    klist_del(&timer->entry);
    return 0;
}
/**
  * @brief This function is used to update kernel timer.
  * @param None
  * @retval None.
  */
void kernel_timer_periodic(void)
{
    struct klist_head *entry;
    struct klist_head *tmp;
    klist_for_each_safe(entry, tmp, &_kernel_timer_head)
    {
        struct kernel_timer *timer = klist_entry(entry, struct kernel_timer, entry);
        if(kernel_time_after(kernel_jiffies, timer->expires))
        {
            kernel_timer_del(timer);
            if(timer->func!=KL_NULL)
            {
                timer->func(timer->data);
            }
        }
    }
}

#endif
/**
  * @brief This function is used to update wall jiffies.
  * @retval None
  */
void kernel_update_wall_jiffies(void)
{
    kernel_jiffies++;
#if CONFIG_USING_KTIMER==1
    kernel_timer_periodic();
#endif
}