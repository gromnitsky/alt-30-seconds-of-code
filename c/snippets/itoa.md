---
title: itoa
---

Converts an integer to a string.

```c
char *itoa(int n);
```

The result is malloc’ed & should be freed.

```c
itoa(-10) // "-10"
```

```c
#include <math.h>

<%= lines 'itoa.c', '#include "utils.h"', 'void itoa\(\) {' %>
```

The only interesting part here is how the length of the final string
is calculated:

~~~
[cling]$ floor(log10l(labs(100)))
(long double) 2.0000000L
~~~
