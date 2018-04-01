#ifndef __PID_H__
#define __PID_H__
#include "../global/include.h"

#ifdef __cplusplus
extern "C" {
#endif

struct pid{
    float err;//偏差
    float err_last;
    float err_last_last;
    float err_sum;
    float kp,ki,kd;
    float output;
    float output_max;
    float output_min;
};


int pid_init(struct pid *pid, float kp, float ki, float kd, float max_output, float min_output);

void pid_reset(struct pid *pid);

int pid_set_param(struct pid *pid, float kp, float ki, float kd);

int pid_set_limit(struct pid *pid, float output_max, float output_min);

float pid_position_calc(struct pid *pid, float desired_val, float feadback, float duration);

float pid_increment_calc(struct pid *pid, float desired_val, float feedback,float duration);
#ifdef __cplusplus
}
#endif
#endif