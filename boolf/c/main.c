#include <stdlib.h>
#include <stdio.h>
#include "boolf.h"

char* boolfuck(char*, char*);

int main(void) {
  char* result = boolfuck(";;;+;+;;+;+;\
  +;+;+;+;;+;;+;\
  ;;+;;+;+;;+;\
  ;;+;;+;+;;+;\
  +;;;;+;+;;+;\
  ;;+;;+;+;+;;\
  ;;;;;+;+;;\
  +;;;+;+;;;+;\
  +;;;;+;+;;+;\
  ;+;+;;+;;;+;\
  ;;+;;+;+;;+;\
  ;;+;+;;+;;+;\
  +;+;;;;+;+;;\
  ;+;+;+;", "");
  printf("%s", result);
  free(result);
  return 0;
}

char* boolfuck(char* code, char* in) {
  return boolf_interpret(code, in);
}
