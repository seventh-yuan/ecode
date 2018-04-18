#ifndef __PLAT_H__
#define __PLAT_H__
#include "../global/include.h"

#ifdef __cplusplus
extern "C"{
#endif


/**
  * @brief This function implement cpu us delay.
  * @param us: us to delay.
  * @retval None.
  */
void plat_udelay(kernel_time_t us);

#ifdef __cplusplus
}
#endif
#endif