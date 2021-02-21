---
title: shellquote
---

Returns a shell-escaped version of a string.

```c
char* shellquote(char *s);
```

* `s`: a string to operate on.

The result is malloc’ed & should be freed.

~~~
shellquote("foo'bar") // "'foo'\''bar'"
~~~

```c
<%= lines 'shellquote.c', '#include "utils.h"', 'void shellquote\(\) {' %>
```

You can also write a smaller version with [replace](#split):
substitute each occurrence of `'` with `'\''` & decorate both ends of
the resulting string with `'`.
