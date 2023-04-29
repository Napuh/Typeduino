#include "unity.h"
#include <utils.h>

// Declare the functions that will be tested
int getSpace(int start, int end);
bool equals(char *strA, int start, int end, char *strB, int strLen);
int toPositive(int num);
bool equalsBuffer(int start, int end, char *str);
int getInt(char *str, int pos);

void setUp(void)
{
    strncpy(buf, "Hello world", buffersize - 1); // Asegúrate de no desbordar el búfer
    buf[buffersize - 1] = '\0';
}

void tearDown(void)
{
    // Clean up any resources allocated during the tests
}

void test_equals(void)
{
    char buf[] = "Hello world";
    bool result = equals(buf, 0, 5, "Hello", 5);
    TEST_ASSERT_TRUE(result);

    result = equals(buf, 0, 5, "World", 5);
    TEST_ASSERT_FALSE(result);
}

void test_equalsBuffer(void)
{
    bool result = equalsBuffer(0, 5, "Hello");
    TEST_ASSERT_TRUE(result);

    result = equalsBuffer(0, 5, "World");
    TEST_ASSERT_FALSE(result);
}

void test_getSpace(void)
{
    int result = getSpace(0, sizeof(buf));
    TEST_ASSERT_EQUAL(5, result);
}

void test_toPositive(void)
{
    int result = toPositive(42);
    TEST_ASSERT_EQUAL(42, result);

    result = toPositive(-42);
    TEST_ASSERT_EQUAL(42, result);
}

void test_getInt(void)
{
    char buf[] = "Hello RANDOM 12345";
    int result = getInt(buf, 6);
    TEST_ASSERT_TRUE(result >= -100 && result <= 100);

    result = getInt(buf, 12);
    TEST_ASSERT_EQUAL(12345, result);
}

int main(int argc, char **argv)
{
    UNITY_BEGIN();
    RUN_TEST(test_equals);
    RUN_TEST(test_toPositive);
    RUN_TEST(test_getInt);
    RUN_TEST(test_equalsBuffer);
    RUN_TEST(test_getSpace);
    UNITY_END();

    return 0;
}
