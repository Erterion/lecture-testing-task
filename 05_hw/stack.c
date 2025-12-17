#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "stack.h"

Node* createNode(int data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (!newNode) {
        fprintf(stderr, "Ошибка выделения памяти\n");
        return NULL;
    }
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

void initStack(Stack* stack) {
    if (!stack) return;
    stack->top = NULL;
}

void destroyStack(Stack* stack) {
    if (!stack) return;

    Node* current = stack->top;
    while (current != NULL) {
        Node* tmp = current;
        current = current->next;
        free(tmp);
    }
    stack->top = NULL;
}

bool push(Stack* stack, int data) {
    if (!stack) return false;

    Node* newNode = createNode(data);
    if (!newNode) return false;

    newNode->next = stack->top;
    stack->top = newNode;
    return true;
}

bool pop(Stack* stack, int* value) {
    if (!stack || !stack->top || !value) {
        return false;
    }

    Node* temp = stack->top;
    *value = temp->data;
    stack->top = temp->next;
    free(temp);

    return true;
}

Node* searchByValue(Stack* stack, int value) {
    if (!stack) return NULL;

    Node* current = stack->top;
    while (current != NULL) {
        if (current->data == value) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

Node* searchByIndex(Stack* stack, int index) {
    if (!stack || index < 0) return NULL;

    Node* current = stack->top;
    int count = 0;

    while (current != NULL) {
        if (count == index) {
            return current;
        }
        count++;
        current = current->next;
    }
    return NULL;
}

Node* getTop(Stack* stack) {
    return stack ? stack->top : NULL;
}

void traverseStack(Stack* stack) {
    if (!stack) {
        printf("Stack is NULL\n");
        return;
    }

    Node* current = stack->top;
    printf("Stack elements: ");

    while (current != NULL) {
        printf("%d ", current->data);
        current = current->next;
    }

    printf("\n");
}

bool isEmpty(Stack* stack) {
    return !stack || stack->top == NULL;
}

int top(Stack* stack) {
    return (stack && stack->top) ? stack->top->data : INT_MIN;
}
