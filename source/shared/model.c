#include "model.h"
#include <stdio.h>

void fizzbuzz_print(int n) {
  if (n % 15 == 0) {
    printf("FizzBuzz\n");
  } else if (n % 3 == 0) {
    printf("Fizz\n");
  } else if (n % 5 == 0) {
    printf("Buzz\n");
  } else {
    printf("%d\n", n);
  }
}

int fizzbuzz_count(void) { return 1001; }
