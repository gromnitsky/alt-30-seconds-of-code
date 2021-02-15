---
title: uuid
---

Generate an UUID.

```c
char* uuid()
```

The result is malloc’ed & should be freed.

The snipped uses [randstr](#randstr) & requires a seeding.

```c
uuid() // "415a4253-6459-5268-6343-4e6a37334146"
```

```c
<%= lines 'randstr.c', '}', '// no seeding is req' %>
```
