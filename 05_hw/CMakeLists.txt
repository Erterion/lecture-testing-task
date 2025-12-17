cmake_minimum_required(VERSION 3.10)
project(stack LANGUAGES C)

# ОПЦИИ КОНФИГУРАЦИИ

# Опция для включения санитайзеров
option(ENABLE_SANITIZERS "Включить санитайзеры (Address, Undefined)" ON)
option(ENABLE_ANALYZER "Включить статический анализатор MSVC" ON)

# Настройки компиляции
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# НАСТРОЙКИ КОМПИЛЯТОРА

if(MSVC)
    # Базовые предупреждения
    add_compile_options(/W4 /WX)
    
    # Статический анализатор MSVC
    if(ENABLE_ANALYZER)
        add_compile_options(/analyze)
        add_compile_options(/analyze:stacksize 100000)
        add_compile_options(/analyze:quiet)
        message(STATUS "MSVC Static Analyzer: ENABLED")
    else()
        message(STATUS "MSVC Static Analyzer: DISABLED")
    endif()
    
    # Для MSVC нет встроенных санитайзеров как в GCC/Clang,
    # но можно использовать AddressSanitizer в новых версиях
    if(ENABLE_SANITIZERS AND CMAKE_C_COMPILER_VERSION VERSION_GREATER_EQUAL "16.9")
        # AddressSanitizer для MSVC (требует Visual Studio 2019 16.9+)
        add_compile_options(/fsanitize=address)
        add_link_options(/fsanitize=address)
        message(STATUS "AddressSanitizer for MSVC: ENABLED")
    elseif(ENABLE_SANITIZERS)
        message(WARNING "AddressSanitizer requires Visual Studio 2019 16.9 or later")
    endif()
    
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    
else()
    # Для GCC/Clang
    add_compile_options(-Wall -Wextra -Werror -pedantic)
    
    # САНИТАЙЗЕРЫ ДЛЯ GCC/CLANG
    if(ENABLE_SANITIZERS)
        if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
            # AddressSanitizer (обнаружение утечек памяти, переполнений буфера)
            add_compile_options(-fsanitize=address)
            add_link_options(-fsanitize=address)
            
            # UndefinedBehaviorSanitizer (обнаружение неопределенного поведения)
            add_compile_options(-fsanitize=undefined)
            add_link_options(-fsanitize=undefined)
            
            # Дополнительные опции для UBSan
            add_compile_options(-fsanitize=float-divide-by-zero)
            add_compile_options(-fsanitize=null)
            add_compile_options(-fsanitize=return)
            add_compile_options(-fsanitize=shift)
            add_compile_options(-fsanitize=signed-integer-overflow)
            add_compile_options(-fsanitize=vla-bound)
            
            # Включить трассировку стека
            add_compile_options(-fno-omit-frame-pointer)
            
            # Отладочные символы всегда
            add_compile_options(-g)
            
            message(STATUS "Sanitizers (ASan + UBSan): ENABLED")
            
            # Проверяем поддержку компилятора
            if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
                if(CMAKE_C_COMPILER_VERSION VERSION_LESS "4.8")
                    message(WARNING "GCC version < 4.8, sanitizers may not work properly")
                endif()
            elseif(CMAKE_C_COMPILER_ID STREQUAL "Clang")
                if(CMAKE_C_COMPILER_VERSION VERSION_LESS "3.1")
                    message(WARNING "Clang version < 3.1, sanitizers may not work properly")
                endif()
            endif()
        else()
            message(WARNING "Sanitizers not supported for this compiler: ${CMAKE_C_COMPILER_ID}")
        endif()
    else()
        message(STATUS "Sanitizers: DISABLED")
    endif()
endif()

# СБОРКА ПРОЕКТА

# Библиотека стека
add_library(stack_lib stack.c)

# Демонстрационная программа
add_executable(stack_demo main.c)
target_link_libraries(stack_demo stack_lib)

# Тесты
add_executable(stack_tests tests.c)
target_link_libraries(stack_tests stack_lib)

# 3. Бенчмарки
add_executable(stack_benchmarks benchmarks.c)
target_link_libraries(stack_benchmarks stack_lib)

# Установить демо как стартовый проект для Visual Studio
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT stack_demo)

# ВЫВОД ИНФОРМАЦИИ О КОНФИГУРАЦИИ

message(STATUS "=" * 50)
message(STATUS "Build Configuration:")
message(STATUS "  Compiler: ${CMAKE_C_COMPILER_ID} ${CMAKE_C_COMPILER_VERSION}")
message(STATUS "  Sanitizers: ${ENABLE_SANITIZERS}")
message(STATUS "  Targets: stack_demo, stack_tests")
message(STATUS "=" * 50)
