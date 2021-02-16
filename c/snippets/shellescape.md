---
title: shellescape
---

Returns a shell-escaped version of a string.

```c
char* shellescape(char *s);
```

* `s`: a string to operate on.

The result is malloc’ed & should be freed.

~~~
shellescape("foo'bar") // "'foo'\''bar'"
~~~

```c
<%= lines 'shellescape.c', '#include "utils.h"', 'void shellescape\(\) {' %>
```

You can also write a smaller version with [replace](#split):
substitute each occurrence of `'` with `'\''` & decorate both ends of
the resulting string with `'`.
