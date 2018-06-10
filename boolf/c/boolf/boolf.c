#include <string.h>

#include "stack.h"
#include "buffer.h"
#include "boolf.h"

#define BOOLF_MAINBUFF_SIZE 30000
#define BOOLF_OUTBUFF_SIZE 1024

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

char* boolf_interpret(const char* code, const char* input) {
  Buffer buff, out, in, data;

  buffer_init(&buff, BOOLF_MAINBUFF_SIZE, BUFF_BIT, BUFF_PTRPOS_MIDDLE);
  buffer_init(&out, BOOLF_OUTBUFF_SIZE, BUFF_BIT, BUFF_PTRPOS_BEGIN);
  buffer_sinit(&in, input, BUFF_BYTE, BUFF_PTRPOS_BEGIN);
  buffer_sinit(&data, code, BUFF_BYTE, BUFF_PTRPOS_BEGIN);

  Stack loopStack = STACK_INIT;

  int exit = 0;
  unsigned char inFlag = 255;
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
        *ptr = !*ptr;
      } break;
      case ';':
        *(buffer_getPtr(&out)) = *(buffer_getPtr(&buff));
        buffer_ptrShift(&out, 1);
        break;
      case ',':
        if (buffer_getSize(&in) > strlen(input) + 1) {
          exit = 1;
        } else {
          if (inFlag == 0) {
            buffer_ptrShift(&in, 1);
            inFlag = 255;
          }
          *(buffer_getPtr(&buff)) = *(buffer_getPtr(&in)) & 1;
          *(buffer_getPtr(&in)) >>= 1;
          inFlag >>= 1;
        }
        break;
      case '[':
        if (*(buffer_getPtr(&buff)) == 0) {
          if (buffer_setPtr(&data, getLoopBreak(dataPtr)) == NULL) exit = 1;
        } else stack_push(&loopStack, dataPtr);
        break;
      case ']':
        if (*(buffer_getPtr(&buff)) == 1)
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
