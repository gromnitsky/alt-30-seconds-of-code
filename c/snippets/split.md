---
title: Split or replace
---

JavaScript-like `String.prototype.split()` &
`String.prototype.replace()`. They are combined in 1 entry, for they
both use crude POSIX `regexec()` to find matches & hence share almost
all the code.

This is the most complex/long snipped so far, but it's so handy that I
decided to put it here anyway.

The difference between JS `replace()` and our replace:

* no callback function;
* a replacement string doesn't support any of special "replacement
  patterns" like `$&` or `$1`.
* regex flags are limited to `i`, `m`, `g`.


```c
char **split(const char *pattern, const char *s);

char *replace(const char *pattern, const char *options,
              const char *s, const char *replacement);
```

* `pattern`: a regex;
* `s`: a string to operate on;
* `options`: a combination of `i`, `m`, `g` flags;
* `replacement`: a string that replace a matched substring specified by
  `pattern`.

Both functions return malloc'ed values that should be freed. `split()`
returns an array of `char*` elements (each of which is malloc'ed too)
ended by `NULL`.

```c
split(" +", "abc    def ghi") // ["abc", "def", "ghi", NULL]
replace(" ", NULL, "abc def ghi", "_") // "abc_def ghi"
replace(" ", "g", "abc def ghi", "_") // "abc_def_ghi"
```

The snipped uses [slice](#slice), [join](#join) &
[list_free](#str_vector_utils). To print the results of `slice()`, see
[list_print](#str_vector_utils).

~~~
#include <regex.h>

<%= lines 'split.c', '#include "join.c"', 'void split\(\) {' %>
~~~
