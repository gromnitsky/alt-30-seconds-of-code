---
title: Random string
---

Returns a random string of a user-requested length.

```c
char* randstr(int len)
char *randstr2(int len)
```

The first function gets pseudo-random data using plain random(3)
function. Before invoking it you need to seed it *once* (via
srandom(3) function) during a program initialisation somewhere in
`main()` (see the example below).

The second function get random bytes from /dev/urandom, then it
converts to a hex-looking string with [bin2hex](#bin2hex). You don't
need to seed it.

Both functions return malloc'ed values that should be freed.

```c
srandom(time(0))  // seed randstr()
randstr(0)        // "QZIR2wpb7wTDtU1IXCtMPBBrIYyNPrB0"
randstr(4)        // "s5aE"

randstr2(10)      // "f2e03e163a"
```

```c
<%= lines 'randstr.c', '#include "hex.c"', 'char\* uuid\(\) {' %>

<%= lines 'randstr.c', '// no seeding is req', 'void randstr\(\) {' %>
```
