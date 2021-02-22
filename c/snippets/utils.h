#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include <libgen.h>

#include "str_vector_utils.c"

void __my_assert(const char *assertion, const char *file,
                 unsigned int line, const char *function) {
  fprintf(stderr, "%s:%d: %s: Assertion `%s' failed.\n",
          file, line, function, assertion);
  exit(1);
}

void __my_test(bool expr, const char *assertion, char *file,
               unsigned int line, const char *function) {
  bool ok = expr;
  fprintf(stderr, "%s:%d: `%s`: %s\n",
          basename(file), line, assertion,
          (ok ? "ok" : "FAILED"));
  if (!ok) exit(1);
}


char* ansi_color(char *to, int color, const char* msg) {
  snprintf(to, 1024, "\033[%dm%s\033[m", color, msg);
  return to;
}

void __my_test_streq(char *actual, char *expected,
                      const char *assertion, char *file,
                      unsigned int line, const char *function) {
  bool ok = (0 == strcmp(actual, expected));
  fprintf(stderr, "%s:%d: %s: ", basename(file), line, assertion);
  if (ok) {
    fprintf(stderr, "ok\n");
    return;
  }

  char red[1024];
  ansi_color(red, 31, "FAILED");
  fprintf(stderr, "%s:\ngot `%s`\nexp `%s`\n", red, actual, expected);
  exit(1);
}

void __my_test_listeq(char **actual, char **expected,
                      const char *assertion, char *file,
                      unsigned int line, const char *function) {
  bool ok = true;
  if (list_len(actual) != list_len(expected)) {
    ok = false;
  } else {
    char **pa = actual, **pe = expected;
    for (; *pa; pa++, pe++) {
      if (0 != strcmp(*pa, *pe)) {
        ok = false;
        break;
      }
    }
  }

  fprintf(stderr, "%s:%d: %s: ", basename(file), line, assertion);
  if (ok) { fprintf(stderr, "ok\n"); return; }

  char red[1024];
  ansi_color(red, 31, "FAILED");
  fprintf(stderr, "%s:\ngot ", red);
  list_print(actual);
  fprintf(stderr, "exp ");
  list_print(expected);
  exit(1);
}

#define assert(expr) \
 ((expr) ? (void)0 : __my_assert(#expr, __FILE__, __LINE__, __func__))

#define test(expr) \
 (__my_test((expr), #expr, __FILE__, __LINE__, __func__))

#define test_streq(actual, expected) \
 (__my_test_streq((actual), (expected), #actual, __FILE__, __LINE__, __func__))

#define test_listeq(actual, expected) \
 (__my_test_listeq((actual), (expected), #actual, __FILE__, __LINE__, __func__))

#endif
