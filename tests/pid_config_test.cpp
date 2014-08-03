#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include "platform-abstraction/tests/criticalsection_capturing_mock.h"

extern "C" {
#include "../pidconfig.h"
#include "platform-abstraction/mutex.h"
#include "platform-abstraction/mutex.h"
}

static const float pid_kp = 22.;
static const float pid_ki = 23.;
static const float pid_kd = 24;
static const float pid_int_limit = 25;
static const float pid_freq = 26.;

/** 
 * \group Unit & integration tests
 * @{
 */

TEST_GROUP(PIDConfigRealPIDTestGroup)
{
    pid_config_t pid_config;
    pid_filter_t pid;

    void setup(void)
    {
        pid_init(&pid);

        pid_set_gains(&pid,pid_kp,pid_ki,pid_kd);
        pid_set_integral_limit(&pid,pid_int_limit);
        pid_set_frequency(&pid,pid_freq);

        pidcfg_init(&pid_config,&pid);
    }

    void teardown(void)
    {
        mock().clear();
    }
};

TEST(PIDConfigRealPIDTestGroup, ValuesArePIDsAtInit)
{
    float kp,ki,kd;
    pidcfg_get_gains(&pid_config,&kp,&ki,&kd);
    CHECK_EQUAL(pid_kp, kp);
    CHECK_EQUAL(pid_ki, ki);
    CHECK_EQUAL(pid_kd, kd);
    CHECK_EQUAL(pid_int_limit, pidcfg_get_integral_limit(&pid_config));
    CHECK_EQUAL(pid_freq, pidcfg_get_frequency(&pid_config));
}

TEST(PIDConfigRealPIDTestGroup, PIDCfgIsThreadSafe_GetFrequency)
{
    criticalsection_use_capturing_mock();

    mock().expectOneCall("CPU_CRITICAL_ENTER");
    mock().expectOneCall("CPU_CRITICAL_EXIT");

    pidcfg_get_frequency(&pid_config);

    mock().checkExpectations();
}

TEST(PIDConfigRealPIDTestGroup, PIDCfgIsThreadSafe_GetIntegralLimit)
{
    criticalsection_use_capturing_mock();

    mock().expectOneCall("CPU_CRITICAL_ENTER");
    mock().expectOneCall("CPU_CRITICAL_EXIT");

    pidcfg_get_integral_limit(&pid_config);

    mock().checkExpectations();
}

TEST(PIDConfigRealPIDTestGroup, PIDCfgIsThreadSafe_GetGains)
{
    float kp,ki,kd;
    criticalsection_use_capturing_mock();

    mock().expectOneCall("CPU_CRITICAL_ENTER");
    mock().expectOneCall("CPU_CRITICAL_EXIT");

    pidcfg_get_gains(&pid_config,&kp,&ki,&kd);

    mock().checkExpectations();
}

TEST(PIDConfigRealPIDTestGroup, PIDCfgIsThreadSafe_SetFrequency)
{
    criticalsection_use_capturing_mock();

    mock().expectOneCall("CPU_CRITICAL_ENTER");
    mock().expectOneCall("CPU_CRITICAL_EXIT");

    pidcfg_set_frequency(&pid_config,1.);

    mock().checkExpectations();
}

TEST(PIDConfigRealPIDTestGroup, PIDCfgIsThreadSafe_SetIntegralLimit)
{
    criticalsection_use_capturing_mock();

    mock().expectOneCall("CPU_CRITICAL_ENTER");
    mock().expectOneCall("CPU_CRITICAL_EXIT");

    pidcfg_set_integral_limit(&pid_config,1.);

    mock().checkExpectations();
}

TEST(PIDConfigRealPIDTestGroup, PIDCfgIsThreadSafe_SetGains)
{
    criticalsection_use_capturing_mock();

    mock().expectOneCall("CPU_CRITICAL_ENTER");
    mock().expectOneCall("CPU_CRITICAL_EXIT");

    pidcfg_set_gains(&pid_config,1.,2.,3.);

    mock().checkExpectations();
}

TEST(PIDConfigRealPIDTestGroup, PIDCfgIsThreadSafe_Apply)
{
    pidcfg_set_integral_limit(&pid_config,1.);

    criticalsection_use_capturing_mock();
    
    mock().expectOneCall("CPU_CRITICAL_ENTER");
    mock().expectOneCall("CPU_CRITICAL_EXIT");

    pidcfg_apply(&pid_config);

    mock().checkExpectations();
}

TEST(PIDConfigRealPIDTestGroup, PIDParametersChangeOnApply_Frequency)
{
    const float newFreq = 319.;
    pidcfg_set_frequency(&pid_config,newFreq);

    CHECK_EQUAL(pid_get_frequency(&pid),pid_freq);

    pidcfg_apply(&pid_config);

    CHECK_EQUAL(pid_get_frequency(&pid),newFreq);
}

TEST(PIDConfigRealPIDTestGroup, PIDParametersChangeOnApply_IntegralLimit)
{
    const float newLimit = 5.;

    // prepare changes, they aren't effective yet
    pidcfg_set_integral_limit(&pid_config,newLimit);
    CHECK_EQUAL(pid_get_integral_limit(&pid),pid_int_limit);

    // now changes should be effective
    pidcfg_apply(&pid_config);
    CHECK_EQUAL(pid_get_integral_limit(&pid),newLimit);
}

TEST(PIDConfigRealPIDTestGroup, PIDParametersChangeOnApply_Gains)
{
    const float newKp = 1.;
    const float newKi = 2.;
    const float newKd = 3.;
    float kp,ki,kd;

    // prepare changes, they aren't effective yet
    pidcfg_set_gains(&pid_config,newKp,newKi,newKd);
    pid_get_gains(&pid,&kp,&ki,&kd);
    CHECK_EQUAL(kp,pid_kp);
    CHECK_EQUAL(ki,pid_ki);
    CHECK_EQUAL(kd,pid_kd);

    // now changes should be effective
    pidcfg_apply(&pid_config);
    pid_get_gains(&pid,&kp,&ki,&kd);
    CHECK_EQUAL(kp,newKp);
    CHECK_EQUAL(ki,newKi);
    CHECK_EQUAL(kd,newKd);

}

TEST(PIDConfigRealPIDTestGroup, PIDMultipleChanges)
{
    const float newKp = 1.;
    const float newKi = 2.;
    const float newKd = 3.;
    const float newLimit = 5.;

    float kp,ki,kd;

    // these modifications should be ignored
    pidcfg_set_gains(&pid_config,newKp*4,newKi*4,newKd*4);
    pidcfg_set_gains(&pid_config,newKp*2,newKi*2,newKd*2);
    // as they're superseded by these
    pidcfg_set_integral_limit(&pid_config,newLimit);
    pidcfg_set_gains(&pid_config,newKp,newKi,newKd);

    pid_get_gains(&pid,&kp,&ki,&kd);
    CHECK_EQUAL(kp,pid_kp);
    CHECK_EQUAL(ki,pid_ki);
    CHECK_EQUAL(kd,pid_kd);
    CHECK_EQUAL(pid_get_integral_limit(&pid),pid_int_limit);

    // changes should now be effective
    pidcfg_apply(&pid_config);
    pid_get_gains(&pid,&kp,&ki,&kd);
    CHECK_EQUAL(kp,newKp);
    CHECK_EQUAL(ki,newKi);
    CHECK_EQUAL(kd,newKd);
    CHECK_EQUAL(pid_get_integral_limit(&pid),newLimit);
}

/**@}*/
