---
title: Progress indicator
---

Redraw `123/456 26.97%` on stderr when it's connected to a tty.

~~~
Progress* progress_init(int min, int max);
void progress_update(Progress *p, int cur);
void progress_end(Progress **p);

<%= lines 'progress.c', '#include <unistd.h>', '  // private' %>
  // private fields are skipped
} Progress;
~~~

The indicator repaints itself no more often that 100ms, which is a
default value of `Progress.update_delay`. Increase it in case of a
cursor flickering.

* `min`, `max`: the indicator uses this to calculate the %;
* `p`: a result from `progress_init()`;
* `cur`: a current value, presumably between `min` & `max`.

`progress_init()` returns a malloc’ed value that should be freed with
`progress_end()`. The latter also makes a final redraw.

```c
  <%= lines 'progress.c', 'void progress\(\) {', '  fprintf\(stderr, "\\\n"\);' %>
```

The snippet uses [itoa_len](#itoa).

```c
<%= lines 'progress.c', '#include "itoa.c"', 'void progress\(\) {' %>
```
