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
