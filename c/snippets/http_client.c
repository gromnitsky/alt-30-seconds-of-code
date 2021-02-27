#include "sha256.c"

#include "trim.c"
#include "hash_table.c"
#include "str_tr.c"
#include <err.h>
#include <curl/curl.h>

typedef struct {
  char *body;
  size_t len;
  long status;
  HashTable *headers;
  // curl-specific
  int err_code;
  char err_msg[CURL_ERROR_SIZE];
  // private
  bool _final_header_line;
  CURL *curl;
  struct curl_slist *custom_headers;
} HttpRes;

void http_res_free(HttpRes *v) {
  if (!v) return;
  if (v->headers) ht_free(&(v->headers));
  free(v->body);
  curl_slist_free_all(v->custom_headers);
  curl_easy_cleanup(v->curl);
}

size_t _http_header_callback(char *buf, size_t _, size_t len, void *userdata) {
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

// result must be freed with curl_slist_free_all()
struct curl_slist* _http_custom_headers(CURL *curl, const char **headers) {
  struct curl_slist *hdr = NULL;
  if (headers) {
    for (const char **p = headers; *p; p++) hdr = curl_slist_append(hdr, *p);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, hdr);
  }
  return hdr;
}

HttpRes http_init(const char *url, const char **headers) {
  HttpRes r = { .err_code = -1 };
  strcpy(r.err_msg, "no url or curl_easy_init() fail");
  CURL *curl = curl_easy_init();
  if ( !(curl && url)) return r;

  r.curl = curl;
  curl_easy_setopt(r.curl, CURLOPT_URL, url);
  curl_easy_setopt(r.curl, CURLOPT_FOLLOWLOCATION, 1);
  curl_easy_setopt(r.curl, CURLOPT_MAXREDIRS, 10);
  curl_easy_setopt(r.curl, CURLOPT_WRITEFUNCTION, _http_get_chunk);
  r.headers = mk_hash_table(42, free);
  curl_easy_setopt(r.curl, CURLOPT_HEADERFUNCTION, _http_header_callback);
  if (getenv("CURLOPT_VERBOSE") && 0 == strcmp(getenv("CURLOPT_VERBOSE"), "1")) {
    curl_easy_setopt(r.curl, CURLOPT_VERBOSE, 1);
  }
  r.custom_headers = _http_custom_headers(r.curl, headers);
  return r;
}

void http_run(HttpRes *r) {
  curl_easy_setopt(r->curl, CURLOPT_ERRORBUFFER, r->err_msg);
  curl_easy_setopt(r->curl, CURLOPT_HEADERDATA, r);
  curl_easy_setopt(r->curl, CURLOPT_WRITEDATA, r);
  r->err_code = curl_easy_perform(r->curl);
  curl_easy_getinfo(r->curl, CURLINFO_RESPONSE_CODE, &r->status);
}

HttpRes http_get(const char *url, const char **headers) {
  HttpRes r = http_init(url, headers);
  http_run(&r);
  return r;
}

// result must be freed
char* _http_ht_to_post_data(CURL *curl, HashTable *fields) {
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
  HttpRes r = http_init(url, headers);

  assert(fields);
  char *data = _http_ht_to_post_data(r.curl, fields);
  curl_easy_setopt(r.curl, CURLOPT_POSTFIELDS, data);

  http_run(&r);

  free(data);
  return r;
}

char* http_res_body_str(HttpRes *r) {
  if ( !(r && r->body)) return NULL;
  r->body = (char*)realloc(r->body, r->len+1);
  r->body[r->len] = '\0';
  return r->body;
}


void http_client() {
  const char *hdr[] { "User-Agent: test/0.0.1", NULL };
  HttpRes r = http_get("http://127.0.0.1:3000/cat.webp", hdr);
  if (r.err_code != 0) {
    errx(1, "%d: %s", r.err_code, r.err_msg);
  } else {
    CryptoDigest cd = digest_init(NULL);
    digest_upd(&cd, r.body, r.len);
    digest_fin(&cd);
    test(200 == r.status);
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
    test(200 == r.status);
    test_streq(http_res_body_str(&r), "{\"name\":\"William Chaloner\",\"age\":\"49\"}");
  }
  http_res_free(&r);

  // multipart
  r = http_init("http://127.0.0.1:3000/form2", NULL);
  curl_mime *mime = curl_mime_init(r.curl);
  curl_mimepart *p = curl_mime_addpart(mime);
  curl_mime_name(p, "name"); curl_mime_data(p, "a cat", CURL_ZERO_TERMINATED);
  p = curl_mime_addpart(mime);
  curl_mime_name(p, "photo"); curl_mime_filedata(p, "cat.webp");
  curl_easy_setopt(r.curl, CURLOPT_MIMEPOST, mime);
  http_run(&r);
  curl_mime_free(mime);
  if (r.err_code != 0) {
    errx(1, "%d: %s", r.err_code, r.err_msg);
  } else {
    test(200 == r.status);
    test(r.len > 50);
    test(strstr(http_res_body_str(&r), "<img src"));
  }
  http_res_free(&r);
}
