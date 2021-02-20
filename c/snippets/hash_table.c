#include "split.c"
#include "utils.h"

typedef struct Item {
  char *key;
  void *val;
  Item *next;
} Item;

const int BUCKETS_LEN = 42;
Item *buckets[BUCKETS_LEN];

unsigned int hash(char *key) {
  if (!key) key = "";
  unsigned char h = 0;
  for (unsigned char *p = (unsigned char*)key; *p; p++)
    h = 31*h + *p;
  return h & BUCKETS_LEN;
}

Item* find(Item **buckets, char *key) {
  if (!key) return NULL;
  int idx = hash(key);
  for (Item *item = buckets[idx]; item; item = item->next) {
    if (0 == strcmp(key, item->key)) return item;
  }
  return NULL;
}

void add(Item **buckets, void (*val_free_callback)(void*),
         char *key, void *val) {
  assert(val_free_callback);
  Item *item = find(buckets, key);
  if (item) { // update existing value
    val_free_callback(item->val);
    item->val = val;
    return;
  }

  int idx = hash(key);
  item = (Item*)malloc(sizeof(Item));
  item->key = strdup(key);
  item->val = val;
  item->next = buckets[idx]; // injecting in front of existing ones
  buckets[idx] = item;
}

void print(Item **buckets) {
  for (int idx = 0; idx < BUCKETS_LEN; idx++) {
    if (!buckets[idx]) continue; // don't print an empty bucket
    printf("%2d: ", idx);

    int first = 0;
    for (Item *item = buckets[idx]; item; item = item->next) {
      printf("%s%s", first++ ? " -> " : "", item->key);
    }
    printf("\n");
  }
}

void rm(Item **buckets, void (*val_free_callback)(void*), char *key) {
  assert(val_free_callback);
  if (!find(buckets, key)) return;

  for (int idx = 0; idx < BUCKETS_LEN; idx++) {
    Item *prev = NULL;
    for (Item *item = buckets[idx]; item; item = item->next) {
      if (0 == strcmp(item->key, key)) {
        if (prev) { // N: item1 -> WE_WERE_HERE -> item3
          prev->next = item->next;
        } else {    // N: WE_WERE_HERE -> item2
          buckets[idx] = item->next;
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

void clean(Item **buckets, void (*val_free_callback)(void*)) {
  assert(val_free_callback);
  for (int idx = 0; idx < BUCKETS_LEN; idx++) {
    for (Item *item = buckets[idx]; item; item = item->next) {
      free(item->key);
      val_free_callback(item->val);
      free(item);
    }
    buckets[idx] = NULL;
  }
}

void walk(Item **buckets, void (*callback)(Item *item)) {
  assert(callback);
  for (int idx = 0; idx < BUCKETS_LEN; idx++) {
    for (Item *item = buckets[idx]; item; item = item->next) callback(item);
  }
}


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

void print_words(Item *item) {
  printf("%d %s\n", ((Counter*)(item->val))->count, item->key);
}

void hash_table() {
  char *input = "bad news is come to town, bad news is carry'd, "
    "bad news is come to town, my love is marry'd";
  char **words = split("[ ,]+", input);
  for (char **key = words; *key; *key++) {
    Item *item = find(buckets, *key);
    if (item) {
      ((Counter*)(item->val))->count++;
      continue;
    }

    Counter *val = mk_word();
    add(buckets, val_free, *key, val);
  }
  free(words);

  print(buckets);

  puts("");
  walk(buckets, print_words);

  rm(buckets, val_free, "my");
  rm(buckets, val_free, "bad");
  rm(buckets, val_free, "news");
  rm(buckets, val_free, "no such key");
  puts("");
  walk(buckets, print_words);

  puts("");
  print(buckets);

  clean(buckets, val_free);

  puts("");
  print(buckets);
}
