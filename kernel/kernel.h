#ifndef __KERNEL_H__
#define __KERNEL_H__
#include "../global/include.h"

#ifdef __cplusplus
extern "C"{
#endif

/**
  * @brief This function is used to init kernel.
  * @retval None
  */
void kernel_init(void);
#if CONFIG_USING_RTOS==0
/**
  * @brief This function is used to add a thread to kernel.
  * @param pthread: thread handle.
  * @retval None
  */
void kernel_thread_add(kernel_thread_t *pthread);
/**
  * @brief This function is used to delete a thread from kernel.
  * @param pthread: thread handle.
  * @retval None
  */
void kernel_thread_del(kernel_thread_t *pthread);
#endif
/**
  * @brief This function is used to start kernel.
  * @retval None
  */
void kernel_start(void);
/**
  * @brief This function is used to update kernel period event.
  * @retval None
  */
void kernel_periodic(void);
#ifdef __cplusplus
}
#endif
#endif