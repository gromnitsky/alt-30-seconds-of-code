void list_print(char **list) {
  if (!list) { fprintf(stderr, "(null)\n"); return; }
  int idx = 0;
  for (char **p = list; *p; p++, idx++) {
    fprintf(stderr, "%s\"%s\"%s", idx > 0 ? ", " : "[", *p, *(p+1) == NULL ? "]\n" : "");
  }
  if (!idx) fprintf(stderr, "[]\n");
}
