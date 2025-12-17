cmake_minimum_required(VERSION 3.10)
project(stack)

# Просто и понятно
add_library(stack_lib stack.c)

add_executable(demo main.c)
target_link_libraries(demo stack_lib)

add_executable(tests tests.c)
target_link_libraries(tests stack_lib)

add_executable(benchmarks benchmarks.c)
target_link_libraries(benchmarks stack_lib)

# ТОЛЬКО для Windows
if(WIN32)
    # Включаем анализатор
    add_compile_options(/W4 /WX /analyze)
    
    # Для отладочной информации
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG "/debug")
    
    # Ставим Release по умолчанию
    if(NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE Release)
    endif()
endif()
