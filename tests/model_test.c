#include "model.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/*
 * FizzBuzz unit tests.
 *
 * These tests exercise the real fizzbuzz_print() implementation. Because it
 * writes to stdout, each capture redirects stdout to a temporary file with
 * freopen(), then reads the file back and compares the exact expected text.
 */
#define MAX_LINE 16

static void check(int cond, const char *what) {
  if (!cond) {
    fprintf(stderr, "FAIL: %s\n", what);
    exit(1);
  }
}

/*
 * Redirects stdout to `path`, runs fizzbuzz_print() for each value, and reads
 * the captured output into `buf` (NUL-terminated). Returns on success.
 */
static void capture_prints(const int *values, int count, const char *path,
                           char *buf, size_t buf_size) {
  FILE *f;
  int i;

  fflush(stdout);
  check(freopen(path, "w", stdout) != NULL, "freopen(stdout) failed");
  for (i = 0; i < count; i++)
    fizzbuzz_print(values[i]);
  fflush(stdout);
  check(fclose(stdout) == 0, "fclose(stdout) failed");
  check((f = fopen(path, "r")) != NULL, "cannot reopen capture");
  if (fread(buf, 1, buf_size - 1, f) > buf_size - 1)
    buf[buf_size - 1] = '\0';
  buf[buf_size - 1] = '\0';
  check(fclose(f) == 0, "fclose(capture) failed");
}

/* Compares the captured text against expected lines given as a NUL-separated
 * concatenated string (each expected line already checked separately). */
static void expect_lines(const char *text, const char *const *expected,
                         int count, const char *context) {
  int i;
  const char *p = text;
  for (i = 0; i < count; i++) {
    size_t len = strlen(expected[i]);
    if (strncmp(p, expected[i], len) != 0 || p[len] != '\n') {
      fprintf(stderr, "FAIL: %s: line %d does not match \"%s\"\n", context,
              i + 1, expected[i]);
      exit(1);
    }
    p += len + 1;
  }
  check(*p == '\0', "unexpected trailing bytes in capture");
}

/* Builds the expected FizzBuzz string for n into a caller buffer. */
static const char *expected_for(int n, char *buf, size_t buf_size) {
  if (n % 15 == 0)
    return "FizzBuzz";
  if (n % 3 == 0)
    return "Fizz";
  if (n % 5 == 0)
    return "Buzz";
  snprintf(buf, buf_size, "%d", n);
  return buf;
}

int main(void) {
  const int values[8] = {0, 1, 3, 5, 15, 990, 999, 1000};
  int all[1001];
  char buf[16 * 1001 + 64];
  char path[32];
  char num[MAX_LINE];
  int n;

  /* Count API: 0 through 1000 inclusive is 1001 lines. */
  check(fizzbuzz_count() == 1001, "fizzbuzz_count() must be 1001");

  snprintf(path, sizeof(path), "/tmp/fizzbuzz-test-XXXXXX");
  check(mkstemp(path) != -1, "mkstemp failed");

  /*
   * Boundary cases through the real function: 0 is FizzBuzz, 1 is a number,
   * 3 is Fizz, 5 is Buzz, 15 is FizzBuzz, 990 is FizzBuzz, 999 is Fizz and
   * 1000 is Buzz.
   */
  capture_prints(values, 8, path, buf, sizeof(buf));
  {
    const char *expected[8] = {"FizzBuzz", "1", "Fizz", "Buzz", "FizzBuzz",
                               "FizzBuzz", "Fizz", "Buzz"};
    expect_lines(buf, expected, 8, "boundary");
  }

  /*
   * Full range: run the exact loop the CLI uses and compare every one of the
   * 1001 lines against an independently computed expectation.
   */
  for (n = 0; n <= 1000; n++)
    all[n] = n;
  capture_prints(all, 1001, path, buf, sizeof(buf));
  {
    const char *p = buf;
    for (n = 0; n <= 1000; n++) {
      const char *exp = expected_for(n, num, sizeof(num));
      size_t len = strlen(exp);
      if (strncmp(p, exp, len) != 0 || p[len] != '\n') {
        fprintf(stderr, "FAIL: range: n=%d does not match \"%s\"\n", n, exp);
        exit(1);
      }
      p += len + 1;
    }
    check(*p == '\0', "range capture must be exactly 1001 lines, nothing more");
  }

  check(unlink(path) == 0, "unlink(path) failed");
  printf("PASS: all FizzBuzz tests passed\n");
  return 0;
}
