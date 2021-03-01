---
title: Transform string
---

Apply a function to each char of a string.

```c
char* str_tr(const char *s, int (*fn)(int));
char* str_tr2(char *s, int (*fn)(int));
```

* `s`: a string to operate on;
* `fn`: a function to apply, for example, tolower(3) or `rot13()`.

`str_tr()` returns a malloc'ed value that should be freed. `str_tr2()`
does transformations *in-place* & returns a pointer to a modified
string.

```c
str_tr("qwerty", toupper) // "QWERTY"

[cling]$ char s[] = "qwerty"
(char [7]) "qwerty"
[cling]$ str_tr2(s, toupper)
(char [7]) "QWERTY"
[cling]$ str_tr("Oof Madon, what a beating we gave this poor prick!", rot13)
(char *) "Bbs Znqba, jung n orngvat jr tnir guvf cbbe cevpx!"
```

```c
<%= lines 'str_tr.c', '#include "utils.h"', 'void str_tr\(\) {' %>
```
