---
title: Progress indicator
---

Redraw `min/cur/max %` indicator on stderr when it's connected to a
tty: <code id="snippet__progress">-5/-5/17 0.00%</code>

<script>
document.addEventListener('DOMContentLoaded', snippet_progress)
function snippet_progress() {
  let node = document.querySelector('#snippet__progress')
  let delay = 2000
  let min = -5, cur = -5, max = 17
  let update = () => {
    let percent = (((cur - min) * 100) / ((max - min)+0.0)).toFixed(2)
    node.innerText = `${min}/${cur++}/${max} ${percent}%`
    if (cur > max) cur = min
    setTimeout(update, delay)
  }
  setTimeout(update, 0)
}
</script>

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
