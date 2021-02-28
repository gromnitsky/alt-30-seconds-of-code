#ifndef STR_VECTOR_UTILS_C
#define STR_VECTOR_UTILS_C

#include "utils.h"

void list_print(char **list) {
  if (!list) { fprintf(stderr, "(null)\n"); return; }
  int idx = 0;
  for (char **p = list; *p; p++, idx++) {
    fprintf(stderr, "%s\"%s\"%s", idx > 0 ? ", " : "[", *p, *(p+1) == NULL ? "]\n" : "");
  }
  if (!idx) fprintf(stderr, "[]\n");
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
  while (*src) *dest++ = *src++;
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


void str_vector_utils() {
  typedef const char *list[];

  const char *orig1[] = { "foo", "bar", "baz", NULL };
  const char *orig2[] = { "foo", "bar", "baz", NULL };
  test_listeq(list_scp(orig1), orig1);
  test_listeq(orig1, orig2);

  const char *a1[] = {"bar", "eggplant", "gate", "car", "door", "apple", "fork", NULL};
  const char *a2[] = {"bar", "eggplant", "gate", "car", "door", "apple", "fork", NULL};
  char **sorted = list_sort(a1);
  test_listeq(a1, a2);
  test_listeq(sorted, ((list){"apple", "bar", "car", "door", "eggplant", "fork", "gate", NULL}));
}

#endif
