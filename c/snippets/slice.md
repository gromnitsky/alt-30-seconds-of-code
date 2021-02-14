---
title: slice
---

An equivalent to JavaScript `String.prototype.slice()`: extracts a
section of a string & returns it as a new string, without modifying
the original string.

```c
char* slice(char *s, int begin, int end);
```

* `s`: a string to operate on;
* `begin`: a 0-based index at which to begin extraction;
* `end`: a 0-based index *before* which to end extraction. The char at
  this index isn't included.

```c
slice("qwerty", 2, -1) // "ert"
slice("qwerty", -6, 1) // "q"
slice("qwerty", 0, 100) // "qwerty"
```

Return a malloc'ed value that should be freed.

~~~
<%= lines 'slice.c', '#include "utils.h"', '// q w e r t y' %>
~~~
