#include "utils.h"

char* shellescape(char *s) {
  if (!s) s = "";
  char *r = (char*)malloc(strlen(s)*4 + 2 + 1);
  char *rp = r;
  const char quote = '\'';
  r[0] = quote;
  rp++;

  int len = 0;
  for (char *p = s; *p; p++, len++) {
    if (*p == quote) {
      memcpy(rp, "'\\''", 4);
      len += 4;
      rp += 4;
    } else {
      *rp++ = *p;
    }
  }
  *rp++ = quote; len++;
  *rp = '\0';

  r = (char*)realloc(r, len + 1); assert(r);
  return r;
}

void shellescape() {
  test_streq(shellescape("foobar"), "'foobar'");
  test_streq(shellescape("foo'bar"), "'foo'\\''bar'");
  test_streq(shellescape("'''''''''''''''''''''''"), "''\\'''\\'''\\'''\\'''\\'''\\'''\\'''\\'''\\'''\\'''\\'''\\'''\\'''\\'''\\'''\\'''\\'''\\'''\\'''\\'''\\'''\\'''\\'''");
  test_streq(shellescape("'"), "''\\'''");
  test_streq(shellescape(""), "''");
  test_streq(shellescape(NULL), "''");
}
