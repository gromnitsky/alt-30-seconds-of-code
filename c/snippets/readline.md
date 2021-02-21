---
title: readline
---

POSIX `getdelim()` reads from a stream until it stumbles upon a char
matching a user-provided delimiter: `\n`, for example or even `\0`.

In the snippet below we replicate the 'awk' portion of the command:

    ip route | awk '/vmnet/ {print $1, "->", $9}'

as

~~~
$ ip route | cling -Wno-writable-strings readline.c
172.16.207.0/24 -> 172.16.207.1
172.16.222.0/24 -> 172.16.222.1
~~~

For spliting strings into chunks, see [split](#split), for trimming
newlines, see [trim2](#trim).

```c
<%= File.read 'readline.c' %>
```

Memory for each line gets (re)allocated automatically.
