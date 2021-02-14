---
title: join
---

An equivalent to JavaScript `Array.prototype.join()`: returns a new
string by concatenating all of the elements in an array, separated by
a specified separator string.

    char* join(char **list, const char *separator)

* `list`: a pointer to an array of character strings (char\*)
  terminated by NULL, e.g. `["foo", "bar", NULL]`;
* `separator`: a string to separate each pair of adjacent elements of
  the array.

The result is malloc'ed & should be freed.

```c
typedef char *list[];

join((list){ "An", "old", "maid", "of", "five", "&", "thirty",
"eloped", "with", "a", "colonel", "of", "40", NULL}, " ") // "An old maid of five & thirty eloped with a colonel of 40"

join((list){"foo", NULL}, " ") // "foo"
```

See also [split](#split) that does the opposite--breaks a string into
an array of character strings.

```c
<%= lines 'join.c', '#include "utils.h"', 'void join\(\) {' %>
```
