#include "utils.h"
#include <unistd.h>

#include <errno.h>
#include <sys/stat.h>

bool mkdir_p(const char *s, mode_t mode) {
  char *component = strdup(s);
  char *p = component;

  bool status = true;
  while (*p && *p == '/') p++; // skip leading '/'

  do {
    while (*p && *p != '/') p++;

    if (!*p)
      p = NULL;
    else
      *p = '\0';

    if (-1 == mkdir(component, mode) && errno != EEXIST) {
      status = false;
      break;
    } else if (p) {
      *p++ = '/';
      while (*p && *p == '/') p++;
    }

  } while (p);

  free(component);
  return status;
}


void mkdir_p() {
  system("rm -rf tmp/foo");

  test(mkdir_p("/usr/bin", 0755));
  test(mkdir_p("/root/123", 0755) == false);

  test(mkdir_p("./tmp/foo/.//bar/../baz", 0700));
  test(access("tmp/foo/bar", R_OK | W_OK | X_OK) == 0);
  test(access("tmp/foo/baz", R_OK | W_OK | X_OK) == 0);
}
