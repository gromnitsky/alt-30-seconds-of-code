---
title: Backticks
---

Returns the stdout of a running command in a subshell with its exit code.

```c
Backticks backticks(char* cmd);

<%= lines 'backticks.c', '#include "utils.h"', 'Backticks backticks\(char\* cmd\) {' %>
```

* `cmd`: a string containing a shell command line that is passed to
  /bin/sh using the -c flag.

`Backticks.exit_code` is the termination status of the process, -1 on
internal error. `Backticks.stdout` is a null-terminated string that is
malloc'ed and should be freed.

The snippet uses [_read_file](#read_file). The example below uses
[trim](#trim).

~~~
[cling]$ Backticks r = backticks("curl -s 'https://ipinfo.io/1.2.3.4' | json region")
(Backticks &) @0x7f5e1580f018
[cling]$ r.exit_code
(int) 0
[cling]$ trim2(r.stdout)
(char *) "Greater Poland"
~~~

```c
<%= lines 'backticks.c', '#include "utils.h"', 'void backticks\(\) {' %>
```
