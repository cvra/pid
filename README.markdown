# PID Filter
[![Build Status](https://travis-ci.org/cvra/pid.png)](https://travis-ci.org/cvra/pid)

This modules implements a PID filter in parallel form for use in various control loops.
It has the following features :

* Discrete PID in parallel form.
* Maximum integrator value (ARW).


## Usage
```cpp
pid_filter_t pid;
pid_init(&pid);

/* PD controller. */
pid_set_gains(&pid, 10., 0, 4.);

while (1) {
    error = consign - motor_position;
    motor_pwm = pid_process(&pid, error);
}

```

## Frequency compensation
The user of this module can specify a frequency for the PID loop and the gains
will then be adjusted to allow the same gains to be used for various frequencies.

This is done using the function `pid_set_frequency`.
By default there is no compensation for the frequency of the PID.

## Dependencies
None
