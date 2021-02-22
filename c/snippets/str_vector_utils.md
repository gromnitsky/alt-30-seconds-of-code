---
title: String vector utils
---

Handy funtions for arrays of strings:

```c
void list_print(char **list);
int list_len(char **list);
void list_free(char ***v);
char** list_scp(char **list);
char** list_sort(char **list);
```

* `list`: a pointer to an array of character strings (char\*)
  terminated by NULL, e.g. `["foo", "bar", NULL]`;

`list_scp()` makes a *shallow* copy of an array.

`list_sort()` returns a new sorted array.

The return values of the last 2 functions are malloc'ed & should be
freed with regular free(3), NOT with `list_free()`.

```
[cling]$ char **orig = split(" +", "facebook  google apple")
(char **) 0x432fc20
[cling]$ list_print(orig)
["facebook", "google", "apple"]
[cling]$ char **sorted = list_sort(orig)
(char **) 0x4d044c0
[cling]$ list_print(sorted)
["apple", "facebook", "google"]
[cling]$ list_print(orig)
["facebook", "google", "apple"]
[cling]$ list_free(&sorted)
[cling]$ list_print(sorted)
(null)
[cling]$ list_print(orig) // garbage in output, "sorted" was a shallow copy
["Ҟ�", "���", "T��"]
```

Used by many snippets, for example, [split](#split) or [Hash
table](#hash_table).

```c
<%= File.read 'str_vector_utils.c' %>
```
