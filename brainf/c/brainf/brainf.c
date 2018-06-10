#include <string.h>

#include "stack.h"
#include "buffer.h"
#include "brainf.h"

#define BRAINF_MAINBUFF_SIZE 30000
#define BRAINF_OUTBUFF_SIZE 1024

static char* getLoopBreak(char* data) {
  char* loopBreak;
  char* tmp = data;

  for (size_t innerLoops = 1; innerLoops != 0; innerLoops--) {
    loopBreak = strchr(tmp + 1, ']');
    for (char* ptr = tmp + 1; ptr != loopBreak; ++ptr) {
      if (*ptr == '[') innerLoops++;
    }
    tmp = loopBreak;
  }

  return loopBreak;
}

char* brainf_interpret(const char* code, const char* input) {
  Buffer buff, out, in, data;

  buffer_init(&buff, BRAINF_MAINBUFF_SIZE, BUFF_BYTE, BUFF_PTRPOS_MIDDLE);
  buffer_init(&out, BRAINF_OUTBUFF_SIZE, BUFF_BYTE, BUFF_PTRPOS_BEGIN);
  buffer_sinit(&in, input, BUFF_BYTE, BUFF_PTRPOS_BEGIN);
  buffer_sinit(&data, code, BUFF_BYTE, BUFF_PTRPOS_BEGIN);

  Stack loopStack = STACK_INIT;

  int exit = 0;
  for (
    char* dataPtr = buffer_getPtr(&data);
    *dataPtr != '\0' && !exit;
    buffer_ptrShift(&data, 1), dataPtr = buffer_getPtr(&data)
  ) {
    switch (*dataPtr) {
      case '>':
        buffer_ptrShift(&buff, 1);
        break;
      case '<':
        buffer_ptrShift(&buff, -1);
        break;
      case '+': {
        char* ptr = buffer_getPtr(&buff);
        *ptr = (*ptr + 1) % 256;
      } break;
      case '-': {
        char* ptr = buffer_getPtr(&buff);
        if (*ptr == 0)
          *ptr += 255;
        else
          *ptr -= 1;
      } break;
      case '.':
        *(buffer_getPtr(&out)) = *(buffer_getPtr(&buff));
        buffer_ptrShift(&out, 1);
        break;
      case ',':
        if (buffer_getSize(&in) != strlen(input) + 1) exit = 1;
        else {
          *(buffer_getPtr(&buff)) = *(buffer_getPtr(&in));
          buffer_ptrShift(&in, 1);
        }
        break;
      case '[':
        if (*(buffer_getPtr(&buff)) == 0) {
          if (buffer_setPtr(&data, getLoopBreak(dataPtr)) == NULL) exit = 1;
        } else stack_push(&loopStack, dataPtr);
        break;
      case ']':
        if (*(buffer_getPtr(&buff)) != 0)
          buffer_setPtr(&data, (char*)stack_back(&loopStack));
        else
          stack_pop(&loopStack);
        break;
    }
  }

  char* result = buffer_toString(&out);

  buffer_free(&buff);
  buffer_free(&out);
  buffer_free(&in);
  buffer_free(&data);

  stack_free(&loopStack);

  return result;
}
