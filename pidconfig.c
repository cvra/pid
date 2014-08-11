#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "platform-abstraction/mutex.h"
#include "pid.h"
#include "pidconfig.h"

/** Initializes a PID configuration manager. */
void pidcfg_init(pid_config_t *pid_config, pid_filter_t *pid)
{
    // next config
    pid_get_gains(pid, &(pid_config->kp), &(pid_config->ki), &(pid_config->kd));
    pid_config->integrator_limit = pid_get_integral_limit(pid);
    pid_config->frequency = pid_get_frequency(pid);

    // config initialization
    pid_config->target_pid = pid;
    pid_config->has_update = false;
}

/** Sets the gains of the given PID config. */
void pidcfg_set_gains(pid_config_t *pid_config, float kp, float ki, float kd)
{
    CRITICAL_SECTION_ALLOC();
    CRITICAL_SECTION_ENTER();

    pid_config->kp = kp;
    pid_config->ki = ki;
    pid_config->kd = kd;

    pid_config->has_update = true;

    CRITICAL_SECTION_EXIT();
}

/** Sets a maximum value for the PID integrator. */
void pidcfg_set_integral_limit(pid_config_t *pid_config, float max)
{
    CRITICAL_SECTION_ALLOC();
    CRITICAL_SECTION_ENTER();

    pid_config->integrator_limit = max;

    pid_config->has_update = true;

    CRITICAL_SECTION_EXIT();
}

/** Sets the PID frequency for gain compensation. */
void pidcfg_set_frequency(pid_config_t *pid_config, float frequency)
{
    CRITICAL_SECTION_ALLOC();
    CRITICAL_SECTION_ENTER();

    pid_config->frequency = frequency;

    pid_config->has_update = true;

    CRITICAL_SECTION_EXIT();
}

/** Transfer configuration to PID (thread-safe) */
void pidcfg_apply(pid_config_t *cfg)
{
    CRITICAL_SECTION_ALLOC();

    // check if something to apply
    if (!cfg->has_update) {
        return;
    }

    CRITICAL_SECTION_ENTER();

    pid_set_gains(cfg->target_pid, cfg->kp, cfg->ki, cfg->kd);
    pid_set_integral_limit(cfg->target_pid, cfg->integrator_limit);
    pid_set_frequency(cfg->target_pid, cfg->frequency);

    cfg->has_update = false;

    CRITICAL_SECTION_EXIT();
}

/** Get the current gains of the PID config */
void pidcfg_get_gains(pid_config_t *pid_cfg, float *kp, float *ki, float *kd)
{
    CRITICAL_SECTION_ALLOC();
    CRITICAL_SECTION_ENTER();

    *kp = pid_cfg->kp;
    *ki = pid_cfg->ki;
    *kd = pid_cfg->kd;

    CRITICAL_SECTION_EXIT();
}

/** Get the current integral limit of the PID config */
float pidcfg_get_integral_limit(pid_config_t *pid_cfg)
{
    float limit;
    CRITICAL_SECTION_ALLOC();

    CRITICAL_SECTION_ENTER();

    limit = pid_cfg->integrator_limit;

    CRITICAL_SECTION_EXIT();

    return limit;
}

/** Get the current frequency of the PID config */
float pidcfg_get_frequency(pid_config_t *pid_cfg)
{
    float freq;
    CRITICAL_SECTION_ALLOC();

    CRITICAL_SECTION_ENTER();

    freq = pid_cfg->frequency;

    CRITICAL_SECTION_EXIT();

    return freq;
}
