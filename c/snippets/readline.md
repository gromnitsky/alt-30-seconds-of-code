---
title: readline
---

POSIX `getdelim()` reads from a stream until it stumbles upon a char
matching a user-provided delimiter: `\n`, for example or even `\0`.

In the snippet below we replicate the 'awk' portion of the command:

    ip route | awk '/vmnet/ {print $1, "->", $9}'

as

~~~
$ ip route | ./our-example
172.16.207.0/24 -> 172.16.207.1
172.16.222.0/24 -> 172.16.222.1
~~~

To split a strings into chunks, the snippet uses [split](#split). For
trimming spaces, see [trim2](#trim).

```c
  <%= lines 'readline.c', 'void readline\(\) {', '}' %>
```

Memory for each line gets (re)allocated automatically.
