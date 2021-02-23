#include "utils.h"

char* bin2hex(const char *s, int len) {
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

typedef struct {
  char *bytes;
  int len;
} Hex2bin;

Hex2bin hex2bin(const char *s) {
  Hex2bin r = { .len = -1 };
  if (!s) return r;
  if (strlen(s) > 2 && s[0] == '0' && tolower(s[1]) == 'x') s += 2;
  if (strlen(s) % 2 != 0) return r; // not a properly formatted hex string?

  r.bytes = (char*)malloc(strlen(s)+1);
  char *pr = r.bytes;

  r.len = 0;
  char bytes[3];
  for (const char *p = s; *p; p += 2, r.len++) {
    snprintf(bytes, 3, "%s", p);
    char byte = strtol(bytes, NULL, 16);
    *pr++ = byte;
  }

  return r;
}

void hex() {
  test(bin2hex(NULL, 100) == NULL);
  test_streq(bin2hex("", 0), "");
  test_streq(bin2hex("пороблено", strlen("пороблено")),
             "d0bfd0bed180d0bed0b1d0bbd0b5d0bdd0be");

  Hex2bin r = hex2bin(NULL);
  test(r.len == -1);
  r = hex2bin(NULL); test(r.len == -1);

  r = hex2bin("d0bfd0bed180d0bed0b1d0bbd0b5d0bdd0be");
  r.bytes[r.len] = '\0';
  test_streq(r.bytes, "пороблено");

  r = hex2bin("0x48656c6c6f2c20576f726c6421");
  r.bytes[r.len] = '\0';
  test_streq(r.bytes, "Hello, World!");

  r = hex2bin(bin2hex("пороблено", strlen("пороблено")));
  r.bytes[r.len] = '\0';
  test_streq(r.bytes, "пороблено");
}
