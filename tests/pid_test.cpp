#include "CppUTest/TestHarness.h"

extern "C" {
#include "../pid.h"
}

TEST_GROUP(PIDTestGroup)
{
    pid_filter_t *pid;

    void setup(void)
    {
        pid = pid_create();
    }

    void teardown(void)
    {
        pid_delete(pid);
    }
};

TEST(PIDTestGroup, CanCreatePID)
{
    pid = pid_create();
}
