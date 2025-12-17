#include "stack.h"
#include <stdio.h>

int main() {
    Stack stack;
    initStack(&stack);

    push(&stack, 20);
    push(&stack, 10);
    push(&stack, 30);

    printf("After pushing elements:\n");
    traverseStack(&stack);

    int popped_value;
    if (pop(&stack, &popped_value)) {
        printf("Popped value: %d\n", popped_value);
    }

    printf("After popping an element:\n");
    traverseStack(&stack);

    Node* searchResult = searchByValue(&stack, 20);
    if (searchResult != NULL) {
        printf("Element with value 20 found.\n");
    }
    else {
        printf("Element with value 20 not found.\n");
    }

    Node* topElement = getTop(&stack);
    if (topElement != NULL) {
        printf("Top element: %d\n", topElement->data);
    }

    printf("Peek top element: %d\n", top(&stack));

    while (!isEmpty(&stack)) {
        pop(&stack, &popped_value);
        printf("Cleaning: popped %d\n", popped_value);
    }

    destroyStack(&stack);
    return 0;
}
