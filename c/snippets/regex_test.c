#include <regex.h>
#include "utils.h"

bool regex_test(const char *pattern, const char* s) {
  if ( !(pattern && s)) return false;
  int status;
  regex_t re;
  if (regcomp(&re, pattern, REG_EXTENDED|REG_NOSUB) != 0) return false;
  status = regexec(&re, s, 0, NULL, 0);
  regfree(&re);
  return !status;
}

void regex_test() {
  test(!regex_test(NULL, NULL));
  test(regex_test("^", ""));
  test(regex_test("o+", "foo"));
  test(regex_test("\\w+", "foo"));
  test(!regex_test("z", "foo"));
}
