#include "split.c"
#include "utils.h"

typedef struct Item {
  char *key;
  void *val;
  Item *next;
} Item;

typedef struct {
  Item **buckets;
  int buckets_len;
} HashTable;

HashTable* mk_hash_table(int buckets_len) {
  HashTable* ht = (HashTable*)malloc(sizeof(HashTable));
  ht->buckets_len = buckets_len;
  ht->buckets = (Item**)calloc(ht->buckets_len, sizeof(Item*));
  return ht;
}

unsigned int ht_hash(HashTable* ht, char *key) {
  if (!key) key = "";
  unsigned char h = 0;
  for (unsigned char *p = (unsigned char*)key; *p; p++)
    h = 31*h + *p;
  return h & ht->buckets_len;
}

Item* ht_find(HashTable* ht, char *key) {
  if (!key) return NULL;
  int idx = ht_hash(ht, key);
  for (Item *item = ht->buckets[idx]; item; item = item->next) {
    if (0 == strcmp(key, item->key)) return item;
  }
  return NULL;
}

void ht_add(HashTable* ht, void (*val_free_callback)(void*),
            char *key, void *val) {
  assert(val_free_callback);
  Item *item = ht_find(ht, key);
  if (item) { // update existing value
    val_free_callback(item->val);
    item->val = val;
    return;
  }

  int idx = ht_hash(ht, key);
  item = (Item*)malloc(sizeof(Item));
  item->key = strdup(key);
  item->val = val;
  item->next = ht->buckets[idx]; // injecting in front of existing ones
  ht->buckets[idx] = item;
}

void ht_print(HashTable* ht) {
  if (!ht) return;
  for (int idx = 0; idx < ht->buckets_len; idx++) {
    if (!ht->buckets[idx]) continue; // don't print an empty bucket
    printf("%2d: ", idx);

    int first = 0;
    for (Item *item = ht->buckets[idx]; item; item = item->next) {
      printf("%s%s", first++ ? " -> " : "", item->key);
    }
    printf("\n");
  }
}

void ht_rm(HashTable* ht, void (*val_free_callback)(void*), char *key) {
  assert(val_free_callback);
  if (!ht_find(ht, key)) return;

  for (int idx = 0; idx < ht->buckets_len; idx++) {
    Item *prev = NULL;
    for (Item *item = ht->buckets[idx]; item; item = item->next) {
      if (0 == strcmp(item->key, key)) {
        if (prev) { // N: item1 -> WE_WERE_HERE -> item3
          prev->next = item->next;
        } else {    // N: WE_WERE_HERE -> item2
          ht->buckets[idx] = item->next;
        }
        free(item->key);
        val_free_callback(item->val);
        free(item);
        return;
      }
      prev = item;
    }
  }
}

void ht_free(HashTable** ht, void (*val_free_callback)(void*)) {
  assert(val_free_callback);
  for (int idx = 0; idx < (*ht)->buckets_len; idx++) {
    for (Item *item = (*ht)->buckets[idx]; item; item = item->next) {
      free(item->key);
      val_free_callback(item->val);
      free(item);
    }
    (*ht)->buckets[idx] = NULL;
  }

  free((*ht)->buckets);
  free(*ht);
  *ht = NULL;
}

void ht_walk(HashTable* ht, void (*callback)(Item *, void *),
             void *data) {
  assert(callback);
  for (int idx = 0; idx < ht->buckets_len; idx++) {
    for (Item *item = ht->buckets[idx]; item; item = item->next)
      callback(item, data);
  }
}

void _ht_total(Item *_, void *data) { int *n = (int*)data; (*n)++; }

int ht_total(HashTable *ht) {
  int len = 0;
  ht_walk(ht, _ht_total, &len);
  return len;
}

typedef struct {
  char **keys;
  int idx;
} _HT_Keys;

void _ht_keys(Item *item, void *data) {
  _HT_Keys *hk = (_HT_Keys*)data;
  hk->keys[hk->idx++] = item->key;
}

char **ht_keys(HashTable *ht) {
  int len = ht_total(ht);
  if (!len) return NULL;

  _HT_Keys hk = { .idx = 0 };
  hk.keys = (char**)malloc((len+1)*sizeof(char*));
  ht_walk(ht, _ht_keys, &hk);
  hk.keys[len] = NULL;
  return hk.keys;
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

void val_free(void *obj) {
  free(obj);
}

void words_print(Item *item, void *_) {
  printf("%d %s\n", ((Counter*)(item->val))->count, item->key);
}

void hash_table() {
  char *input = "bad news is come to town, bad news is carry'd, "
    "bad news is come to town, my love is marry'd";
  char **words = split("[ ,]+", input);

  HashTable *ht = mk_hash_table(42);
  for (char **key = words; *key; *key++) {
    Item *item = ht_find(ht, *key);
    if (item) {
      ((Counter*)(item->val))->count++;
      continue;
    }

    Counter *val = mk_word();
    ht_add(ht, val_free, *key, val);
  }
  free(words);

  ht_print(ht);

  puts("");
  ht_walk(ht, words_print, NULL);
  printf("total = %d\n", ht_total(ht));
  list_print(ht_keys(ht));

  ht_rm(ht, val_free, "my");
  ht_rm(ht, val_free, "bad");
  ht_rm(ht, val_free, "news");
  ht_rm(ht, val_free, "no such key");
  puts("");
  ht_walk(ht, words_print, NULL);

  puts("");
  ht_print(ht);

  ht_free(&ht, val_free);
  assert(NULL == ht);
}
