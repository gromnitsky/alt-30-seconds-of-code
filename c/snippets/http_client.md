---
title: HTTP get & post
---

Make GET/POST requests using libcurl (tested with
7.71.1). Unfortunately, libcurl is not a very high-level library:
without this snippet you'd have to manually parse headers into
key/value pairs, properly encode data for for
application/x-www-form-urlencoded posts, write a custom callback for
obtaining a response body.

Linker flags: `-lcurl`.

```c
HttpRes http_get(const char *url, const char **headers);
HttpRes http_post(const char *url, const char **headers, HashTable *fields);
void http_res_free(HttpRes *v);

typedef struct {
  char *body;
  size_t len;
  long status;
  HashTable *headers;
  // curl-specific
  int err_code;
  char err_msg[CURL_ERROR_SIZE];
  // private fields are skipped
} HttpRes;

// utils & a more low-level api
char* http_res_body_str(HttpRes *r);
HttpRes http_init(const char *url, const char **headers);
void http_run(HttpRes *r);
```

* `url`: guess what;
* `headers`: an optional NULL-terminated list of strings with custom
  user headers, e.g., `{ "User-Agent: test/0.0.1", NULL }`;
* `fields`: a [hash table](#hash_table) instance of name-value pairs
  from which `http_post()` encodes a data string a plain POST
  `application/x-www-form-urlencoded` request.

The functions are blocking & automatically follow up to 10
redirections. The results should be freed via
`http_res_free()`. `HttpRes.body` is NOT null-terminated; if your
responce is a text based, use `http_res_body_str()` to extract a
null-terminated string (do NOT free its result, though).

If there was no errors during a request, `HttpRes.err_code` will
be 0. `HttpRes.status` is an HTTP status like 200 or 404.

`HttpRes.headers` is a [hash table](#hash_table) instance with all
responce headers, e.g. `(char*)ht_find(r.headers,
"last-modified")->val` retreives a value. Keys are in a lower
case. N.B.: if there were several headers with the same key, only the
last one will be availabe.

To emulate `curl -v`, set CURLOPT_VERBOSE=1 environment variable.

Collect a text responce into a string:

```c
[cling]$ HttpRes r = http_get("https://www.google.com/.well-known/security.txt", NULL)
(HttpRes &) @0x7f04a550e018
[cling]$ r.err_code
(int) 0
[cling]$ r.status
(long) 200
[cling]$ http_res_body_str(&r)
(char *) "Contact: https://g.co/vulnz
Contact: mailto:security@google.com
Encryption: https://services.google.com/corporate/publickey.txt
Acknowledgements: https://bughunter.withgoogle.com/
Policy: https://g.co/vrp
Hiring: https://g.co/SecurityPrivacyEngJobs
# Flag: BountyCon{075e1e5eef2bc8d49bfe4a27cd17f0bf4b2b85cf}
"
[cling]$ (char*)ht_find(r.headers, "last-modified")->val
(char *) "Mon, 25 May 2020 08:30:00 GMT"
```

Submit a form:

~~~
HashTable *h = mk_hash_table(42, free)
(HashTable *) 0x46ff130
[cling]$ ht_add(h, "name", strdup("bob"))
[cling]$ HttpRes r = http_post("https://httpbin.org/post", NULL, h)
(HttpRes &) @0x7f6a0f728020
[cling]$ http_res_body_str(&r)
(char *) "{
  "args": {},
  "data": "",
  "files": {},
  "form": {
    "name": "bob"
  },
  "headers": {
    "Accept": "*/*",
    "Content-Length": "8",
    "Content-Type": "application/x-www-form-urlencoded",
    "Host": "httpbin.org",
    "X-Amzn-Trace-Id": "Root=1-603a8e8d-21e2fea61b71836f6be21ad7"
  },
  "json": null,
  "origin": "<redacted>",
  "url": "https://httpbin.org/post"
}
"
~~~

To make a multipart POST, you need to manually fill up `curl_mimepart`
structure. Here we create 2 mime parts `name="a cat"` and `photo`
with the contents of "cat.webp" file:

~~~
// prepare a request
HttpRes r = http_init("http://127.0.0.1:3000/form2", NULL);

curl_mime *mime = curl_mime_init(r.curl);
curl_mimepart *p = curl_mime_addpart(mime);
curl_mime_name(p, "name"); curl_mime_data(p, "a cat", CURL_ZERO_TERMINATED);
p = curl_mime_addpart(mime);
curl_mime_name(p, "photo"); curl_mime_filedata(p, "cat.webp");

// this will be a POST
curl_easy_setopt(r.curl, CURLOPT_MIMEPOST, mime);

http_run(&r); // do the actual request
// examine r.status or whatever
// ...

// cleanup
curl_mime_free(mime);
http_res_free(&r)
~~~

The snippet also uses [trim2](#trim) & [str_tr2](#str_tr).

```c
<%= lines 'http_client.c', '#include <err.h>', 'void http_client\(\) {' %>
```

If you don't want to include [HashTable](#hash_table) implementation,
replace it with with a linked list (in
`_http_header_callback()`). curl comes with curl_slist_append(3), for
example.
