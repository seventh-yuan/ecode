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
#if CONFIG_USING_RTOS==0
static KLIST_HEAD(_kernel_thread_head);
#endif
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

#if CONFIG_USING_RTOS==0
/**
  * @brief This function is used to add a thread to kernel.
  * @param pthread: thread handle.
  * @retval None
  */
void kernel_thread_add(kernel_thread_t *pthread)
{
    klist_add(&pthread->entry, &_kernel_thread_head);
}
/**
  * @brief This function is used to delete a thread from kernel.
  * @param pthread: thread handle.
  * @retval None
  */
void kernel_thread_del(kernel_thread_t *pthread)
{
    klist_del(&pthread->entry);
}
/**
  * @brief This function is used to dispatch kernel thread.
  * @retval None
  */
static void kernel_thread_dispatch(void)
{
    struct klist_head *pos;
    struct klist_head *n;
    kernel_thread_t *pthread;
    
    if(klist_empty(&_kernel_thread_head))
        return;
    klist_for_each_safe(pos, n, &_kernel_thread_head)
    {
        pthread = container_of(pos, kernel_thread_t, entry);
        if(pthread->thread_entry!=KL_NULL)
        {
            if(pthread->thread_entry(pthread->arg)<0)
            {
                klist_del(&pthread->entry);
            }
        }
    }
}
#endif

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
        kernel_thread_dispatch();
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