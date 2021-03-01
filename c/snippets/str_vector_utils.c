#ifndef STR_VECTOR_UTILS_C
#define STR_VECTOR_UTILS_C

#include "utils.h"

void list_print(char **list) {
  if (!list) { fprintf(stderr, "(null)\n"); return; }
  int idx = 0;
  for (char **p = list; *p; p++, idx++)
    fprintf(stderr, "%s\"%s\"", idx > 0 ? ", " : "[", *p);
  fprintf(stderr, "%s\n", idx ? "]" : "[]");
}

int list_len(const char **list) {
  if (!list) return 0;
  int len = 0;
  for (const char **p = list; *p; p++) len++;
  return len;
}

void list_free(char ***v) {
  if ( !(v && *v)) return;
  for (char **p = *v; *p; p++) free(*p);
  free(*v);
  *v = NULL;
}

char** list_scp(const char **list) {
  if (!list) return NULL;
  int len = list_len(list);
  char **r = (char**)malloc((len+1) * sizeof(char*));
  char **src = (char**)list, **dest = r;
  while (*src) *dest++ = *src++; // †
  r[len] = NULL;
  return r;
}

int list_cmp(const void *a, const void *b) {
  return strcmp(*(char**)a, *(char**)b);
}

char** list_sort(const char **list) {
  if (!list) return NULL;
  int len = list_len(list);
  char **copy = list_scp(list);
  if (len < 2) return copy;

  qsort(copy, len, sizeof(char*), list_cmp);
  return copy;
}

char** list_reverse(const char **list) {
  if (!list) return NULL;
  int len = list_len(list);
  char **r = (char**)malloc((len+1)*sizeof(char*));
  char **rp = r;

  for (list = &list[--len]; len >= 0; len--) *r++ = (char*)*list--; // †
  *r = NULL;
  return rp;
}


void str_vector_utils() {
  typedef const char *list[];

  const char *v[] = { "foo", "bar", "baz", NULL };
  const char *v_rev[] = { "baz", "bar", "foo", NULL };
  const char *v2[] = { "foo", "bar", "baz", NULL };
  test_listeq(list_scp(v), v);
  test_listeq(v, v2);

  test_listeq(list_reverse(v), v_rev);
  test_listeq(v, v2);

  const char *a1[] = {"bar", "eggplant", "gate", "car", "door", "apple", "fork", NULL};
  const char *a2[] = {"bar", "eggplant", "gate", "car", "door", "apple", "fork", NULL};
  char **sorted = list_sort(a1);
  test_listeq(a1, a2);
  test_listeq(sorted, ((list){"apple", "bar", "car", "door", "eggplant", "fork", "gate", NULL}));
}

#endif
