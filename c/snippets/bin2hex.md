---
title: bin2hex
---

Converts a "binary" string to a hex-looking string. The same as

~~~
$ echo d0bfd0bed180d0bed0b1d0bbd0b5d0bdd0be | xxd -r -p
пороблено
~~~

in shell.

```c
char* bin2hex(char *s, int len)
```

* `s`: a string to operate on;
* `len`: a length of `s`. We need this argument for `s` can contain a
  null char.

The result is malloc’ed & should be freed.

See also [hex2bin](#hex2bin).

```c
bin2hex("пороблено", strlen("пороблено")) // "d0bfd0bed180d0bed0b1d0bbd0b5d0bdd0be"
```

```c
<%= lines 'hex.c', '#include "utils.h"', 'char\* hex2bin\(char \*s\) {' %>
```
