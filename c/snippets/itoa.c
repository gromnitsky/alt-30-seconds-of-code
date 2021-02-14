#include <math.h>
#include "utils.h"

char *itoa(int n) {
  int len = n == 0 ? 1 : floor(log10l(labs(n)))+1;
  if (n < 0) len++;
  char *s = (char*)malloc(len+1);
  snprintf(s, len+1, "%d", n);
  return s;
}

void itoa() {
  test_streq(itoa(0), "0");
  test_streq(itoa(-10), "-10");
  test_streq(itoa(100000), "100000");
}
