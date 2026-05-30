cmake_minimum_required(VERSION 3.14)
project(lab17_tests C)

include(CTest)
enable_testing()

add_subdirectory(unity)

add_executable(run_tests tests.c lab17.c)
target_link_libraries(run_tests unity)

add_test(NAME SumBasicTest COMMAND run_tests sum_basic)
add_test(NAME SumNegativeTest COMMAND run_tests sum_neg)
add_test(NAME DivBasicTest COMMAND run_tests div_basic)
add_test(NAME DivZeroTest COMMAND run_tests div_zero)
