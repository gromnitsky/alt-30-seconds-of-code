#include "utils.h"

char* join(const char **list, const char *separator) {
  if (!list) return NULL;

  int len = 0, ilen = 0;
  for (const char **p = list; *p; p++) {
    len++;                      // number of items in the list
    ilen += strlen(*p);         // sum of items length
  }

  //  printf("len=%d, ilen=%d\n", len, ilen);
  if (len < 1) return NULL;
  char *r = (char*)malloc(ilen + (len - 1)*strlen(separator) + 1);
  r[0] = '\0';

  for (int i = 0; i < len; ++i) {
    if (i != 0) strcat(r, separator);
    strcat(r, list[i]);
  }

  return r;
}

void join() {
  test(NULL == join(NULL, " "));

  typedef const char *list[];
  test(NULL == join((list){NULL}, " "));
  test_streq(join((list){"foo", NULL}, " "), "foo");
  test_streq(join((list){"foo", "", NULL}, " "), "foo ");
  test_streq(join((list){ "An", "old", "maid", "of", "five", "&", "thirty", "eloped", "with", "a", "colonel", "of", "40", NULL}, " "), "An old maid of five & thirty eloped with a colonel of 40");
}
