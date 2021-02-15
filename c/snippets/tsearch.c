// Why not hsearch?
//
// hsearch is pretty useless because one must provide a fixed size for
// the table (which cannot be increased). And there is no way to empty
// the table.

#include "split.c"
#include "utils.h"

#include <search.h>

typedef struct {
  char word[50];
  int count;
} WC;

int cmp(const void *a, const void *b) {
  const WC *pa = (WC*)a;
  const WC *pb = (WC*)b;
  return strcmp(pa->word, pb->word);
}

WC *mk_entry(char *name) {
  WC *d = (WC*)malloc(sizeof(WC));
  d->count = 1;
  snprintf(d->word, sizeof(d->word), "%s", name);
  return d;
}

// for twalk
void entry_print(const void *nodep, VISIT v, int _) {
   WC *wc = *(WC **)nodep;
   if (v == postorder || v == leaf) printf("%d %s\n", wc->count, wc->word);
}

void trm(void **tree, char *key) {
  WC *node = mk_entry(key);
  void *r = tfind(node, tree, cmp);
  if (r) {
    WC *v = *(WC**)r;
    // tdelete only removes the reference from the tree, it does NOT
    // free the memory for the entry itself
    tdelete(node, tree, cmp);
    free(v);
  }
  free(node);
}

// used with tdelete() when no actual comparison is necessary
int cmp_tfree(const void *a, const void *b) { return 0; }

void tfree(void **tree) {
  while (*tree != NULL) {
    WC *node = *(WC **)*tree; // ain't it pretty?
    //printf("deleting node: word=%s, count=%d\n", node->word, node->count);
    tdelete(node, tree, cmp_tfree);
    free(node);
  }
}

void tsearch() {
  char *input = "bad news is come to town, bad news is carry'd, "
    "bad news is come to town, my love is marry'd";
  char **words = split("[ ,]+", input);

  void *tree = NULL;
  for (char **p = words; *p; *p++) {
    WC *entry = mk_entry(*p);
    void *r = tfind(entry, &tree, cmp);
    if (r) {
      free(entry); // the entry with the _same_ key already exists
      // the return value from tfind is NOT the entry, it's a pointer
      // to the tree-internal node structure whose first element is a
      // pointer to the entry
      WC *found = *(WC**)r; // everybody loves C
      found->count++;
      continue;
    }
    // we can't free `entry` immediately afterwards for tsearch saves
    // a pointer to it--`entry` must live as long `tree` lives
    assert(tsearch(entry, &tree, cmp));
  }
  free(words);
  twalk(tree, entry_print);

  // remove 2 entries
  trm(&tree, "bad"); trm(&tree, "news"); trm(&tree, "no such entry");
  puts("");
  twalk(tree, entry_print);

  // remove all, `tree` is going to be NULL aftewards
  tfree(&tree);
  assert(NULL == tree);
}
