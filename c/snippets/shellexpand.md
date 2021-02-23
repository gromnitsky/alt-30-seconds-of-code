---
title: shellexpand
---

Expands shell variables, commands & splits a string into an array of
tokens--all in the same way /bin/sh does.

```c
char** shellexpand(const char *s);
```

* `s`: a string to operate on.

Return an array of strings terminated with NULL. The result should be
freed with [list_free](#str_vector_utils).

~~~
[cling]$ list_print(shellexpand("$USER is using `uname -s`"))
["alex", "is", "using", "Linux"]
~~~

The example uses [list_print](#str_vector_utils).

```c
<%= lines 'shellexpand.c', '#include "utils.h"', 'void shellexpand\(\) {' %>
```
