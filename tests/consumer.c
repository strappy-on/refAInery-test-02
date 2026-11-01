#include <model.h>
#include <stdio.h>

int main(void) {
  /* External library consumer: verify the archive's API is reachable and
   * produces the expected output. */
  if (fizzbuzz_count() != 1001) {
    fprintf(stderr, "FAIL: fizzbuzz_count() = %d, expected 1001\n",
            fizzbuzz_count());
    return 1;
  }
  fizzbuzz_print(15);
  fizzbuzz_print(1);
  fizzbuzz_print(3);
  fizzbuzz_print(5);
  return 0;
}
