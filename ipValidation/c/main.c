#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>

int validateNumber(const char* num, size_t size) {
  if (!(size >= 1 && size <= 3)) return 0;
  for (int i = 0; i < size; i++) {
    if (num[i] == '0') {
      if (!i && size != 1) return 0;
    } else {
      if (!isdigit(num[i])) return 0;
    }
  }
  int res = atoi(num);
  return res <= 255 && res >= 0 ? 1 : 0;
}

/* Return 1 is addr is a valid IP address, return 0 otherwise */
int is_valid_ip(const char * addr) {
  char* ip = addr;
  char* end = strchr(ip, '.');
  int count = 0;
  while (ip != NULL) {
    if (!validateNumber(ip, end != NULL ? end - ip : strlen(ip))) return 0;
    count++;
    if (end == NULL) break;
    ip = end + 1;
    end = strchr(ip, '.');
  }
  return count == 4 ? 1 : 0;
}

int main(void) {
  printf("%d\n", is_valid_ip("165.165.217.109"));
  return 0;
}
