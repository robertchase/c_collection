#include <assert.h>
#include <string.h>
#include "c_map.h"
#include "hash_func.h"

int main (void) {
  C_ITERATOR *i;
  int count;
  C_MAPITEM *item;
  char *name [] = {"one", "two", "three", "four"};
  char *value [] = {"eleven", "twelve", "thirteen", "fourteen"};

  C_MAP *m = c_map_dict_create (0);
  assert (0 == c_map_size (m));
  assert (0 == c_map_add (m, name [0], value [0]));
  assert (1 == c_map_size (m));
  assert (0 == strcmp (c_map_find (m, "one"), "eleven"));
  assert (0 != strcmp (c_map_find (m, "one"), "twelve"));

  c_map_remove (m, "two");
  assert (1 == c_map_size (m));
  c_map_remove (m, "one");
  assert (0 == c_map_size (m));

  assert (0 == c_map_add (m, name [0], value [0]));
  assert (0 == c_map_add (m, name [1], value [1]));
  assert (0 == c_map_add (m, name [2], value [2]));
  assert (0 == c_map_add (m, name [3], value [3]));
  assert (4 == c_map_size (m));
  assert (0 == c_map_add (m, name [0], value [0]));
  assert (4 == c_map_size (m));

  count = 0;
  i = c_map_iterator (m);
  while (c_iterator_has_next (i)) {
    count += 1;
    item = (C_MAPITEM *) c_iterator_next (i);
    if (0 == strcmp (item -> value, "thirteen")) {
      assert (0 == strcmp (item -> key, "three"));
      c_iterator_remove (i);
    }
  }
  assert (4 == count);
  assert (3 == c_map_size (m));
  assert (0 == c_map_find_key (m, "three"));
  assert (0 == c_map_exists (m, "three"));
  assert (1 == c_map_exists (m, "one"));

  c_map_clear (m);
  assert (0 == c_map_size (m));

  c_map_free (m);
  return 0;
}
