# PID Filter
[![Build Status](https://travis-ci.org/cvra/pid.png)](https://travis-ci.org/cvra/pid)

This modules implements a PID filter in parallel form for use in various control loops.


## Usage
```cpp
pid_filter_t *pid = pid_create();

/* PD controller. */
pid_set_gains(pid, 10., 0, 4.);

while (1) {
    error = consign - motor_position;
    motor_pwm = pid_process(pid, error);
}

/* Free memory */
pid_delete(pid);
```

## Dependencies
None
