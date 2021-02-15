#include <time.h>
#include <fcntl.h>
#include <unistd.h>

#include "utils.h"
#include "regex_test.c"
#include "hex.c"

char* randstr(int len) {
  if (!len || len < 1) len = 32;
  char charset[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
  char *r = (char*)malloc(len + 1);
  r[len] = '\0';
  while (len--) r[len] = charset[random() % (sizeof charset - 1)];
  return r;
}

char* uuid() {
  char *s = randstr(16);
  char *r = (char*)malloc(16*2+4+1);
  snprintf(r, 36+1, "%02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-"
           "%02x%02x%02x%02x%02x%02x",
           s[0],  s[1],  s[2],  s[3],
           s[4],  s[5],  s[6],  s[7],
           s[8],  s[9],  s[10], s[11],
           s[12], s[13], s[14], s[15]);
  free(s);
  return r;
}

// no seeding is req
char *randstr2(int len) {
  if (!len || len < 2) len = 32;

  int fd = open("/dev/urandom", O_RDONLY); assert (-1 != fd);

  char buf[len/2 + 1];
  int n = read(fd, buf, len/2); assert(n == len/2);
  close(fd);
  buf[len/2] = '\0';
  return bin2hex(buf, len/2);
}


void randstr() {
  srandom(time(0));

  test(strlen(randstr(0)) == 32);
  test(strlen(randstr(10)) == 10);
  test(strlen(randstr(1)) == 1);

  test(regex_test("^\\w{8}-\\w{4}-\\w{4}-\\w{4}-\\w{12}$", uuid()));

  test(strlen(randstr2(0)) == 32);
  test(strlen(randstr2(10)) == 10);
  test(strlen(randstr2(2)) == 2);

  printf("%s\n", randstr2(0));
}
