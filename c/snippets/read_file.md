---
title: Read a file into memory
---

Reads contents of a file into a string.

```c
LargeFile read_file(const char *name);

<%= lines 'read_file.c', '#include <fcntl.h>', 'char\* _read_file\(int fd, int \*len\) {' %>
```

* `name`: a file name or "-" for stdin.

`LargeFile.size` is a size of _read_ bytes. On error, it'll be be -1.
`LargeFile.data` is a null-terminated string that is malloc'ed and
should be freed.

~~~
[cling]$ LargeFile r = read_file("/usr/share/dict/words")
(LargeFile &) @0x7f58aa2d4000
[cling]$ r.size
(int) 4953598
[cling]$ digest_hex("sha1", r.data)
(char *) "404721f760e0acb6b7a725dfad859ab900a2469f"
~~~

```c
<%= lines 'read_file.c', '#include "utils.h"', 'void read_file\(\) {' %>
```
