#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "stack.h"

// Константы для бенчмарков
#define SMALL_SIZE     1000
#define MEDIUM_SIZE    10000
#define LARGE_SIZE     100000
#define HUGE_SIZE      1000000

// Утилиты для измерения времени
typedef struct {
    clock_t start;
    clock_t end;
} Timer;

static void timer_start(Timer* t) {
    t->start = clock();
}

static void timer_stop(Timer* t) {
    t->end = clock();
}

static double timer_elapsed_ms(Timer* t) {
    return ((double)(t->end - t->start) / CLOCKS_PER_SEC) * 1000.0;
}

static double timer_elapsed_us(Timer* t) {
    return ((double)(t->end - t->start) / CLOCKS_PER_SEC) * 1000000.0;
}

// БЕНЧМАРКИ ДЛЯ PUSH

void benchmark_push_single() {
    printf("\n=== Benchmark: Single Push ===\n");

    Stack s;
    initStack(&s);

    Timer timer;
    timer_start(&timer);

    if (!push(&s, 42)) {
        fprintf(stderr, "Push failed!\n");
        return;
    }

    timer_stop(&timer);

    printf("Operation: push single element\n");
    printf("Time: %.3f microseconds\n", timer_elapsed_us(&timer));
    printf("Result: %s\n", (s.top != NULL && s.top->data == 42) ? "OK" : "FAIL");

    destroyStack(&s);
}

void benchmark_push_multiple(int num_elements) {
    printf("\n=== Benchmark: Push %d elements ===\n", num_elements);

    Stack s;
    initStack(&s);

    Timer timer;
    timer_start(&timer);

    for (int i = 0; i < num_elements; i++) {
        if (!push(&s, i)) {
            fprintf(stderr, "Push failed at iteration %d\n", i);
            destroyStack(&s);
            return;
        }
    }

    timer_stop(&timer);

    double total_time_ms = timer_elapsed_ms(&timer);
    double avg_time_us = (total_time_ms * 1000.0) / num_elements;

    printf("Total elements: %d\n", num_elements);
    printf("Total time: %.3f ms\n", total_time_ms);
    printf("Average time per push: %.3f µs\n", avg_time_us);

    // Проверяем, что все элементы добавлены
    int count = 0;
    Node* current = s.top;
    while (current != NULL) {
        count++;
        current = current->next;
    }
    printf("Actual elements in stack: %d\n", count);
    printf("Verification: %s\n", (count == num_elements) ? "PASS" : "FAIL");

    destroyStack(&s);
}

// БЕНЧМАРКИ ДЛЯ POP

void benchmark_pop_single() {
    printf("\n=== Benchmark: Single Pop ===\n");

    Stack s;
    initStack(&s);
    push(&s, 42);

    Timer timer;
    timer_start(&timer);

    int value;
    if (!pop(&s, &value)) {
        fprintf(stderr, "Pop failed!\n");
        destroyStack(&s);
        return;
    }

    timer_stop(&timer);

    printf("Operation: pop single element\n");
    printf("Time: %.3f microseconds\n", timer_elapsed_us(&timer));
    printf("Popped value: %d (expected: 42)\n", value);
    printf("Stack empty after pop: %s\n", isEmpty(&s) ? "YES" : "NO");

    destroyStack(&s);
}

