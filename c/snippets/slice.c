// cling -Wno-writable-strings slice.c
// -*-c++-*-

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "utils.h"

char* slice(char *s, int begin, int end) {
  if (!s) return NULL;
  int len = strlen(s);
  if (begin < 0) begin = len + begin;
  if (begin < 0) begin = 0;
  if (end < 0) end = len + end;
  if (end >= len) end = len;

  if (begin >= end) begin = end = 0;

  int size = end - begin;
  //printf("`%s`, begin=%d, end=%d, size=%d, len=%d\n", s, begin, end, size, len);
  char *r = (char*)malloc(size + 1);
  memcpy(r, s+begin, size);
  r[size] = '\0';

  return r;
}

// q w e r t y
// 0 1 2 3 4 5

void slice() {
  test(slice(NULL, 1, 2) == NULL);
  test_streq(slice("", 2, 2), "");
  test_streq(slice("", 100, 2), "");
  test_streq(slice("", 1, 2), "");
  test_streq(slice("qwerty", 2, -1), "ert");
  test_streq(slice("qwerty", 0, -1), "qwert");
  test_streq(slice("qwerty", 0, 6), "qwerty");
  test_streq(slice("qwerty", 0, 100), "qwerty");
  test_streq(slice("qwerty", -6, 1), "q");
  test_streq(slice("qwerty", -6, 6), "qwerty");
  test_streq(slice("qwerty", -6, 7), "qwerty");
  test_streq(slice("qwerty", 0, 20), "qwerty");
  test_streq(slice("qwerty", 0, 2), "qw");
  test_streq(slice("qwerty", 10000, 10000), "");
  test_streq(slice("qwerty", 1, 1), "");
  test_streq(slice("qwerty", -1, -1), "");
  test_streq(slice("qwerty", 1, -1), "wert");
}
