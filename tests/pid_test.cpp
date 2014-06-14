#include "CppUTest/TestHarness.h"

extern "C" {
#include "../pid.h"
}

TEST_GROUP(PIDTestGroup)
{
    pid_filter_t pid;
    float output;

    void setup(void)
    {
        pid_init(&pid);
    }
};

TEST(PIDTestGroup, IntegralValueIsZeroAtInit)
{
    CHECK_EQUAL(0, pid_get_integral(&pid));
}

TEST(PIDTestGroup, PControllerAtInit)
{
    CHECK_EQUAL(1., pid_get_kp(&pid));
    CHECK_EQUAL(0., pid_get_ki(&pid));
    CHECK_EQUAL(0., pid_get_kd(&pid));
}

TEST(PIDTestGroup, CanSetGains)
{
    pid_set_gains(&pid, 10., 20., 30.);
    CHECK_EQUAL(10., pid_get_kp(&pid));
    CHECK_EQUAL(20., pid_get_ki(&pid));
    CHECK_EQUAL(30., pid_get_kd(&pid));
}

TEST(PIDTestGroup, ZeroErrorMakesForZeroOutput)
{
    output = pid_process(&pid, 0.);
    CHECK_EQUAL(0, output);
}

TEST(PIDTestGroup, ByDefaultIsIdentity)
{
    output = pid_process(&pid, 42.);
    CHECK_EQUAL(42., output);
}

TEST(PIDTestGroup, KpHasInfluenceOnOutput)
{
    pid_set_gains(&pid, 2., 0., 0.);
    output = pid_process(&pid, 21.);
    CHECK_EQUAL(42., output);
}

TEST(PIDTestGroup, IntegratorIsChangedByProcess)
{
    pid_process(&pid, 42.);
    CHECK_EQUAL(42., pid_get_integral(&pid));

    pid_process(&pid, 42.);
    CHECK_EQUAL(84., pid_get_integral(&pid));
}

TEST(PIDTestGroup, AddingKiMakesOutputIncrease)
{
    pid_set_gains(&pid, 0., 2., 0.);
    DOUBLES_EQUAL(2*42., pid_process(&pid, 42.), 1e-3);
    DOUBLES_EQUAL(2*84., pid_process(&pid, 42.), 1e-3);
}

TEST(PIDTestGroup, IntegratorWorksInNegativeToo)
{
    pid_set_gains(&pid, 0., 1., 0.);
    DOUBLES_EQUAL(-42., pid_process(&pid, -42.), 1e-3);
    DOUBLES_EQUAL(-84., pid_process(&pid, -42.), 1e-3);
}

TEST(PIDTestGroup, AddingKdCreatesDerivativeAction)
{
    pid_set_gains(&pid, 0., 0., 2.);
    DOUBLES_EQUAL(84., pid_process(&pid, 42.), 1e-3);
    DOUBLES_EQUAL(0., pid_process(&pid, 42.), 1e-3);
}

TEST(PIDTestGroup, IntegratorMaxValueIsRespected)
{
    pid_set_integral_limit(&pid, 20.);
    pid_set_gains(&pid, 0., 1., 0.);

    DOUBLES_EQUAL(20., pid_process(&pid, 20.), 1e-3);
    DOUBLES_EQUAL(20., pid_process(&pid, 20.), 1e-3);
}

TEST(PIDTestGroup, IntegratorMaxValueWorksInNegativeToo)
{
    pid_set_integral_limit(&pid, 20.);
    pid_set_gains(&pid, 0., 1., 0.);

    DOUBLES_EQUAL(-20., pid_process(&pid, -20.), 1e-3);
    DOUBLES_EQUAL(-20., pid_process(&pid, -20.), 1e-3);
}

TEST(PIDTestGroup, CanResetIntegrator)
{
    pid_set_gains(&pid, 0., 1., 0.);
    DOUBLES_EQUAL(20., pid_process(&pid, 20.), 1e-3);
    DOUBLES_EQUAL(40., pid_process(&pid, 20.), 1e-3);
    pid_reset_integral(&pid);
    DOUBLES_EQUAL(20., pid_process(&pid, 20.), 1e-3);
}

