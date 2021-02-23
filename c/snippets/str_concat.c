#include "join.c"
#include "utils.h"
#include <stdarg.h>

int args_len(va_list ap) {
  if (!ap) return 0;
  int len = 0;
  for (char *args = NULL; (args = va_arg(ap, char*)); len++)
    ;
  va_end(ap);
  return len;
}

char* str_concat(const char *args, ...) {
  va_list ap;
  va_start(ap, args);
  int len = args_len(ap); if (!len) return NULL;
  char **list = (char**)malloc((len + 1) * sizeof(char*));

  va_start(ap, args);
  for (int idx = 0; (args = va_arg(ap, char*)); idx++) {
    list[idx] = (char*)args;
    //printf("%d, %d: %s\n", len, idx, args);
  }
  list[len] = NULL;
  va_end(ap);

  char *r = join((const char**)list, "");
  free(list);
  return r;
}

#define strcat2(...) str_concat("va anchor", __VA_ARGS__, NULL)

void str_concat() {
  test(NULL == strcat2(NULL));
  test_streq(strcat2("foo", "bar", "baz"), "foobarbaz");
  test_streq(strcat2("foo", NULL, "baz"), "foo");
  //test_streq(strcat2("foo", 123, "baz"), "foo"); // segfaults
  test_streq(strcat2("foo"), "foo");
}
