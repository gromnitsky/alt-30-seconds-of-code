---
title: trim
---

Removes whitespace from both ends of a string.

```c
char* trim(const char *s)
```

* `s`: a string to operate on.

Return a new malloc'ed string that should be freed.

```c
trim("  foo bar ") // "foo bar"
```

```c
<%= lines 'trim.c', '#include "utils.h"', 'void trim\(\) {' %>
```
