// see Makefile for how to run it
#include "read_file.c"
#include <jansson.h>
#include <err.h>

void json_parse() {
  int _;
  char *input = read_fd(0, &_);

  json_error_t e;
  json_t *val, *root = json_loads(input, 0, &e);
  if (!root) errx(1, "%d: %s", e.line, e.text);
  free(input);

  const char *key;
  json_object_foreach(root, key, val) {
    printf("%s = %s\n", key, json_string_value(val));
  }

  json_decref(root);
}

// test data
//
// {
//   "ip": "1.2.3.4",
//   "city": "Koło",
//   "region": "Greater Poland",
//   "country": "PL",
//   "loc": "52.2002,18.6386",
//   "postal": "62-600",
//   "timezone": "Europe/Warsaw",
//   "readme": "https://ipinfo.io/missingauth"
// }
