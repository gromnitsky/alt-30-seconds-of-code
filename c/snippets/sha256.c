// cling -Wno-writable-strings sha256.c -lcrypto
#include "utils.h"
#include "hex.c"

// tested with 1.1.1i
#include <openssl/evp.h>

typedef struct {
  char *bindata;
  int len;
  // private
  EVP_MD_CTX *ctx;
  const EVP_MD *(*algo)();
} CryptoDigest;

CryptoDigest digest_init(char *algorithm) {
  CryptoDigest cd  = { .bindata = NULL, .len = -1, .algo = EVP_sha1};

  if (!algorithm) algorithm = "";
  if (0 == strcmp(algorithm, "sha256")) cd.algo = EVP_sha256;

  cd.ctx = EVP_MD_CTX_new(); assert(cd.ctx);
  assert(1 == EVP_DigestInit_ex(cd.ctx, cd.algo(), NULL));
  return cd;
}

void digest_upd(CryptoDigest *cd, char *s, int len) {
  if ( !(s && len >= 0)) return;
  assert(1 == EVP_DigestUpdate(cd->ctx, s, len));
}

void digest_fin(CryptoDigest *cd) {
  cd->bindata = (char *)malloc(EVP_MD_size(cd->algo()));
  assert(cd->bindata);

  assert(1 == EVP_DigestFinal_ex(cd->ctx, (unsigned char*)cd->bindata,
                                 (unsigned int*)&cd->len));
  EVP_MD_CTX_free(cd->ctx);
}

CryptoDigest digest(char *algorithm, char *s, int len) {
  CryptoDigest cd = digest_init(algorithm);
  digest_upd(&cd, s, len);
  digest_fin(&cd);
  return cd;
}

char *digest_hex(char *algorithm, char *s) {
  if (!s) return NULL;
  CryptoDigest cd = digest(algorithm, s, strlen(s));
  char *r = bin2hex(cd.bindata, cd.len);
  free(cd.bindata);
  return r;
}

void sha256() {
  test(NULL == digest_hex(NULL, NULL));
  test_streq(digest_hex(NULL, "foo"), "0beec7b5ea3f0fdbc95d0dd47f3c5bc275da8a33");
  test_streq(digest_hex("sha256", ""), "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855");
}
