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

TEST(PIDTestGroup, IntegralValueIsZeroAtInit)
{
    CHECK_EQUAL(0, pid_get_integral(pid));
}

TEST(PIDTestGroup, CanSetGains)
{
    pid_set_gains(pid, 10., 20., 30.);
    CHECK_EQUAL(10., pid_get_kp(pid));
    CHECK_EQUAL(20., pid_get_ki(pid));
    CHECK_EQUAL(30., pid_get_kd(pid));
}
