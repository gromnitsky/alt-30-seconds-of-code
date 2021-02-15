// cling -Wno-writable-strings read_file.c -lcrypto
#include "sha256.c"
#include "utils.h"

#include <linux/limits.h>
#include <unistd.h>
#include <fcntl.h>

typedef struct {
  char name[PATH_MAX];
  int size;
  char *data;
} LargeFile;

LargeFile read_file(char *name) {
  LargeFile r = { .size = -1 };
  if (!name) return r;

  int fd = 0;
  if (0 == strcmp(name, "-")) {
    name = "<stdin>";
  } else {
    fd = open(name, O_RDONLY); if (-1 == fd) return r;
  }
  snprintf(r.name, sizeof(r.name), "%s", name);

  r.size = 0;
  int bufsiz = BUFSIZ;
  char *buf = (char*)malloc(bufsiz);
  char *bufp = buf;
  char tmp[BUFSIZ];
  for (int lastread = 0; (lastread = read(fd, tmp, BUFSIZ)); ) {
    //printf("bufsiz=%d, lastread=%d, r.size=%d\n", bufsiz, lastread, r.size);
    if (r.size + lastread >= bufsiz - 1) {
      bufsiz *= 2;
      //printf("* realloc bufsiz=%d\n", bufsiz);
      buf = (char*)realloc(buf, bufsiz); assert(buf);
      bufp = buf + r.size;
    }
    r.size += lastread;
    memcpy(bufp, tmp, lastread);
    bufp += lastread;
  }
  if (fd) close(fd);

  buf = (char*)realloc(buf, r.size + 1); assert(buf);
  buf[r.size] = '\0';

  r.data = buf;
  return r;
}

void read_file() {
  // 142M file from fluid-soundfont-gm Fedora package
  LargeFile r = read_file("/usr/share/soundfonts/FluidR3_GM.sf2");
  test_streq(r.name, "/usr/share/soundfonts/FluidR3_GM.sf2");
  test(148398306 == r.size);

  CryptoDigest cd = digest_init(NULL);
  digest_upd(&cd, r.data, r.size);
  digest_fin(&cd);
  test_streq(bin2hex(cd.bindata, cd.len),
             "4fd467afcb45c8b4cfd1eadac9372a819e35d46d");
}
