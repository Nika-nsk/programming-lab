#include "unity.h"
#include <string.h>

int sum(int a, int b);
int divide(int a, int b);

void setUp(void) {}
void tearDown(void) {}

void test_sum_basic(void) {
    TEST_ASSERT_EQUAL_INT(5, sum(2, 3));
}

void test_sum_negative(void) {
    TEST_ASSERT_EQUAL_INT(-1, sum(2, -3));
}

void test_div_basic(void) {
    TEST_ASSERT_EQUAL_INT(2, divide(6, 3));
}

void test_div_zero(void) {
    TEST_ASSERT_EQUAL_INT(0, divide(10, 0));
}

int main(int argc, char** argv) {
    UNITY_BEGIN();
    
    if (argc < 2) {
        RUN_TEST(test_sum_basic);
        RUN_TEST(test_sum_negative);
        RUN_TEST(test_div_basic);
        RUN_TEST(test_div_zero);
    } else {
        if (strcmp(argv[1], "sum_basic") == 0) RUN_TEST(test_sum_basic);
        else if (strcmp(argv[1], "sum_neg") == 0) RUN_TEST(test_sum_negative);
        else if (strcmp(argv[1], "div_basic") == 0) RUN_TEST(test_div_basic);
        else if (strcmp(argv[1], "div_zero") == 0) RUN_TEST(test_div_zero);
    }
    
    return UNITY_END();
}
