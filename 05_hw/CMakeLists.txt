cmake_minimum_required(VERSION 3.10)
project(stack)

# Настройки компиляции
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Для разных компиляторов
if(MSVC)
    # ============================================
    # НАСТРОЙКИ ДЛЯ VISUAL STUDIO (MSVC)
    # ============================================
    
    # Базовые предупреждения
    add_compile_options(/W4 /WX)  # Все предупреждения, считать их ошибками
    
    # ============================================
    # ВСТРОЕННЫЙ СТАТИЧЕСКИЙ АНАЛИЗАТОР MSVC
    # ============================================
    add_compile_options(/analyze)                     # Включить анализатор
    add_compile_options(/analyze:stacksize 100000)    # Увеличить размер стека анализа
    add_compile_options(/analyze:quiet)              # Только важные предупреждения
    add_compile_options(/analyze:WX-)                # Не считать предупреждения анализатора ошибками (опционально)
    
    # Дополнительные опции анализатора
    # add_compile_options(/analyze:autolog)          # Логирование в XML (для CI)
    # add_compile_options(/analyze:ruleset native.ruleset)  # Правила
    
    # Настройки времени выполнения
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    
    # Для отладочной сборки
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /Zi")  # Отладочная информация
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} /debug")
    
else()
    # ============================================
    # НАСТРОЙКИ ДЛЯ GCC/Clang (Linux/macOS)
    # ============================================
    add_compile_options(-Wall -Wextra -Werror -pedantic)
    
    # Для Clang статический анализатор
    if(CMAKE_C_COMPILER_ID MATCHES "Clang")
        # Можно добавить опции для clang-tidy или scan-build
        add_compile_options(-fanalyzer)  # Встроенный анализатор GCC
    endif()
endif()

# ============================================
# СБОРКА ПРОЕКТА
# ============================================

# 1. Библиотека стека
add_library(stack_lib stack.c)

# 2. Демонстрационная программа
add_executable(stack_demo main.c)
target_link_libraries(stack_demo stack_lib)

# 3. Тесты
#add_executable(stack_tests tests.c)
#target_link_libraries(stack_tests stack_lib)

# Установить демо как стартовый проект для Visual Studio
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT stack_demo)

# Информационное сообщение
message(STATUS "Build configuration:")
message(STATUS "  Compiler: ${CMAKE_C_COMPILER_ID}")
if(MSVC)
    message(STATUS "  Static analyzer: MSVC /analyze ENABLED")
endif()
message(STATUS "  Targets: stack_demo, stack_tests")
