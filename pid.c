#include <stdlib.h>
#include "pid.h"

struct pid_filter_s {

};

pid_filter_t *pid_create(void)
{
    pid_filter_t *pid;
    pid = malloc(sizeof(pid_filter_t));
    return pid;
}

void pid_delete(pid_filter_t *pid)
{
    free(pid);
}
