void list_print(char **list) {
  if (!list) { fprintf(stderr, "(null)\n"); return; }
  int idx = 0;
  for (char **p = list; *p; p++, idx++) {
    fprintf(stderr, "%s\"%s\"%s", idx > 0 ? ", " : "[", *p, *(p+1) == NULL ? "]\n" : "");
  }
  if (!idx) fprintf(stderr, "[]\n");
}

int list_len(char **list) {
  if (!list) return 0;
  int len = 0;
  for (char **p = list; *p; p++) len++;
  return len;
}

void list_free(char ***v) {
  if ( !(v && *v)) return;
  for (char **p = *v; *p; p++) free(*p);
  free(*v);
  *v = NULL;
}

char** list_scp(char **list) {
  if (!list) return NULL;
  int len = list_len(list);
  char **r = (char**)malloc((len+1) * sizeof(char*));
  char **src = list, **dest = r;
  while (*src) *dest++ = *src++;
  r[len] = NULL;
  return r;
}

int list_cmp(const void *a, const void *b) {
  return strcmp(*(char**)a, *(char**)b);
}

char** list_sort(char **list) {
  if (!list) return NULL;
  int len = list_len(list);
  char **copy = list_scp(list);
  if (len < 2) return copy;

  qsort(copy, len, sizeof(char*), list_cmp);
  return copy;
}
