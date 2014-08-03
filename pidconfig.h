#ifndef PIDCONFIG_H_
#define PIDCONFIG_H_

#include <stdbool.h>

#include "platform-abstraction/criticalsection.h"

#include "pid.h"

/** Instance of a PID Configuration manager
 *
 * @note This structure is only public to be able to do static allocation of it.
 * Do not access its fields directly.
 */
typedef struct {
    float kp;
    float ki;
    float kd;
    float integrator_limit;
    float frequency;

    pid_filter_t *target_pid;

    bool has_update;
} pid_config_t;

/** Initializes a PID configuration manager. */
void pidcfg_init(pid_config_t *pid_config, pid_filter_t *pid);

/** Sets the gains of the given PID config. */
void pidcfg_set_gains(pid_config_t *pid_config, float kp, float ki, float kd);

/** Sets a maximum value for the PID integrator. */
void pidcfg_set_integral_limit(pid_config_t *pid_config, float max);

/** Sets the PID frequency for gain compensation. */
void pidcfg_set_frequency(pid_config_t *pid_config, float frequency);

/** Transfers configuration to PID */ 
void pidcfg_apply(pid_config_t *pid_config);

/** Get the current gains of the PID config */
void pidcfg_get_gains(pid_config_t *pid_config, float *kp, float *ki, float *kd);

/** Get the current integral limit of the PID config */
float pidcfg_get_integral_limit(pid_config_t *pid_cfg);

/** Get the current frequency of the PID config */
float pidcfg_get_frequency(pid_config_t *pid_cfg);

#endif
