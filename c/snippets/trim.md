---
title: trim
---

Removes whitespace from both ends of a string.

```c
char* trim(const char *s);
char* trim2(char s[]);
```

* `s`: a string to operate on.

`trim()` returns a new malloc'ed string that should be
freed. `trim2()` modifies the passed array *in-place* & returns a
pointer to it.

```
trim("  foo bar ")            // "foo bar"

[cling]$ char s[] = "  foo "
(char [7]) "  foo "
[cling]$ trim2(s)
(char *) "foo"
[cling]$ s
(char [7]) "foo\0o "
[cling]$ strlen(s)
(unsigned long) 3
```

```c
<%= lines 'trim.c', '#include "utils.h"', 'void trim\(\) {' %>
```
