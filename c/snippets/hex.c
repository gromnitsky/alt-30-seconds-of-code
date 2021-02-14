#include "utils.h"

char* bin2hex(char *s, int len) {
  if (!s) return NULL;
  char *r = (char*)malloc(len*2 + 1); assert(r);
  *r = '\0';
  char hex[3];
  for (int idx = 0; idx < len; idx++) {
    snprintf(hex, 3, "%02x", (unsigned char)s[idx]);
    strcat(r, hex);
  }
  return r;
}

char* hex2bin(char *s) {
  if (!s) return NULL;
  if (strlen(s) > 2 && s[0] == '0' && tolower(s[1]) == 'x') s += 2;
  if (strlen(s) % 2 != 0) return NULL; // not a properly formatted hex string?

  char *r = (char*)malloc(strlen(s)+1); assert(r);
  char *pr = r;

  char bytes[3];
  for (char *p = s; *p; p += 2) {
    snprintf(bytes, 3, "%s", p);
    char byte = strtol(bytes, NULL, 16);
    *pr++ = byte;
  }
  *pr = '\0';
  return r;
}

void hex() {
  test(bin2hex(NULL, 100) == NULL);
  test_streq(bin2hex("", 0), "");
  test_streq(bin2hex("пороблено", strlen("пороблено")),
             "d0bfd0bed180d0bed0b1d0bbd0b5d0bdd0be");

  test(hex2bin(NULL) == NULL);
  test_streq(hex2bin(""), "");
  test_streq(hex2bin("d0bfd0bed180d0bed0b1d0bbd0b5d0bdd0be"), "пороблено");
  test_streq(hex2bin("0x48656c6c6f2c20576f726c6421"), "Hello, World!");

  test_streq(hex2bin(bin2hex("пороблено", strlen("пороблено"))), "пороблено");
}
