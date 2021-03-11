#include "utils.h"
#include <time.h>

int rand_range(int min, int max) { return random() % (max - min + 1) + min; }

#define mk_shuffle2_fn(name, type)              \
  void shuffle2_##name(type* arr, int len) {    \
    if ( !(arr && len)) return;                 \
    for (int idx = 0; idx < len; idx++) {       \
      int rnd_idx = rand_range(0, idx);         \
      type tmp = arr[rnd_idx];                  \
      arr[rnd_idx] = arr[idx];                  \
      arr[idx] = tmp;                           \
    }                                           \
  }

#define mk_shuffle_fn(name, type, terminator)           \
  type* shuffle_##name(const type* arr, int len) {      \
    if ( !(arr && len)) return NULL;                    \
    type* copy = (type*)malloc((len+1)*sizeof(type));   \
    int size = len;                                     \
    while (size--) copy[size] = (type)arr[size];        \
    shuffle2_##name(copy, len);                         \
    copy[len] = terminator;                             \
    return copy;                                        \
  }

// a helper
#define arr_print(spec, arr, len) ({                                    \
      int idx = 0;                                                      \
      for (; idx < len; idx++)                                          \
        fprintf(stderr, (idx ? ", " spec : "[" spec), arr[idx]);        \
      fprintf(stderr, "%s\n", idx ? "]" : "[]");                        \
    })

mk_shuffle2_fn(ints, int);
mk_shuffle2_fn(chars, char);
mk_shuffle2_fn(list, char*);

mk_shuffle_fn(ints, int, 0);
mk_shuffle_fn(chars, char, '\0');
mk_shuffle_fn(list, char*, NULL);

/*
  #define shuffle2(type, arr, len)                \
  for (int idx = 0; idx < len; idx++) {         \
  int rnd_idx = rand_range(0, idx);           \
  type tmp = arr[rnd_idx];                    \
  arr[rnd_idx] = arr[idx];                    \
  arr[idx] = tmp;                             \
  }


  #define shuffle(type, arr, len) ({                      \
  type* copy = (type*)malloc(len*sizeof(type));     \
  int size = len;                                   \
  while (size--) copy[size] = (type)arr[size];      \
  shuffle2(type, copy, len);                        \
  copy;                                             \
  })
*/

void shuffle_array() {
  srandom(time(0));
  typedef const char *list[];

  int q[] = {1,2,3,4,5};
  int *q_copy = shuffle_ints(q, 5);
  arr_print("%d", q, 5);
  arr_print("%d", q_copy, 5);
  free(q_copy);

  char s[] = "qwert___";
  arr_print("%c", s, strlen(s));
  shuffle2_chars(s, 5);
  arr_print("%c", s, strlen(s));

  list l1 = {"qq", "ww", "ee", "rr", "tt", NULL};
  test(NULL == shuffle_list(NULL, 123));
  test(NULL == shuffle_list(l1, 0));
  char **l1_copy = shuffle_list(l1, list_len(l1));
  list_print(l1_copy);
  test_listeq(l1, ((list){"qq", "ww", "ee", "rr", "tt", NULL}));
}

int main() {
  srandom(time(0));
  const char *q[] = {"foo", "bar", "baz", "qux", "quux", "quuz", NULL};
  list_print(shuffle_list(q, list_len(q)));
}
