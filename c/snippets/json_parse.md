---
title: Parse JSON
---

Read `{ "ip": "4.3.2.1", "city": "Atlantic City", ... }` using Jansson
(tested with 2.13.1). This library is robust & fast enough to be
chosen by such projects as Emacs.

Linker flags: `-ljansson`.

~~~
$ curl -s 'https://ipinfo.io/1.2.3.4' | ./our-example
ip = 1.2.3.4
city = Koło
region = Greater Poland
country = PL
loc = 52.2002,18.6386
postal = 62-600
timezone = Europe/Warsaw
~~~

The snippet uses [read_fd](#read_file).

```c
<%= lines 'json_parse.c', '#include "read_file.c"', '// test data' %>
```

In case you're wondering, `json_object_foreach` is indeed a macro.
