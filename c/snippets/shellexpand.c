#include "utils.h"
#include <wordexp.h>

char** shellexpand(const char *s) {
  if (!s) return NULL;
  wordexp_t r;
  int status;
  if (0 != (status = wordexp(s, &r, WRDE_SHOWERR))) {
    if (status == WRDE_NOSPACE) wordfree(&r);
    return NULL;
  }
  return r.we_wordv;
}

void shellexpand() {
  typedef const char *list[];

  test(shellexpand(NULL) == NULL);
  test_listeq(shellexpand(""), ((list){NULL}));

  char **r = shellexpand("echo f\\ o\\ o `uname -s`");
  test_listeq(r, ((list){"echo", "f o o", "Linux", NULL}));
  list_free(&r);
  test(r == NULL);
}
