---
title: Simple split
---

Divides a string into substrings based on a delimiter (that is a
simple string), returning a list of these substrings. If you need to
split by a regexp pattern, see much more complex [split](#split).

```c
char** simple_split(const char *separator, const char *str);
```

* `separator`: a pattern;
* `str`: a string to operate on.

Return an array of strings terminated with NULL. The result should be
freed with [list_free](#str_vector_utils).

```c
simple_split(" ", " abc def  ghi") // ["abc", "def", "ghi", NULL]
```

It uses `strtok_r(3)`, which *is* thread-safe.

```c
<%= lines 'simple_split.c', '#include "utils.h"', 'void simple_split\(\) {' %>
```

You may increase the initial value of `r_len` to avoid almost
immediate calls to `realloc()`.
