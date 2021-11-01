#include "utils.h"

char** simple_split(const char *separator, const char *str) {
  if ( !(str && separator)) return NULL;

  size_t r_len = 2, idx = 0;
  char **r = (char**)malloc(r_len * sizeof(char*));
  char *str_copy = strdup(str), *chunk, *_saveptr;
  char *str_copy_p = str_copy;

  while ( (chunk = strtok_r(str_copy_p, separator, &_saveptr))) {
    if (idx >= r_len-1) {
      r_len *= 2;
      r = (char**)realloc(r, r_len * sizeof(char*));
    }
    r[idx++] = strdup(chunk);
    str_copy_p = NULL;
  }
  r[idx] = NULL;

  free(str_copy);
  return r;
}

void simple_split() {
  test(NULL == simple_split(NULL, " "));

  typedef const char *list[];
  test_listeq(simple_split(" ", " abc def  ghi"), ((list){"abc", "def", "ghi", NULL}));
  test_listeq(simple_split("x", "abc def ghi"), ((list){"abc def ghi", NULL}));
}
