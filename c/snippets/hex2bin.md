---
title: hex2bin
---

Converts a hex-looking string to a byte-array terminated with NULL.

```c
char* hex2bin(char *s)
```

* `s`: a string to operate on; it can optionally contain "0x" prefix.

The result is malloc’ed & should be freed.

See also [bin2hex](#bin2hex).

```c
hex2bin("0x48656c6c6f2c20576f726c6421")         // "Hello, World!"
hex2bin("d0bfd0bed180d0bed0b1d0bbd0b5d0bdd0be") // "пороблено"
```

```c
<%= lines 'hex.c', '}', 'void hex\(\) {' %>
```