void benchmark_pop_multiple(int num_elements) {
    printf("\n=== Benchmark: Pop %d elements ===\n", num_elements);

    // Создаем и заполняем стек
    Stack s;
    initStack(&s);

    for (int i = 0; i < num_elements; i++) {
        push(&s, i);
    }

    Timer timer;
    timer_start(&timer);

    int value;
    int successful_pops = 0;
    for (int i = 0; i < num_elements; i++) {
        if (pop(&s, &value)) {
            successful_pops++;
            // Проверяем, что значения извлекаются в правильном порядке (LIFO)
            if (value != num_elements - i - 1) {
                printf("Warning: Unexpected value %d at pop %d (expected %d)\n",
                    value, i, num_elements - i - 1);
            }
        }
        else {
            fprintf(stderr, "Pop failed at iteration %d\n", i);
            break;
        }
    }

    timer_stop(&timer);

    double total_time_ms = timer_elapsed_ms(&timer);
    double avg_time_us = (total_time_ms * 1000.0) / successful_pops;

    printf("Attempted pops: %d\n", num_elements);
    printf("Successful pops: %d\n", successful_pops);
    printf("Total time: %.3f ms\n", total_time_ms);
    printf("Average time per pop: %.3f µs\n", avg_time_us);
    printf("Stack empty after all pops: %s\n", isEmpty(&s) ? "YES" : "NO");

    destroyStack(&s);
}

// БЕНЧМАРКИ ДЛЯ PUSH+POP (СМЕШАННЫЕ ОПЕРАЦИИ)

void benchmark_push_pop_alternating(int num_operations) {
    printf("\n=== Benchmark: Alternating Push/Pop (%d ops) ===\n", num_operations);

    Stack s;
    initStack(&s);

    Timer timer;
    timer_start(&timer);

    int value;
    int push_count = 0;
    int pop_count = 0;

    for (int i = 0; i < num_operations; i++) {
        // Чередуем push и pop
        if (i % 2 == 0) {
            if (push(&s, i)) {
                push_count++;
            }
        }
        else {
            if (pop(&s, &value)) {
                pop_count++;
            }
        }
    }

    timer_stop(&timer);

    double total_time_ms = timer_elapsed_ms(&timer);
    double avg_time_us = (total_time_ms * 1000.0) / num_operations;

    printf("Total operations: %d\n", num_operations);
    printf("  Push operations: %d\n", push_count);
    printf("  Pop operations: %d\n", pop_count);
    printf("Total time: %.3f ms\n", total_time_ms);
    printf("Average time per operation: %.3f µs\n", avg_time_us);

    // Очищаем оставшиеся элементы
    int remaining = 0;
    while (!isEmpty(&s)) {
        pop(&s, &value);
        remaining++;
    }
    printf("Remaining elements cleared: %d\n", remaining);

    destroyStack(&s);
}

void benchmark_push_pop_sequence(int sequence_size, int repetitions) {
    printf("\n=== Benchmark: Push/Pop Sequence (size=%d, reps=%d) ===\n",
        sequence_size, repetitions);

    Stack s;
    initStack(&s);

    Timer timer;
    timer_start(&timer);

    int value;
    int total_operations = 0;

    for (int rep = 0; rep < repetitions; rep++) {
        // Push sequence_size элементов
        for (int i = 0; i < sequence_size; i++) {
            push(&s, i + rep * sequence_size);
            total_operations++;
        }

        // Pop sequence_size элементов
        for (int i = 0; i < sequence_size; i++) {
            pop(&s, &value);
            total_operations++;
        }
    }

    timer_stop(&timer);

    double total_time_ms = timer_elapsed_ms(&timer);
    double avg_time_us = (total_time_ms * 1000.0) / total_operations;

    printf("Sequence size: %d\n", sequence_size);
    printf("Repetitions: %d\n", repetitions);
    printf("Total operations: %d\n", total_operations);
    printf("Total time: %.3f ms\n", total_time_ms);
    printf("Average time per operation: %.3f µs\n", avg_time_us);
    printf("Stack empty at end: %s\n", isEmpty(&s) ? "YES" : "NO");

    destroyStack(&s);
}

// БЕНЧМАРКИ ДЛЯ ГРАНИЧНЫХ СЛУЧАЕВ

