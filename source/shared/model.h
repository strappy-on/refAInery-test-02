/*
 * FizzBuzz - public API.
 *
 * fizzbuzz_print(n) prints the FizzBuzz value for integer n:
 *   - "FizzBuzz" if n is divisible by both 3 and 5
 *   - "Fizz"     if n is divisible by 3
 *   - "Buzz"     if n is divisible by 5
 *   - the number itself otherwise
 *
 * fizzbuzz_count() returns the total number of lines printed for
 * 0 through 1000 inclusive (1001).
 */
#ifndef ALTIVEC_FIZZBUZZ_H
#define ALTIVEC_FIZZBUZZ_H

#ifdef __cplusplus
extern "C" {
#endif

void fizzbuzz_print(int n);
int fizzbuzz_count(void);

#ifdef __cplusplus
}
#endif
#endif
