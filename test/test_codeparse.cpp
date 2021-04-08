#include <unity.h>
#include <DeviceSwitchFactory.h>
#include <RFSender.h>

class RFSenderMock : public IRFSender
{
    void switchOn(char *code1, char *code2) override
    {
    }

    void switchOff(char *code1, char *code2) override
    {
    }
    
    void send(unsigned long code, unsigned int length) override
    {
    }
};

void testDipCodeParse_whenWrongArea_thenReturnNULL(void)
{
    // arrange
    RFSenderMock sender = RFSenderMock();
    DeviceSwitchFactory factory = DeviceSwitchFactory((IRFSender *)&sender);
    char topic[] = "rfdevices/station1/fail/1239434953/switch";

    IDeviceSwitch *deviceSwitch = factory.create(topic);

    // assert
    TEST_ASSERT_NULL(deviceSwitch);
}

void testDipCodeParse_whenCodeCorrect_thenExtractCodes(void)
{
    // arrange
    RFSenderMock sender = RFSenderMock();
    DeviceSwitchFactory factory = DeviceSwitchFactory((IRFSender *)&sender);
    char topic[] = "rfdevices/station1/dip/11110:11100/switch";

    // act
    IDeviceSwitch *deviceSwitch = factory.create(topic);

    // assert
    // TODO: test again
    //TEST_ASSERT_NOT_NULL(deviceSwitch);
    //TEST_ASSERT_EQUAL_STRING("11110", ((DipDeviceSwitch*) deviceSwitch)->getCode1());
    //TEST_ASSERT_EQUAL_STRING("11100", ((DipDeviceSwitch*) deviceSwitch)->getCode2());
}

void testDipCodeParse_whenCodeTooShort_thenReturnNULL(void)
{
    // arrange
    RFSenderMock sender = RFSenderMock();
    DeviceSwitchFactory factory = DeviceSwitchFactory((IRFSender *)&sender);
    char topic[] = "rfdevices/station1/dip/11110:1100/switch";
    char code1[12];
    char code2[12];

    IDeviceSwitch *deviceSwitch = factory.create(topic);

    // assert
    TEST_ASSERT_NULL(deviceSwitch);
}

int main(int argc, char **argv)
{
    UNITY_BEGIN();
    RUN_TEST(testDipCodeParse_whenWrongArea_thenReturnNULL);
    RUN_TEST(testDipCodeParse_whenCodeCorrect_thenExtractCodes);
    RUN_TEST(testDipCodeParse_whenCodeTooShort_thenReturnNULL);
    UNITY_END();

    return 0;
}