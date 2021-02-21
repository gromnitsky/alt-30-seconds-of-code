#include "split.c"

void readline() {
  char *line = NULL;
  size_t len = 0;               // unused in the example
  while (getdelim(&line, &len, '\n', stdin) != -1) {
    if (!strstr(line, "vmnet")) continue;

    char **cols = split("\\s+", line);
    printf("%s -> %s\n", cols[0], cols[8]);
    free(cols);
  }
  free(line);
}
