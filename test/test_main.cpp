#include <unity.h>
#include <helpers.h>

// void setUp(void) {
// // set stuff up here
// }

// void tearDown(void) {
// // clean stuff up here
// }

void testDipCodeParse_whenWrongArea_thenFail(void) {
    // arrange
    char topic[] = "rfdevices/station1/fail/1239434953/switch";
    char code1[12];
    char code2[12];
    // act
    bool result = extractDipCodeFromTopic(topic, code1, code2);

    // assert
    TEST_ASSERT_FALSE(result);
}

int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(testDipCodeParse_whenWrongArea_thenFail);
    UNITY_END();

    return 0;
}