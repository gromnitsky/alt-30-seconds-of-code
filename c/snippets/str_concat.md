---
title: Concatenate strings
---

Concatenates the string arguments and returns a new string.

```c
char* strcat2(char *s1, ...)
```

The result is malloc'ed & should be freed.

This is actually a macro that wraps a function with a variable-length
argument list. We need the macro for C VA lists have an unpleasant
interface: there is no way no know the exact number of function
arguments, thus a user would require either to specify the number of
arguments or manually append a terminator. Both methods are error
prone.

The snippet uses [join](#join).

```c
strcat2("foo", "bar", "baz") // "foobarbaz"
strcat2("foo", NULL, "baz")  // "foo"
strcat2(NULL)                // NULL
```

```c
<%= lines 'str_concat.c', '#include "utils.h"', 'void str_concat\(\) {' %>
```