void benchmark_empty_stack_operations() {
    printf("\n=== Benchmark: Empty Stack Operations ===\n");

    Stack s;
    initStack(&s);

    Timer timer;
    timer_start(&timer);

    int value;
    int iterations = 1000000;
    int failed_pops = 0;

    // Многократно пытаемся извлечь из пустого стека
    for (int i = 0; i < iterations; i++) {
        if (!pop(&s, &value)) {
            failed_pops++;  // Ожидаемое поведение
        }
    }

    timer_stop(&timer);

    double total_time_ms = timer_elapsed_ms(&timer);
    double avg_time_us = (total_time_ms * 1000.0) / iterations;

    printf("Operations: pop from empty stack\n");
    printf("Iterations: %d\n", iterations);
    printf("Failed pops (expected): %d\n", failed_pops);
    printf("Total time: %.3f ms\n", total_time_ms);
    printf("Average time per operation: %.3f µs\n", avg_time_us);

    destroyStack(&s);
}

void benchmark_small_stacks() {
    printf("\n=== Benchmark: Small Stacks ===\n");

    int sizes[] = { 1, 2, 5, 10, 20, 50, 100 };
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);

    for (int i = 0; i < num_sizes; i++) {
        int size = sizes[i];

        Stack s;
        initStack(&s);

        Timer timer;
        timer_start(&timer);

        // Push size элементов
        for (int j = 0; j < size; j++) {
            push(&s, j);
        }

        // Pop size элементов
        int value;
        for (int j = 0; j < size; j++) {
            pop(&s, &value);
        }

        timer_stop(&timer);

        double total_time_us = timer_elapsed_us(&timer);
        double avg_time_us = total_time_us / (2 * size);

        printf("Size: %3d elements | Total: %7.2f µs | Avg: %6.3f µs/op\n",
            size, total_time_us, avg_time_us);

        destroyStack(&s);
    }
}

// ОСНОВНАЯ ФУНКЦИЯ БЕНЧМАРКОВ

int main() {
    printf("========================================\n");
    printf("        STACK BENCHMARK SUITE\n");
    printf("========================================\n");

    printf("\nSystem information:\n");
    printf("Clock ticks per second: %ld\n", CLOCKS_PER_SEC);
    printf("sizeof(Node): %zu bytes\n", sizeof(Node));
    printf("sizeof(Stack): %zu bytes\n", sizeof(Stack));

    // Прогрев (warm-up) чтобы избежать аномалий кэша
    printf("\nWarming up...\n");
    Stack warmup;
    initStack(&warmup);
    for (int i = 0; i < 1000; i++) {
        push(&warmup, i);
        int val;
        pop(&warmup, &val);
    }
    destroyStack(&warmup);

    // Запуск бенчмарков
    printf("\nStarting benchmarks...\n");

    // 1. Одиночные операции
    benchmark_push_single();
    benchmark_pop_single();

    // 2. Множественные операции (разные размеры)
    benchmark_push_multiple(SMALL_SIZE);
    benchmark_pop_multiple(SMALL_SIZE);

    benchmark_push_multiple(MEDIUM_SIZE);
    benchmark_pop_multiple(MEDIUM_SIZE);

    benchmark_push_multiple(LARGE_SIZE);
    benchmark_pop_multiple(LARGE_SIZE);

    // 3. Смешанные операции
    benchmark_push_pop_alternating(SMALL_SIZE * 2);
    benchmark_push_pop_sequence(100, 100);  // 100 push/pop пар, повторенные 100 раз

    // 4. Граничные случаи
    benchmark_empty_stack_operations();
    benchmark_small_stacks();

    // 5. Большой бенчмарк (опционально - может занять время)
    printf("\n=== Optional: Large Scale Benchmark ===\n");
    printf("Run large benchmark? This may take a while... (y/n): ");

    char response = getchar();
    if (response == 'y' || response == 'Y') {
        benchmark_push_multiple(HUGE_SIZE);
        benchmark_pop_multiple(HUGE_SIZE);
    }

    printf("\n========================================\n");
    printf("        BENCHMARK COMPLETE\n");
    printf("========================================\n");

    return 0;
}
