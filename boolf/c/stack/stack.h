#ifndef _STACK_H_
#define _STACK_H_

#include <stdlib.h>

#define STACK_INIT ((Stack) { NULL, NULL, 0, 0 })

typedef struct {
  void** __buff;
  void** __top;
  size_t __buffsize;
  size_t __stacksize;
} Stack;

void  stack_push(Stack*, void*);
void* stack_pop(Stack*);
void* stack_back(Stack*);
void  stack_free(Stack*);

#endif
