/*
u(t)=Kp[e(k)+T/Ti∑e(j)+Td/T[e(k)−e(k−1)]]
    =Kp*e(k)+Kp*T/Ti∑e(j)+Kp*Td/T[e(k)-e(k-1)]
    =Kp*e(k)+Ki*∑e(j)+Kd*[e(k)-e(k-1)]
Ki = Kp*T/Ti
Kd = Kp*Td/T

Δu(k)=u(k)−u(k−1)
     =Kp[e(k)-e(k-1)]+Ki*e(k)+kd[e(k)-2e(k-1)+e(k-2)]
*/

#include "pid.h"

int pid_init(struct pid *pid, float kp, float ki, float kd, float max_output, float min_output)
{

    pid->err = 0;
    pid->err_last = 0;
    pid->err_last_last = 0;
    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;
    pid->output_max = max_output;
    pid->output_min = min_output;
    
    return 0;
}

void pid_reset(struct pid *pid)
{
    pid->err=0;
    pid->err_last = 0;
    pid->err_last_last = 0;
    pid->err_sum = 0;
}

float pid_position_calc(struct pid *pid, float desired_val, float feadback, float duration)
{
    
    pid->err = desired_val - feadback;
    pid->err_sum += pid->err;
    
    pid->output = pid->kp*pid->err+pid->ki*duration*pid->err_sum+\
                    pid->kd*(pid->err-pid->err_last)/duration;

    pid->err_last = pid->err;
    
    if(pid->output>pid->output_max)
        pid->output = pid->output_max;
    if(pid->output<pid->output_min)
        pid->output = pid->output_min;
    
    return pid->output;
}

int pid_set_param(struct pid *pid, float kp, float ki, float kd)
{
    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;
    
    return 0;
}

int pid_set_limit(struct pid *pid, float output_max, float output_min)
{
    pid->output_max = output_max;
    pid->output_min = output_min;
    return 0;
}

float pid_increment_calc(struct pid *pid, float desired_val, float feedback, float duration)
{
    float duk = 0;

    pid->err = desired_val-feedback;
    pid->err_sum += pid->err;
    
    duk = pid->kp*(pid->err-pid->err_last)+pid->ki*duration*pid->err+\
          pid->kd*(pid->err-2*pid->err_last+pid->err_last_last)/duration;
          
    pid->err_last_last = pid->err_last;
    pid->err_last = pid->err;
    
    pid->output += duk;
    
    if(pid->output>pid->output_max)
        pid->output = pid->output_max;
    if(pid->output<pid->output_min)
        pid->output = pid->output_min;
    
    return pid->output;    
}




