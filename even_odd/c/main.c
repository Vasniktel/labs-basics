#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

int *split_odd_and_even(int value, size_t *pcount);
// Please make sure you report the number of groups
// in your array via *pcount.

int main(void) {
  size_t size;
  int* arr = split_odd_and_even(88135426, &size);
  for (int i = 0; i < size; i++) {
    printf("%d ", arr[i]);
  }
  printf("\n%zu\n", size);
  free(arr);
  return 0;
}

int* split_odd_and_even(int value, size_t* pcount) {
  int numSize = log10(value) + 1;
  int arr[numSize];
  size_t pos = 0;
  arr[0] = 0;
  int even = (int)(value / pow(10, numSize - 1)) % 2 == 0;

  for (int i = 0; i < numSize; i++) {
    int num = (int)(value / pow(10, numSize - i - 1)) % 10;
    if (num % 2 == 0 && !even) {
      even = 1;
      pos++;
      arr[pos] = 0;
    } else if (num % 2 != 0 && even) {
      even = 0;
      pos++;
      arr[pos] = 0;
    }
    arr[pos] = arr[pos] * 10 + num;
  }

  int* result = (int*)malloc((pos + 1) * sizeof(int));
  memcpy(result, arr, (pos + 1) * sizeof(int));
  *pcount = pos + 1;
  return result;
}
