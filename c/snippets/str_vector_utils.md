---
title: String vector utils
---

Handy funtions for arrays of strings:

```c
void list_print(char **list);
int list_len(char **list);
void list_free(char ***v);
```

* `list`: a pointer to an array of character strings (char\*)
  terminated by NULL, e.g. `["foo", "bar", NULL]`;

```
[cling]$ char **r = split(" +", "abc    def ghi")
(char **) 0x5c8cc50
[cling]$ list_print(r)
["abc", "def", "ghi"]
[cling]$ list_len(r)
(int) 3
[cling]$ list_free(&r)
[cling]$ list_print(r)
(null)
```

Used by many snippets, for example, [split](#split) or [Hash
table](#hash_table).

```c
<%= File.read 'str_vector_utils.c' %>
```
