#include <stdlib.h>
#include <string.h>
#include "buffer.h"

void buffer_init(Buffer* buff, size_t num, buffType_t type, buffPtrPos_t ptrType) {
  if (num == 0) {
    buff->__buff = buff->__ptr = NULL;
    buff->__size = 0;
    buff->__ptrPos = -1;
  } else {
    switch (type) {
      case BUFF_BIT:
      case BUFF_BYTE:
        buff->__buff = (char*)calloc(num, sizeof(char));
        break;
    }
    buff->__size = num;
    switch (ptrType) {
      case BUFF_PTRPOS_BEGIN:
        buff->__ptrPos = 0;
        break;
      case BUFF_PTRPOS_MIDDLE:
        buff->__ptrPos = buff->__size / 2;
        break;
      case BUFF_PTRPOS_END:
        buff->__ptrPos = buff->__size - 1;
        break;
    }
    buff->__ptr = buff->__buff + buff->__ptrPos;
  }
  buff->__type = type;
  buff->__ptrType = ptrType;
}

void buffer_sinit(Buffer* buff, const char* str, buffType_t type, buffPtrPos_t ptrType) {
  if (str == NULL) return;
  buffer_init(buff, strlen(str) + 1, type, ptrType);
  strcpy(buff->__buff, str);
}

void buffer_free(Buffer* buff) {
  free(buff->__buff);
  buff->__buff = buff->__ptr = NULL;
  buff->__size = 0;
  buff->__ptrPos = -1;
}

static void buffer_realloc(Buffer* buff, size_t expand) {
  if (expand == 0) return;
  size_t toRealloc = buff->__size + expand;
  char* ptr = NULL;

  switch (buff->__type) {
    case BUFF_BIT:
    case BUFF_BYTE:
      ptr = (char*)calloc(toRealloc, sizeof(char));
      break;
  }

  size_t start = 0;
  switch (buff->__ptrType) {
    case BUFF_PTRPOS_BEGIN:
      start = 0;
      break;
    case BUFF_PTRPOS_MIDDLE:
      start = expand / 2;
      break;
    case BUFF_PTRPOS_END:
      start = expand;
      break;
  }

  if (buff->__buff != NULL) {
    memcpy(ptr + start, buff->__buff, buff->__size * sizeof(char));
    free(buff->__buff);
  }

  buff->__buff = ptr;
  buff->__size = toRealloc;

  if (toRealloc == 1) {
    buff->__ptr = buff->__buff;
    buff->__ptrPos = 0;
  } else {
    buff->__ptr = buff->__buff + start + buff->__ptrPos;
    buff->__ptrPos += start;
  }
}

char* buffer_ptrShift(Buffer* buff, int shift) {
  if (buff->__size == 0) return NULL;
  while (buff->__ptrPos + shift > buff->__size - 1 || buff->__ptrPos + shift < 0) {
    buffer_realloc(buff, buff->__size / 2 + 1);
  }
  buff->__ptr += shift;
  buff->__ptrPos += shift;
  return buff->__ptr;
}

char* buffer_getPtr(Buffer* buff) {
  return buff->__ptr;
}

// ptr must be reachable
char* buffer_setPtr(Buffer* buff, char* ptr) {
  if (buff->__size == 0 || ptr == NULL) return NULL;
  if (ptr > buff->__buff + buff->__size - 1 || ptr < buff->__buff)
    return NULL;
  buff->__ptr = ptr;
  buff->__ptrPos = buff->__ptr - buff->__buff;
  return ptr;
}

long buffer_getPtrPos(Buffer* buff) {
  return buff->__ptrPos;
}

size_t buffer_getSize(Buffer* buff) {
  return buff->__size;
}

char* buffer_toString(Buffer* buff) {
  if (buff->__size == 0) return NULL;
  char* str = NULL;
  switch (buff->__type) {
    case BUFF_BYTE:
      str = (char*)malloc((buff->__size + 1) * sizeof(char));
      memcpy(str, buff->__buff, buff->__size * sizeof(char));
      str[buff->__size] = '\0';
      break;
    case BUFF_BIT: { // little endian order
      size_t full = buff->__size / 8;
      size_t numOfZeroes = (8 - (buff->__size - full * 8)) % 8;
      size_t all = numOfZeroes != 0 ? full + 1 : full;

      str = (char*)calloc(all + 1, sizeof(char));
      for (int i = 0, flag = 255, j = 0; i < buff->__size; i++, flag >>= 1) {
        if (flag == 0) {
          j++;
          flag = 255;
        }
        str[j] = ((unsigned char)str[j] >> 1) | (buff->__buff[i] ? 128 : 0);
      }
    } break;
  }
  return str;
}
