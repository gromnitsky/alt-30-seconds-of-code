#include "split.c"

typedef struct HTItem {
  char *key;
  void *val;
  HTItem *next;
} HTItem;

typedef struct {
  HTItem **buckets;
  int buckets_len;
  void (*item_val_free)(void*);
} HashTable;

HashTable* mk_hash_table(int buckets_len, void (*item_val_free)(void*)) {
  assert(item_val_free);
  if (buckets_len < 10) buckets_len = 10;

  HashTable* h = (HashTable*)malloc(sizeof(HashTable));
  h->buckets_len = buckets_len;
  h->item_val_free = item_val_free;
  h->buckets = (HTItem**)calloc(h->buckets_len, sizeof(HTItem*));
  return h;
}

// calculate an index for h->buckets
unsigned int ht_hash(HashTable* h, char *key) {
  if (!key) key = "";
  unsigned char hash = 0;
  for (unsigned char *p = (unsigned char*)key; *p; p++)
    hash = 31*hash + *p;
  return hash & h->buckets_len;
}

HTItem* ht_find(HashTable* h, char *key) {
  if (!key) return NULL;
  int idx = ht_hash(h, key);
  for (HTItem *item = h->buckets[idx]; item; item = item->next) {
    if (0 == strcmp(key, item->key)) return item;
  }
  return NULL;
}

void ht_add(HashTable* h, char *key, void *val) {
  HTItem *item = ht_find(h, key);
  if (item) { // update existing value
    h->item_val_free(item->val);
    item->val = val;
    return;
  }

  int idx = ht_hash(h, key);
  item = (HTItem*)malloc(sizeof(HTItem));
  item->key = strdup(key);
  item->val = val;
  item->next = h->buckets[idx]; // injecting in front of existing ones
  h->buckets[idx] = item;
}

void ht_print(HashTable* h) {
  if (!h) return;
  for (int idx = 0; idx < h->buckets_len; idx++) {
    if (!h->buckets[idx]) continue; // don't print an empty bucket
    printf("%2d: ", idx);

    int first = 0;
    for (HTItem *item = h->buckets[idx]; item; item = item->next) {
      printf("%s%s", first++ ? " -> " : "", item->key);
    }
    printf("\n");
  }
}

void ht_rm(HashTable* h, char *key) {
  if (!ht_find(h, key)) return;

  for (int idx = 0; idx < h->buckets_len; idx++) {
    HTItem *prev = NULL;
    for (HTItem *item = h->buckets[idx]; item; item = item->next) {
      if (0 == strcmp(item->key, key)) {
        if (prev) { // N: item1 -> WE_WERE_HERE -> item3
          prev->next = item->next;
        } else {    // N: WE_WERE_HERE -> item2
          h->buckets[idx] = item->next;
        }
        free(item->key);
        h->item_val_free(item->val);
        free(item);
        return;
      }
      prev = item;
    }
  }
}

void ht_free(HashTable** ht) {
  HashTable *h = *ht;
  for (int idx = 0; idx < h->buckets_len; idx++) {
    for (HTItem *item = h->buckets[idx]; item; item = item->next) {
      free(item->key);
      h->item_val_free(item->val);
      free(item);
    }
    h->buckets[idx] = NULL;
  }

  free(h->buckets);
  free(*ht);
  *ht = NULL;
}

void ht_walk(HashTable* h, void (*callback)(HTItem *, void *), void *data) {
  assert(callback);
  for (int idx = 0; idx < h->buckets_len; idx++) {
    for (HTItem *item = h->buckets[idx]; item; item = item->next)
      callback(item, data);
  }
}

void _ht_total(HTItem *_, void *data) { int *n = (int*)data; (*n)++; }

int ht_total(HashTable *ht) {
  int len = 0;
  ht_walk(ht, _ht_total, &len);
  return len;
}

typedef struct {
  char **keys;
  int idx;
} _HTKeys;

void _ht_keys(HTItem *item, void *data) {
  _HTKeys *hk = (_HTKeys*)data;
  hk->keys[hk->idx++] = item->key;
}

char **ht_keys(HashTable *ht) {
  int len = ht_total(ht);
  if (!len) return NULL;

  _HTKeys r = { .idx = 0 };
  r.keys = (char**)malloc((len+1)*sizeof(char*));
  ht_walk(ht, _ht_keys, &r);
  r.keys[len] = NULL;
  return r.keys;
}


// main
typedef struct {
  int count;
} Counter;

Counter* mk_word() {
  Counter *w = (Counter*)malloc(sizeof(Counter));
  w->count = 1;
  return w;
}

void val_free(void *obj) { free(obj); }

void words_print(HTItem *item, void *_) {
  printf("%d %s\n", ((Counter*)(item->val))->count, item->key);
}

void hash_table() {
  char *input = "bad news is come to town, bad news is carry'd, "
    "bad news is come to town, my love is marry'd";
  char **words = split("[ ,]+", input);

  HashTable *ht = mk_hash_table(42, val_free);
  for (char **key = words; *key; *key++) {
    HTItem *item = ht_find(ht, *key);
    if (item) {
      ((Counter*)(item->val))->count++;
      continue;
    }

    Counter *val = mk_word();
    ht_add(ht, *key, val);
  }
  free(words);

  ht_print(ht);

  puts("");
  ht_walk(ht, words_print, NULL);
  printf("total = %d\n", ht_total(ht));
  list_print(ht_keys(ht));

  ht_rm(ht, "my"); ht_rm(ht, "bad"); ht_rm(ht, "news"); ht_rm(ht, "no such key");
  puts("");
  ht_walk(ht, words_print, NULL);

  puts("");
  ht_print(ht);

  ht_free(&ht);
  assert(NULL == ht);
}
