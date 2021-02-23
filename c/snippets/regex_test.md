---
title: regex_test
---

Tests if a string matches a regex.

```c
bool regex_test(const char *pattern, const char* s)
```

* `pattern`: a regex;
* `s`: a string to operate on.

```c
regex_test("o+", "foo") // true
regex_test("z", "foo")  // false
```

```c
#include <regex.h>

<%= lines 'regex_test.c', '#include "utils.h"', 'void regex_test\(\) {' %>
```
