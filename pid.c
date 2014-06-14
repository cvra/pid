#include <stdlib.h>
#include <math.h>
#include "pid.h"

struct pid_filter_s {
    float kp;
    float ki;
    float kd;
    float integrator;
    float previous_value;
    float integrator_limit;
};

pid_filter_t *pid_create(void)
{
    pid_filter_t *pid;
    pid = malloc(sizeof(pid_filter_t));

    pid_set_gains(pid, 1., 0., 0.);
    pid->integrator = 0.;
    pid->previous_value = 0.;
    pid->integrator_limit = INFINITY;
    return pid;
}

void pid_delete(pid_filter_t *pid)
{
    free(pid);
}

void pid_set_gains(pid_filter_t *pid, float kp, float ki, float kd)
{
    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;
}

float pid_get_kp(const pid_filter_t *pid)
{
    return pid->kp;
}

float pid_get_ki(const pid_filter_t *pid)
{
    return pid->ki;
}

float pid_get_kd(const pid_filter_t *pid)
{
    return pid->kd;
}

float pid_get_integral(const pid_filter_t *pid)
{
    return pid->integrator;
}

float pid_process(pid_filter_t *pid, float value)
{
    float output;
    pid->integrator += value;

    if (pid->integrator > pid->integrator_limit) {
        pid->integrator = pid->integrator_limit;
    } else if (pid->integrator < -pid->integrator_limit) {
        pid->integrator = -pid->integrator_limit;
    }

    output  = pid->kp * value;
    output += pid->ki * pid->integrator;

    output += pid->kd * (value - pid->previous_value);

    pid->previous_value = value;
    return output;
}

void pid_set_integral_limit(pid_filter_t *pid, float max)
{
    pid->integrator_limit = max;
}
