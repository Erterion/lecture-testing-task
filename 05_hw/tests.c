#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "stack.h"

// Вспомогательная функция для создания заполненного стека
static Stack* create_test_stack(int elements[], int count) {
    Stack* s = (Stack*)malloc(sizeof(Stack));
    initStack(s);
    for (int i = 0; i < count; i++) {
        assert(push(s, elements[i]));
    }
    return s;
}

// Вспомогательная функция для проверки стека
static void verify_stack(Stack* s, int expected[], int count, bool reverse) {
    Node* current = s->top;
    for (int i = 0; i < count; i++) {
        int idx = reverse ? count - i - 1 : i;
        assert(current != NULL);
        assert(current->data == expected[idx]);
        current = current->next;
    }
    assert(current == NULL);
}

void test_init_and_destroy() {
    printf("Test: init_and_destroy... ");

    Stack s;
    initStack(&s);
    assert(isEmpty(&s));
    assert(top(&s) == INT_MIN);

    // Попытка извлечь из пустого стека
    int val;
    assert(!pop(&s, &val));

    // Поиск в пустом стеке
    assert(searchByValue(&s, 42) == NULL);
    assert(searchByIndex(&s, 0) == NULL);

    destroyStack(&s);

    // Работа с NULL
    initStack(NULL);  // Не должно падать
    destroyStack(NULL);

    printf("OK\n");
}

void test_push_pop() {
    printf("Test: push_pop... ");

    Stack s;
    initStack(&s);

    // Push элементов
    assert(push(&s, 10));
    assert(!isEmpty(&s));
    assert(top(&s) == 10);

    assert(push(&s, 20));
    assert(top(&s) == 20);

    assert(push(&s, 30));
    assert(top(&s) == 30);

    // Проверяем порядок (LIFO)
    int expected[] = { 30, 20, 10 };
    verify_stack(&s, expected, 3, false);

    // Pop элементов
    int val;
    assert(pop(&s, &val) && val == 30);
    assert(top(&s) == 20);

    assert(pop(&s, &val) && val == 20);
    assert(top(&s) == 10);  // Исправлено: было "petopek"

    assert(pop(&s, &val) && val == 10);
    assert(isEmpty(&s));
    assert(top(&s) == INT_MIN);

    // Дополнительные pop из пустого стека
    assert(!pop(&s, &val));

    destroyStack(&s);
    printf("OK\n");
}

void test_search_functions() {
    printf("Test: search_functions... ");

    int elements[] = { 5, 10, 15, 20, 25 };
    Stack* s = create_test_stack(elements, 5);

    // searchByValue - существующие элементы
    Node* node = searchByValue(s, 15);
    assert(node != NULL);
    assert(node->data == 15);

    node = searchByValue(s, 5);  // Последний элемент
    assert(node != NULL);
    assert(node->data == 5);

    node = searchByValue(s, 25);  // Первый элемент
    assert(node != NULL);
    assert(node->data == 25);

    // searchByValue - несуществующий элемент
    assert(searchByValue(s, 99) == NULL);

    // searchByIndex
    node = searchByIndex(s, 0);
    assert(node != NULL && node->data == 25);  // Вершина

    node = searchByIndex(s, 2);
    assert(node != NULL && node->data == 15);

    node = searchByIndex(s, 4);
    assert(node != NULL && node->data == 5);  // Дно стека

    // Граничные индексы
    assert(searchByIndex(s, -1) == NULL);
    assert(searchByIndex(s, 5) == NULL);  // За пределами
    assert(searchByIndex(s, 100) == NULL);

    // getTop
    node = getTop(s);
    assert(node != NULL && node->data == 25);

    // Проверка с NULL
    assert(searchByValue(NULL, 10) == NULL);
    assert(searchByIndex(NULL, 0) == NULL);
    assert(getTop(NULL) == NULL);

    destroyStack(s);
    free(s);
    printf("OK\n");
}

void test_edge_cases() {
    printf("Test: edge_cases... ");

    Stack s;
    initStack(&s);

    // 1. Много элементов
    for (int i = 0; i < 1000; i++) {
        assert(push(&s, i));
    }
    assert(!isEmpty(&s));
    assert(top(&s) == 999);

    // 2. Проверка после многих операций
    int val;
    for (int i = 999; i >= 0; i--) {
        assert(pop(&s, &val) && val == i);
    }
    assert(isEmpty(&s));

    // 3. Push после полной очистки
    assert(push(&s, 42));
    assert(top(&s) == 42);
    assert(pop(&s, &val) && val == 42);
    assert(isEmpty(&s));

    // 4. Передача NULL в pop
    assert(!pop(&s, NULL));  // value = NULL
    assert(!pop(NULL, &val)); // stack = NULL

    // 5. Проверка top с NULL
    assert(top(NULL) == INT_MIN);

    destroyStack(&s);
    printf("OK\n");
}

void test_memory_management() {
    printf("Test: memory_management... ");

    // Тест на утечки памяти (нужно запускать с санитайзером)
    Stack s;
    initStack(&s);

    // Добавляем и удаляем элементы
    for (int i = 0; i < 100; i++) {
        assert(push(&s, i));
    }

    int val;
    for (int i = 0; i < 50; i++) {
        assert(pop(&s, &val));
    }

    // Добавляем еще
    for (int i = 0; i < 50; i++) {
        assert(push(&s, i + 100));
    }

    // Очищаем все
    while (!isEmpty(&s)) {
        assert(pop(&s, &val));
    }

    assert(isEmpty(&s));
    destroyStack(&s);

    printf("OK\n");
}

void test_isEmpty() {
    printf("Test: isEmpty... ");

    Stack s;
    initStack(&s);

    assert(isEmpty(&s));
    assert(isEmpty(NULL));  // NULL считается пустым

    push(&s, 1);
    assert(!isEmpty(&s));

    int val;
    pop(&s, &val);
    assert(isEmpty(&s));

    destroyStack(&s);
    printf("OK\n");
}

int main() {
    printf("=== Running Stack Tests ===\n\n");

    test_init_and_destroy();
    test_push_pop();
    test_search_functions();
    test_edge_cases();
    test_memory_management();
    test_isEmpty();

    printf("\n=== All Tests Passed! ===\n");
    return 0;
}
