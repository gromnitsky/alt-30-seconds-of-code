// cling -Wno-writable-strings split.c

#include <regex.h>
#include "utils.h"

#include "slice.c"
#include "join.c"

char** list_realloc(char **list, int idx, int *size) {
  if (list) {
    if (idx < *size - 1) return list; // no need to realloc
    *size = idx * 2;
  }
  char **r = (char**)realloc(list, (*size) * sizeof(char*)); assert(r);
  return r;
}

typedef struct RegexOptions {
  bool all;
  int flags;
} RegexOptions;

RegexOptions regex_options_parse(char *spec) {
  if (!spec) spec = "";
  RegexOptions opt = { false, REG_EXTENDED };
  if (strstr(spec, "i")) opt.flags |= REG_ICASE;
  if (strstr(spec, "m")) opt.flags |= REG_NEWLINE;
  opt.all = !!strstr(spec, "g");
  return opt;
}

char **regex_match(char *pattern, char *options, char *s, char *replacement) {
  if (!s) return NULL;
  if (!pattern) pattern = "";
  RegexOptions opt = regex_options_parse(options);

  regex_t re;
  if (0 != regcomp(&re, pattern, opt.flags)) return NULL; // invalid RE

  regmatch_t pm;
  char *p = s;
  char **result = NULL;
  int error, idx = 0, flags = 0, len = strlen(s), result_size = 3,
    step = 0, replacement_count = 0, circumflex, fugazy;
  do {
    result = list_realloc(result, idx, &result_size);

    p = p + step;
    error = regexec(&re, p, 1, &pm, idx > 0 ? REG_NOTBOL : 0);
    int end = error ? strlen(p) : pm.rm_so;
    step = pm.rm_eo;
    circumflex = strcmp(pattern, "^") == 0;
    if ( (fugazy = (0 == pm.rm_so && 0 == pm.rm_eo))) {
      if (circumflex) {
        end = len;
      } else {
        step = end = 1;
      }
      if (idx == 0 && replacement && !error) { // inject matched substring
        result = list_realloc(result, idx, &result_size);
        result[idx++] = strdup(replacement);
        replacement_count++;
      }
    }

    char *data = slice(p, 0, end); // data BEFORE matched chunk
    //printf("%d %d:`%s`: %d-%d `%s`\n", idx, error, p, pm.rm_so, pm.rm_eo, data);
    if ( !(error && fugazy)) result[idx++] = data;

    if (replacement && !error && !circumflex) { // inject matched substring
      result = list_realloc(result, idx, &result_size);
      if (opt.all || replacement_count++ == 0) {
        result[idx++] = strdup(replacement);
      } else {
        result[idx++] = slice(p, pm.rm_so, pm.rm_eo);
      }
    }
  } while (!error && strlen(p)-1 > 0 && pm.rm_so < len);

  result[idx] = NULL;
  regfree(&re);
  return result;
}

char **split(char *pattern, char *s) {
  return regex_match(pattern, NULL, s, NULL);
}

void list_free(char **list) {
  if (!list) return;
  for (char **p = list; *p; p++) free(*p);
  free(list);
}

char *replace(char *pattern, char *options, char *s, char *replacement) {
  char **list = regex_match(pattern, options, s, replacement);
  char *r = join(list, "");
  list_free(list);
  return r;
}

void split() {
  typedef char *list[];

  test_listeq(split("&", "An old maid of five & thirty eloped with a colonel of forty"), ((list){"An old maid of five ", " thirty eloped with a colonel of forty", NULL}));
  test_listeq(split(" ", "abc def ghi"), ((list){"abc", "def", "ghi", NULL}));
  test_streq(replace(" ", NULL, "abc def ghi", "_"), "abc_def ghi");
  test_streq(replace(" ", "g", "abc def ghi", "_"), "abc_def_ghi");

  // FIXME
  //test_listeq(split(" *",  "abc def ghi"), ((list){"a", "b", "c", "d", "e", "f", "g", "h", "i", NULL}));
  test_streq(replace(" *", "g", "abc def ghi", "_"), "_a_b_c__d_e_f__g_h_i_");

  test_listeq(split("",  "abc def ghi"), ((list){"a", "b", "c", " ", "d", "e", "f", " ", "g", "h", "i", NULL}));
  test_streq(replace("", "g", "abc def ghi", "_"), "_a_b_c_ _d_e_f_ _g_h_i_");
  test_streq(replace("", "", "abc def ghi", "_"), "_abc def ghi");

  test_listeq(split("^", "abc def ghi"), ((list){"abc def ghi", NULL}));
  // FIXME
  //test_listeq(split("^", ""), ((list){NULL}));
  test_streq(replace("^", "g", "abc def ghi", "_"), "_abc def ghi");
  test_listeq(split("^", "_"), ((list){"_", NULL}));
  test_streq(replace("^.", "g", "abc def ghi", "_"), "_bc def ghi");

  test_listeq(split("$", "abc def ghi"), ((list){"abc def ghi", NULL}));
  test_streq(replace("$", "g", "abc def ghi", "_"), "abc def ghi_");
  test_streq(replace(".$", "g", "abc def ghi", "_"), "abc def gh_");

  test_listeq(split("..", "abc def ghi"), ((list){"", "", "", "", "", "i", NULL}));
  // FIXME
  //test_listeq(split(".", "abc def ghi"), ((list){"", "", "", "", "", "", "", "", "", "", "", "", NULL}));
}
