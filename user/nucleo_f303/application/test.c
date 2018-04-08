#include "test.h"

#if CONFIG_USING_RTOS==1
#include "arm_math.h"


void std_fabs_test(float arg1, float arg2, int count)
{
    float delta = (arg2-arg1)/count;
    float result = 0;
    kernel_msec_t start_time = kernel_get_millis();
    for(;arg1<arg2;arg1+=delta)
    {
        fabs(arg1);
    }
    kernel_msec_t end_time = kernel_get_millis();
    kprintf("std fabs time:%d", end_time-start_time);
    kprintf("result=%d\n", result);
}

void arm_fabs_test(float arg1, float arg2, int count)
{
    float delta = (arg2-arg1)/count;
    float result = 0;
    kernel_msec_t start_time = kernel_get_millis();
    for(;arg1<arg2;arg1+=delta)
    {
        arm_abs_f32(&arg1, &result, 1);
    }
    kernel_msec_t end_time = kernel_get_millis();
    kprintf("dsp fabs time:%d", end_time-start_time);
    kprintf("result=%d\n", result);
}




void do_test(void)
{
    std_fabs_test(1,100,5000000);
    arm_fabs_test(1,100,5000000);
}


void test_thread(void *args)
{
    
    for(;;)
    {
        
    }
}

void test_init(void)
{
    

}

#endif