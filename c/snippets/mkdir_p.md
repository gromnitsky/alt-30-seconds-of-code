---
title: mkdir -p
---

Creates a directory & all its parent directories.

```c
bool mkdir_p(const char *s, mode_t mode);
```

The signature is equal to mkdir(2).

* `s`: a pathname;
* `mode`: permission bits for directories function creates.

Returns true if all components of `s` were created successfully or
were present already.

```c
[cling]$ mkdir_p("./foo/bar/../baz", 0755)
(bool) true
[cling]$ system("tree foo --noreport")
foo
├── bar
└── baz
(int) 0
[cling]$ mkdir_p("/usr/bin", 0755)
(bool) true
[cling]$ mkdir_p("/root/foo", 0755)
(bool) false
```

In the last example we didn't have rights to write into `/root` directory.

```c
<%= lines 'mkdir_p.c', '#include <unistd.h>', 'void mkdir_p\(\) {' %>
```
