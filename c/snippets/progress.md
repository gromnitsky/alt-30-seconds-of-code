---
title: Progress indicator
---

Draw on stderr `123/456 26.97%`, carefully erasing previous values on
a TTY.

~~~
Progress* progress_init(int min, int max);
void progress_update(Progress *p, int cur);
void progress_end(Progress **p);

typedef struct {
  int first;
  int last;
  int cur;
  long update_delay;
  // private fields are not shown
} Progress;
~~~

The indicator redraws itself no more often that 100ms, which is the
default value of `Progress.update_delay`.

* `min`, `max`: the indicator uses this to calculate the %;
* `p`: the result from `progress_init()`;
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
