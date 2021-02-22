---
title: slice
---

2 functions that are equivalents to JavaScript
`String.prototype.slice()` & `Array.prototype.slice()`: they extract a
section of a string/array & return it as a new string/array, without
modifying the original object.

```c
char* slice(char *s, int begin, int end);
char** list_slice(char **list, int begin, int end);
```

* `s`: a string to operate on;
* `list`: an array of strings to operate on, terminated with NULL,
  e.g., `["foo", "bar", NULL]`;
* `begin`: a 0-based index at which to begin extraction;
* `end`: a 0-based index *before* which to end extraction. The char at
  this index isn't included.

Both functions return malloc'ed values that should be freed with

* `slice()`: regular free(3);
* `list_slice()`: [list_free](#str_vector_utils).


```c
slice("qwerty", 2, -1)  // "ert"
slice("qwerty", -6, 1)  // "q"
slice("qwerty", 0, 100) // "qwerty"

char *v[] = {"q", "w", "e", "r", "t", "y", NULL};
list_slice(v, 2, -1)    // ["e", "r", "t", NULL]
```

~~~
<%= lines 'slice.c', '#include "utils.h"', '// q w e r t y' %>
~~~
