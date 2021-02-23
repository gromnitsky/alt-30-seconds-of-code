#include "split.c"

typedef struct HTItem {
  char *key;
  void *val;
  HTItem *next; // private
} HTItem;

typedef struct {
  HTItem **buckets;
  int buckets_len;
  void (*item_val_free)(void*); // private
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
unsigned int ht_hash(HashTable* h, const char *key) {
  if (!key) key = "";
  unsigned char hash = 0;
  for (unsigned char *p = (unsigned char*)key; *p; p++)
    hash = 31*hash + *p;
  return hash & h->buckets_len;
}

HTItem* ht_find(HashTable* h, const char *key) {
  if (!key) return NULL;
  int idx = ht_hash(h, key);
  for (HTItem *item = h->buckets[idx]; item; item = item->next) {
    if (0 == strcmp(key, item->key)) return item;
  }
  return NULL;
}

void ht_add(HashTable* h, const char *key, void *val) {
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

void ht_rm(HashTable* h, const char *key) {
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

int ht_total(HashTable *h) {
  int len = 0;
  ht_walk(h, _ht_total, &len);
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

char **ht_keys(HashTable *h) {
  int len = ht_total(h);
  if (!len) return NULL;

  _HTKeys r = { .idx = 0 };
  r.keys = (char**)malloc((len+1)*sizeof(char*));
  ht_walk(h, _ht_keys, &r);
  r.keys[len] = NULL;
  return r.keys;
}


// main
void words_print(HTItem *item, void *_) {
  printf("%d %s\n", *(int*)item->val, item->key);
}

void hash_table() {
  const char *input = "bad news is come to town, bad news is carry'd, "
    "bad news is come to town, my love is marry'd";
  char **words = split("[ ,]+", input);

  HashTable *ht = mk_hash_table(42, free);
  for (char **key = words; *key; key++) {
    HTItem *item = ht_find(ht, *key);
    if (item) {
      int *n = (int*)item->val; (*n)++; // inc the word counter
    } else {
      int *n = (int*)malloc(sizeof(int)); *n = 1;
      ht_add(ht, *key, n);
    }
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
