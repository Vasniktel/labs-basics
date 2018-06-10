#include <stdlib.h>
#include <string.h>
#include "stack.h"

static void stack_realloc(Stack* stack, size_t size) {
  size_t toRealloc = stack->__buffsize + size;
  void** ptr = (void**)malloc(toRealloc * sizeof(void*));
  if (stack->__buffsize != 0) {
    memcpy(ptr, stack->__buff, stack->__buffsize * sizeof(void*));
    free(stack->__buff);
  }
  stack->__buff = ptr;
  if (stack->__stacksize != 0) {
    stack->__top = stack->__buff + stack->__stacksize - 1;
  }
  stack->__buffsize = toRealloc;
}

void stack_push(Stack* stack, void* elem) {
  if (stack->__stacksize == stack->__buffsize) {
    stack_realloc(stack, stack->__buffsize / 2 + 1);
  }
  if (stack->__stacksize == 0) stack->__top = stack->__buff;
  else stack->__top += 1;
  *stack->__top = elem;
  stack->__stacksize += 1;
}

void* stack_pop(Stack* stack) {
  if (stack->__top == NULL) return NULL;
  void* elem = *stack->__top;
  stack->__stacksize -= 1;
  if (stack->__stacksize == 0) stack->__top = NULL;
  else stack->__top -= 1;
  return elem;
}

void* stack_back(Stack* stack) {
  if (stack->__top == NULL) return NULL;
  return *stack->__top;
}

void stack_free(Stack* stack) {
  free(stack->__buff);
  stack->__buff = stack->__top = NULL;
  stack->__stacksize = stack->__buffsize = 0;
}
