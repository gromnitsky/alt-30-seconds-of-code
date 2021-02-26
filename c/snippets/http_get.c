#include "sha256.c"

#include "trim.c"
#include "hash_table.c"
#include "str_tr.c"
#include <curl/curl.h>
#include <err.h>

typedef struct {
  char *body;
  int len;
  int code;
  HashTable *headers;
  // curl-specific
  int err_code;
  char err_msg[CURL_ERROR_SIZE];
  // private
  bool _final_header_line;
} HttpRes;

void http_res_free(HttpRes *v) {
  if (!v) return;
  ht_free(&(v->headers));
  free(v->body);
}

size_t http_header_callback(char *buf, size_t _, size_t len, void *userdata) {
  HttpRes *r = (HttpRes*)userdata;
  if (len <= 2) r->_final_header_line = true;
  if (r->_final_header_line) return len; // chunked encoded trailer

  buf[len-2] = '\0'; // rm CR-RL

  char *val = strchr(buf, ':'); if (!val) return len;
  int key_len = val - buf + 1;
  char key[key_len];
  snprintf(key, key_len, "%s", buf);
  str_tr2(key, tolower);

  val++; trim2(val);

  ht_add(r->headers, key, strdup(val));
  return len;
}

size_t _http_get_chunk(char *chunk, size_t _, size_t len, void *userdata) {
  HttpRes *r = (HttpRes*)userdata;
  r->body = (char*)realloc(r->body, r->len + len); assert(r->body);
  memcpy(r->body+r->len, chunk, len);
  r->len += len;
  return len;
}

HttpRes http_prepare(const char *url, CURL **curl) {
  HttpRes r = { .err_code = -1 };
  strcpy(r.err_msg, "no url or curl_easy_init() fail");
  *curl = curl_easy_init();
  if ( !(*curl && url)) return r;

  curl_easy_setopt(*curl, CURLOPT_ERRORBUFFER, r.err_msg);
  curl_easy_setopt(*curl, CURLOPT_URL, url);
  curl_easy_setopt(*curl, CURLOPT_FOLLOWLOCATION, 1);
  curl_easy_setopt(*curl, CURLOPT_MAXREDIRS, 5);
  curl_easy_setopt(*curl, CURLOPT_WRITEFUNCTION, _http_get_chunk);
  curl_easy_setopt(*curl, CURLOPT_WRITEDATA, &r);
  r.headers = mk_hash_table(42, free);
  curl_easy_setopt(*curl, CURLOPT_HEADERFUNCTION, http_header_callback);
  curl_easy_setopt(*curl, CURLOPT_HEADERDATA, &r);
  if (getenv("CURLOPT_VERBOSE") && 0 == strcmp(getenv("CURLOPT_VERBOSE"), "1")) {
    curl_easy_setopt(*curl, CURLOPT_VERBOSE, 1);
  }
  return r;
}

struct curl_slist* http_custom_headers(CURL *curl, const char **headers) {
  struct curl_slist *hdr = NULL;
  if (headers) {
    for (const char **p = headers; *p; p++) hdr = curl_slist_append(hdr, *p);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, hdr);
  }
  return hdr;
}

void http_get_status(HttpRes *r, CURL *curl) {
  int code;
  curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &code);
  r->code = code;
}

HttpRes http_get(const char *url, const char **headers) {
  CURL *curl = NULL;
  HttpRes r = http_prepare(url, &curl);
  struct curl_slist *hdr = http_custom_headers(curl, headers);

  r.err_code = curl_easy_perform(curl);

  http_get_status(&r, curl);
  curl_slist_free_all(hdr);
  curl_easy_cleanup(curl);
  return r;
}

// result must be freed
char *http_ht_to_post_data(CURL *curl, HashTable *fields) {
  char **keys = ht_keys(fields);
  int len = ht_total(fields);
  char **list = (char**)malloc((len + 1) * sizeof(char*));
  int idx = 0;
  for (char **p = keys; *p; p++) {
    char *k = curl_easy_escape(curl, *p, strlen(*p));
    char *val = (char*)(ht_find(fields, *p)->val);
    char *v = curl_easy_escape(curl, val, strlen(val));
    char param_len = strlen(k) + strlen(v) + 2; // including '='
    char param[param_len];
    snprintf(param, param_len, "%s=%s", k, v);
    free(k);
    free(v);
    list[idx++] = strdup(param);
  }
  list[len] = NULL;
  free(keys);
  char *data = join((const char**)list, "&");
  list_free(&list);
  return data;
}

HttpRes http_post(const char *url, const char **headers, HashTable *fields) {
  CURL *curl = NULL;
  HttpRes r = http_prepare(url, &curl);
  struct curl_slist *hdr = http_custom_headers(curl, headers);

  assert(fields);
  char *data = http_ht_to_post_data(curl, fields);
  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
  r.err_code = curl_easy_perform(curl);
  free(data);

  http_get_status(&r, curl);
  curl_slist_free_all(hdr);
  curl_easy_cleanup(curl);
  return r;
}

char* http_res_body_str(HttpRes *r) {
  if ( !(r && r->body)) return NULL;
  r->body = (char*)realloc(r->body, r->len+1);
  r->body[r->len] = '\0';
  return r->body;
}

void http_get() {
  const char *hdr[] { "User-Agent: test/0.0.1", NULL };
  HttpRes r = http_get("http://127.0.0.1:3000/cat.webp", hdr);
  if (r.err_code != 0) {
    errx(1, "%d: %s", r.err_code, r.err_msg);
  } else {
    CryptoDigest cd = digest_init(NULL);
    digest_upd(&cd, r.body, r.len);
    digest_fin(&cd);
    test(200 == r.code);
    test_streq(bin2hex(cd.bindata, cd.len), "bda9185ab01773f3d09771bd64d008b064b852ce");
  }
  http_res_free(&r);

  r = http_get("http://127.0.0.1:3000/loop", hdr);
  test(47 == r.err_code);
  http_res_free(&r);

  HashTable *fields = mk_hash_table(42, free);
  ht_add(fields, "name", strdup("William Chaloner"));
  ht_add(fields, "age", strdup("49"));
  r = http_post("http://127.0.0.1:3000/form1", NULL, fields);
  ht_free(&fields);
  if (r.err_code != 0) {
    errx(1, "%d: %s", r.err_code, r.err_msg);
  } else {
    test(200 == r.code);
    test_streq(http_res_body_str(&r), "{\"name\":\"William Chaloner\",\"age\":\"49\"}");
  }

  http_res_free(&r);
}
