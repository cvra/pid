#ifndef PID_H_
#define PID_H_

typedef struct pid_filter_s pid_filter_t;

/** Allocates the memory for a PID filter. */
pid_filter_t *pid_create(void);

/** Frees the memory used by a PID filter. */
void pid_delete(pid_filter_t *pid);

#endif
