#ifndef _BUFFER_H_
#define _BUFFER_H_

#include <stdlib.h>

typedef enum {
  BUFF_PTRPOS_BEGIN,
  BUFF_PTRPOS_MIDDLE,
  BUFF_PTRPOS_END
} buffPtrPos_t;

typedef enum {
  BUFF_BYTE,
  BUFF_BIT
} buffType_t;

typedef struct {
  char*        __buff;
  size_t       __size;
  buffType_t   __type;
  char*        __ptr;
  long         __ptrPos;
  buffPtrPos_t __ptrType;
} Buffer;

void buffer_init(Buffer*, size_t, buffType_t, buffPtrPos_t);
void buffer_sinit(Buffer*, const char*, buffType_t, buffPtrPos_t);

char* buffer_ptrShift(Buffer*, int);
char* buffer_getPtr(Buffer*);
char* buffer_setPtr(Buffer*, char*);

long buffer_getPtrPos(Buffer*);
size_t buffer_getSize(Buffer*);

char* buffer_toString(Buffer*);

void buffer_free(Buffer*);

#endif
