#include <stdio.h>
#include <stdlib.h>

#include "stack.h"

Node* createNode(int data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

void initStack(Stack* stack) {
    stack->top = NULL;
}

void destroyStack(Stack* stack) {
    Node* current = stack->top;
    while (current != NULL) {
        Node *tmp = current;
        current = current->next;
	    free(tmp);
    }
}

void push(Stack* stack, int data) {
    Node* newNode = createNode(data);
    newNode->next = stack->top;
    stack->top = newNode;
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
    return stack->top;
}

void traverseStack(Stack* stack) {
    Node* current = stack->top;
    printf("Stack elements: ");
    while (current != NULL) {
        printf("%d ", current->data);
        current = current->next;
    }
    printf("\n");
}

bool isEmpty(Stack* stack) {
    free(stack->top);
    return stack->top == NULL;
}

