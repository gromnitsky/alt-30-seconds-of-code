#include "utils.h"

char* str_tr(const char *s, int (*fn)(int)) {
  assert(fn);
  if (!s) return NULL;

  char *r = (char*)malloc(strlen(s)+1);
  char *copy = r;
  for (const char *p = s; *p; ++p) *copy++ = fn(*p);
  *copy = '\0';
  return r;
}

char* str_tr2(char *s, int (*fn)(int)) {
  assert(fn);
  if (!s) return NULL;
  for (char *p = s; *p; ++p) *p = fn(*p);
  return s;
}

void str_tr() {
  char s[] = "qwe";
  str_tr2(s, toupper);
  test_streq(s, "QWE");
  str_tr2(s, tolower);
  test_streq(s, "qwe");

  test_streq(str_tr("", toupper), "");
  test_streq(str_tr("qwerty", toupper), "QWERTY");
}
