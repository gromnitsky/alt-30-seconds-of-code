#include "utils.h"

int nonspace(const char *s, const int step) {
  int pos = 0, start = step > 0 ? 0 : strlen(s)-1;
  for (int i=start; pos = i, isspace(s[i]); i+=step)
    ;
  return step > 0 ? pos : start - pos;
}

char* trim(const char *s) {
  if (!s) s = "";
  int start = nonspace(s, 1), size = strlen(s), len = 0;
  if (start != size) len = size - start - nonspace(s, -1);
  //printf("start=%d, len=%d\n", start, len);
  char *copy = (char*)malloc(len + 1);
  memcpy(copy, s+start, len);
  copy[len] = '\0';
  return copy;
}

void trim() {
  test_streq(trim(NULL), "");
  test_streq(trim(""), "");
  test_streq(trim("  qwerty   "), "qwerty");
}
