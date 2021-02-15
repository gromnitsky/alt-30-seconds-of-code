---
title: Hash table
---

POSIX has `hsearch()` function for manipulating a hash table, but it's
pretty useless for a user is required to provide a fixed size for the
table that cannot be changed afterwards. There is no way to properly
empty the table or get a list of its elements.

POSIX also has `tsearch()` & co. for manipulating a tree (Red-Black or
AVL, depending on a libc implementation). It has a more useful
interface--the user don't need to worry about a tree size, can delete
individual entries & clean up the whole tree. Nevertheless I still
find it fairly brittle & cumbersome.

For example, memory management is terrifically complex: if, when
adding an entry to a tree you do it twice with the same key, it'll
replace the existing entry (as one would expect), but won't either
free the previous version nor provide a way to pass a callback to do
that automatically.

Deleting an entry removes the reference from the tree, but doesn't
free the memory for the entry itself.

There's no straightforward key/value distinction--you're saving a
chunk of data (a struct, presumably), but must provide a comparator
with which `tsearch()` & co. would fish for keys.

The structure of a tree is opaque: when you search for an entry you
don't get a pointer to it, but get a pointer to the unknown
tree-internal node structure whose first element is a pointer to your
entry. The same goes for a pointer for the tree itself--it's some blob
of memory that you cannot inspect, hence is unable to peek at its
contents.

The only way to list all the elements of the tree is write a callback
for `twalk()`. This would be fine for a language with closures &
lambdas support, like Ruby or JavaScript, but C has none of that
whatsoever. If you want to filter-out some entries your only choice is
global variables.

Anyway, this snippet provides an example of using as `tsearch()` as if
it was a hash table routine:

1. It adds a bunch of words (from a 17th-century English ballad) to a
   tree (each word is a key, its number of occurrences is a value).
2. Then it deletes 2 entries from the tree.
3. Then it destroys the tree completely.

This is the array of words:

```c
  <%= lines 'tsearch.c', 'void tsearch\(\) {', '  void \*tree = NULL;' %>
```

(We're using [split](#split) to create the array from a string.)

This is what you get after running the snippet:

~~~
3 bad
1 carry'd
2 come
4 is
1 love
1 marry'd
1 my
3 news
2 to
2 town

1 carry'd
2 come
4 is
1 love
1 marry'd
1 my
2 to
2 town
~~~

Our data structure, a comparator & an allocator for a new entry:

```c
<%= lines 'tsearch.c', '#include "utils.h"', '// for twalk' %>
```

The main routine:

```c
  <%= lines 'tsearch.c', 'void tsearch\(\) \{', '}' %>
```


The rest of the garbage:

```c
<%= lines 'tsearch.c', '// for twalk', 'void tsearch\(\) {' %>
```
