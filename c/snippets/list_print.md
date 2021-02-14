---
title: Print a list
---

A handy debug funtion that can print the results of [split](#split).

    void list_print(char **list)

* `list`: a pointer to an array of character strings (char\*)
  terminated by NULL, e.g. `["foo", "bar", NULL]`;

```
[cling]$ list_print(split(" +", "abc    def ghi"))
["abc", "def", "ghi"]
```

```c
<%= File.read 'list_print.c' %>
```
