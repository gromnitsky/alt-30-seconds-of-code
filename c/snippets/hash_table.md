---
title: Hash table
---

A small API for classic hash tables. In contrast with POSIX
`hsearch()` & co, you can query with it all HT entries, get a list of
its keys, size, remove individual entries, provide a callback to a
"walk" function, destroy the HT with all its memory automatically
freed.

If you still want to use POSIX facilities, see [tsearch](#tsearch)
example.

```c
<%= lines 'hash_table.c', '#include "split.c"', 'HashTable\* mk_hash_table\(int buckets_len, void \(\*item_val_free\)\(void\*\)\) {' %>

HashTable* mk_hash_table(int buckets_len, void (*item_val_free)(void*));
HTItem* ht_find(HashTable* h, const char *key);
void ht_add(HashTable* h, const char *key, void *val);
void ht_rm(HashTable* h, const char *key);
void ht_free(HashTable** ht);
int ht_total(HashTable *ht);
char **ht_keys(HashTable *ht);
void ht_walk(HashTable* h, void (*callback)(HTItem *, void *), void *data);
void ht_print(HashTable* h);
```

Start with `mk_hash_table()`. You pass its return value to all other
HT functions.

* `buckets_len` is NOT the max length of HT entries but a number of
  *buckets*. Internally, each bucket is a linked list, so when you
  search through HT via `ht_find()`, it first calculates a *hash* (an
  index in `HashTable->buckets` array) from the name of a
  *key*. Hence, for big hash tables, if you have only a few buckets
  available, each bucket will contain a long list of entries; this is,
  obviously, slows down HT operations.

* `item_val_free`: a callback to a function that will release memory
  for `HTItem->val`.

Keys in a HT are always null-terminated strings that are *copied*
inside the HT. Values should be dynamically allocated before adding a
key-value pair to the HT.

If you add an entry with a key that already exists in a HT,
`ht_add()` function automatically releases memory for the previous
value & injects in its place a new one.

When you remove an entry via `ht_rm()`, all the memory is
automatically released.

The same goes for `ht_free()`--all memory is freed, the HT is
completely wiped out & `HashTable` pointer that was returned from
`mk_hash_table()` becomes nil.

`ht_total()` returns the current HT size.

`ht_keys()` is analogues to JavaScript `Object.keys()`; it returns an
array of strings. You should free it aftewards, but do NOT free the
individual strings from it, for they point to actual keys in a HT.

`ht_walk()` is a helper that runs a callbalk for every entry in a
HT. You can pass additional data to it, that is in turn passed to
the callbalk. `ht_total()` & `ht_keys()` are implemented using
`ht_walk()` & use this technique.

`ht_print` is a debug function that you may use to visualize the state
of a HT.

In the example below we:

1. Add a bunch of words (each word is a key, its number of
   occurrences is a value) to a HT.
2. Remove 3 entries from the HT.
3. Destroy the HT.

This is what you get after running it:

~~~
 0: come -> bad
 2: marry'd -> town -> news
34: love
40: my -> carry'd

2 come
3 bad
1 marry'd
2 town
3 news
1 love
1 my
1 carry'd
total = 8
["come", "bad", "marry'd", "town", "news", "love", "my", "carry'd"]

2 come
1 marry'd
2 town
1 love
1 carry'd

 0: come
 2: marry'd -> town
34: love
40: carry'd
~~~

The 1st & the last blocks are `ht_print()` (left column is a bucket
number, right--linked lists; empty buckets are not shown). In the
middle there is an example of a callback to `ht_walk()`:

```c
<%= lines 'hash_table.c', '// main', 'void hash_table\(\) {' %>
```

Main (it uses [split](#split) & [list_print](#list_print)):

```c
  <%= lines 'hash_table.c', 'void hash_table\(\) {', '}' %>
```

The API the code above uses:

```c
<%= lines 'hash_table.c', '#include "split.c"', '// main' %>
```

One way to enhance this would be moving `item_val_free` callback from
`mk_hash_table()` to `ht_add()`--that way you may potentially have
totally different types of `val`, all with a custom `free()` function.
