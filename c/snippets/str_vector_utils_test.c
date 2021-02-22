#include "utils.h"

void str_vector_utils_test() {
  typedef char *list[];

  char *orig1[] = { "foo", "bar", "baz", NULL };
  char *orig2[] = { "foo", "bar", "baz", NULL };
  test_listeq(list_scp(orig1), orig1);
  test_listeq(orig1, orig2);

  char *a1[] = {"bar", "eggplant", "gate", "car", "door", "apple", "fork", NULL};
  char *a2[] = {"bar", "eggplant", "gate", "car", "door", "apple", "fork", NULL};
  char **sorted = list_sort(a1);
  test_listeq(a1, a2);
  test_listeq(sorted, ((list){"apple", "bar", "car", "door", "eggplant", "fork", "gate", NULL}));
}
