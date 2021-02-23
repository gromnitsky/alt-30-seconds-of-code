---
title: hex2bin
---

Converts a hex-looking string to a byte-array terminated with NULL.

```c
Hex2bin hex2bin(const char *s);

<%= lines 'hex.c', '}', 'Hex2bin hex2bin\(const char \*s\) {' %>
```

* `s`: a string to operate on; it can optionally contain "0x" prefix.

We can't just return *char\**, for unhex values may contain null chars.

`Hex2bin.len` on error becomes -1. `Hex2bin.bytes` is malloc’ed &
should be freed.

See also [bin2hex](#bin2hex).

```c
[cling]$ Hex2bin r = hex2bin("0x48656c6c6f2c20576f726c6421")
(Hex2bin &) @0x7fbbefb2f018
[cling]$ 0 == strncmp(r.bytes, "Hello, World!", r.len)
(bool) true
[cling]$ r.bytes[r.len] = '\0'
(char) '0x00'
[cling]$ r.bytes
(char *) "Hello, World!"
```

```c
<%= lines 'hex.c', '}', 'void hex\(\) {' %>
```
