// cling -Wno-writable-strings -lbackticks.c -lcrypto
#include "read_file.c"
#include "trim.c"
#include "utils.h"

typedef struct {
  int exit_code;
  char *stdout;
  int stdout_len;
} Backticks;

Backticks backticks(char* cmd) {
  Backticks r = { .exit_code = -1 };
  FILE *stream = popen(cmd, "r"); if (!stream) return r;
  fflush(stream);
  int len;
  char *s = _read_file(fileno(stream), &len);
  r.exit_code = WEXITSTATUS(pclose(stream));
  r.stdout = s;
  r.stdout_len = len;
  return r;
}

void backticks() {
  Backticks r = backticks("uname -s");
  test(0 == r.exit_code);
  test(6 == r.stdout_len);
  test_streq(trim(r.stdout), "Linux");

  r = backticks(NULL);
  test(2 == r.exit_code);

  r = backticks("no-such-command");
  test(127 == r.exit_code);
}
