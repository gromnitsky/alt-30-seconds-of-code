---
title: Shuffle array
---

A couple of macros that *generate* shuffle functions for specific array
types.

```c
mk_shuffle2_fn(name, type) // void shuffle2_name(type* arr, int len);

mk_shuffle_fn(name, type, terminator)
// type* shuffle_name(const type* arr, int len);
```

* `name`: an arbitual suffix for a function name, e.g. `numbers`.
* `type`: a type of an array element, e.g. for an array of strings it
  would be `char*`.
* `terminator`: a value that will be appended to the end of an array,
  e.g. NAN, 0 or NULL.

`mk_shuffle2_fn` generates a function that modifies array
*in-place*. `mk_shuffle_fn` generates a function that makes a shallow
copy of an input array first, then modifies the copy & returns it; its
result should be freed.

Both generated functions require seeding with srandom(3) during a
program initialisation somewhere in main().

A yield from `mk_shuffle_fn2` can be used separately, but
`mk_shuffle_fn` requires `mk_shuffle_fn2`, e.g., to generate a
function named `shuffle_list` that shuffles an array of strings:

```c
#include "shuffle_array.c"

mk_shuffle2_fn(list, char*);
mk_shuffle_fn(list, char*, NULL);

int main() {
  <%= lines 'shuffle_array.c', 'int main\(\) {', '}' %>
  // prints ["quuz", "bar", "foo", "baz", "quux", "corge", "qux"]
}
```

The example uses [list_len & list_print](str_vector_utils).

```c
<%= lines 'shuffle_array.c', '#include "utils.h"', '// a helper' %>
```
