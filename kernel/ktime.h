#ifndef __KTIME_H__
#define __KTIME_H__
#include "../global/include.h"

#ifdef __cplusplus
extern "C"{
#endif
    

#if CONFIG_USING_RTOS==1
#define KERNEL_HZ       configTICK_RATE_HZ
#else
#ifndef KERNEL_HZ
#define KERNEL_HZ       200
#endif
#endif

#define KTIMER_INITIALIZER(_func, _expires, _data)  {\
                .func = (_func),                        \
                .expires = (_expires),                  \
                .data = (_data),                }

#define KTIMER_DEFINE(_name,_func,_expires, _data) \
            struct kernel_timer name=KTIMER_INITIALIZER(_func, _expires, _data)

#define kernel_time_after(unknow, known)    ((long)(known)-(long)(unknow)<0)
#define kernel_time_before(unknow, known)    ((long)(unknow)-(long)(known)<0)
#define kernel_time_after_eq(unknow, known)  ((long)(unknow)-(long)(known)>=0)
#define kernel_time_before_eq(unknow, known) ((long)(known)-(long)(unknow)>=0)
    
/**
  * @brief This function is used to get kernel timeval.
  * @param tv_val: destination to save timeval.
  * @retval 0: ok, -1: failed.
  */
int kernel_gettime(kernel_timeval_t *tv_val);

/**
  * @brief This function is used to get kernel kernel millisecond time.
  * @retval milliseconds of kernel.
  */
kernel_msec_t kernel_get_millis(void);

/**
  * @brief This function is used to get kernel kernel second time.
  * @retval seconds of kernel.
  */
kernel_sec_t kernel_get_second(void);

/**
  * @brief This function is used to delay milliseconds.
  * @param milliseconds to delay.
  * @retval None.
  */
void kernel_delay_ms(kernel_msec_t millis);

/**
  * @brief This function is used to sleep milliseconds.
  * @param millis milliseconds to sleep.
  * @retval None.
  */
void kernel_sleep_ms(kernel_msec_t millis);

/**
  * @brief This function is used to sleep seconds.
  * @param seconds to sleep.
  * @retval None.
  */
void kernel_sleep(kernel_sec_t sec);

/**
  * @brief This function is used to init a kernel timer.
  * @param timer: kernel timer to init.
  * @retval None.
  */
void kernel_timer_init(struct kernel_timer *timer);

/**
  * @brief This function is used to setup a kernel timer.
  * @param timer: kernel timer handle.
  * @param func: kernel timer callback.
  * @param data: kernel timer callback param.
  * @retval None.
  */
void kernel_timer_setup(struct kernel_timer *timer, void (*func)(unsigned long), unsigned long data);

/**
  * @brief This function is used to add a timer to kernel.
  * @param timer: kernel timer handle.
  * @retval None.
  */
void kernel_timer_add(struct kernel_timer *timer);

/**
  * @brief This function is used to start a kernel timer.
  * @param timer: kernel timer handle.
  * @param expires: kernel timer timeout.
  * @retval None.
  */
void kernel_timer_start(struct kernel_timer *timer, kernel_time_t expires);

/**
  * @brief This function is used to modify kernel timer expires.
  * @param timer: kernel timer handle.
  * @param func: kernel timer timerout.
  * @retval 0:ok, -1:failed.
  */
int kernel_timer_mod(struct kernel_timer *timer, kernel_time_t expires);

/**
  * @brief This function is used to delete a timer from timer.
  * @param timer: kernel timer handle.
  * @retval 0:ok, -1:failed.
  */
int kernel_timer_del(struct kernel_timer *timer);

/**
  * @brief This function is used to update kernel timer.
  * @param None
  * @retval None.
  */
void kernel_timer_periodic(void);

/**
  * @brief This function is used to update wall jiffies.
  * @retval None
  */
void kernel_update_wall_jiffies(void);

#ifdef __cplusplus
}
#endif

#endif