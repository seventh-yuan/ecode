#ifndef __SYSCFG_H__
#define __SYSCFG_H__
#include "kconfig.h"

#ifndef CONFIG_USING_RTOS
#define CONFIG_USING_RTOS       1
#endif

#ifndef CONFIG_USING_DEVICE
#define CONFIG_USING_DEVICE     1
#endif

#ifndef CONFIG_USING_PRINT
#define CONFIG_USING_PRINT      1
#endif

#ifndef CONFIG_USING_KTIMER
#define CONFIG_USING_KTIMER     1
#endif



#endif