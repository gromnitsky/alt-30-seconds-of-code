#include "utils.h"

typedef struct {
  int begin;
  int size;
} _SliceRange;

_SliceRange _slice_calc(int len, int begin, int end) {
  _SliceRange r = { .begin = begin };
  if (len < 0) len = 0;

  if (r.begin < 0) r.begin = len + r.begin;
  if (r.begin < 0) r.begin = 0;
  if (end < 0) end = len + end;
  if (end >= len) end = len;

  if (r.begin >= end) r.begin = end = 0;

  r.size = end - r.begin;
  return r;
}

char* slice(const char *s, int begin, int end) {
  if (!s) return NULL;
  int len = strlen(s);

  _SliceRange range = _slice_calc(len, begin, end);

  char *r = (char*)malloc(range.size + 1);
  memcpy(r, s+range.begin, range.size);
  r[range.size] = '\0';

  return r;
}

char** list_slice(const char **list, int begin, int end) {
  if (!list) return NULL;
  int len = list_len(list);

  _SliceRange range = _slice_calc(len, begin, end);

  char **r = (char**)malloc((range.size + 1)*sizeof(char*));
  const char **src = list+range.begin;
  for (int idx = 0; idx < range.size; idx++) {
    r[idx] = strdup(*src++);
  }
  r[range.size] = NULL;

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

  typedef const char *list[];

  const char *v[] = {"q", "w", "e", "r", "t", "y", NULL};
  test(list_slice(NULL, 1, 2) == NULL);
  test_listeq(list_slice(v, 2, -1), ((list){"e", "r", "t", NULL}) );
  test_listeq(list_slice(v, 10000, 10000), ((list){NULL}) );
  test_listeq(list_slice(v, 1, -1), ((list){"w", "e", "r", "t", NULL}) );
}
