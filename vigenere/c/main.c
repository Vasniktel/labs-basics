#include <stdio.h>
#include <string.h>

#define LEN ('z' - 'a' + 1)

char table[LEN][LEN + 1];

void encode(const char*, const char*, char*);
void decode(const char*, const char*, char*);

int main(void) {
  char msg[80], key[80];

  printf("Enter message and key respectively:\n");
  scanf("%79s %79s", msg, key);

  for (int i = 0; i < LEN; i++) {
    for (int j = 0; j < LEN; j++) {
      table[i][j] = 'a' + (j + i) % LEN;
    }
    table[i][LEN] = '\0';
  }

  char encoded[strlen(msg) + 1];
  encode(msg, key, encoded);
  printf("Encoded: %s\n", encoded);

  char decoded[strlen(msg) + 1];
  decode(encoded, key, decoded);
  printf("Decoded: %s\n", decoded);

  printf("Equal: %s\n", !strcmp(msg, decoded) ? "true" : "false");

  return 0;
}

void encode(const char* msg, const char* key, char* result) {
  const size_t msgLen = strlen(msg);
  const size_t keyLen = strlen(key);

  for (int i = 0; i < msgLen; i++) {
    result[i] = table[msg[i] - 'a'][key[i % keyLen] - 'a'];
  }
  result[msgLen] = '\0';
}

void decode(const char* msg, const char* key, char* result) {
  const size_t msgLen = strlen(msg);
  const size_t keyLen = strlen(key);

  for (int i = 0; i < msgLen; i++) {
    char* str = table[key[i % keyLen] - 'a'];
    result[i] = 'a' + (strchr(str, msg[i]) - str);
  }
  result[msgLen] = '\0';
}
