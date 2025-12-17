cmake_minimum_required(VERSION 3.10)
project(stack)

add_library(stack stack.c)

add_executable(main main.c)
target_link_libraries(main stack)

#add_executable(stack_tests tests.c)
#target_link_libraries(stack_tests stack)
