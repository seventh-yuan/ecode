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