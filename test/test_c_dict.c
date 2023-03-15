#include <assert.h>
#include <string.h>
#include "c_dict.h"

int main (void) {
  C_ITERATOR *i;
  int count;
  C_DICTITEM *item;

  C_DICT *d = c_dict_create ();
  assert (0 == c_dict_size (d));
  assert (0 == c_dict_add (d, "one", "eleven"));
  assert (1 == c_dict_size (d));
  assert (0 == strcmp (c_dict_find (d, "one"), "eleven"));
  assert (0 != strcmp (c_dict_find (d, "one"), "twelve"));

  c_dict_remove (d, "two");
  assert (1 == c_dict_size (d));
  c_dict_remove (d, "one");
  assert (0 == c_dict_size (d));

  assert (0 == c_dict_add (d, "one", "eleven"));
  assert (0 == c_dict_add (d, "two", "twelve"));
  assert (0 == c_dict_add (d, "three", "thirteen"));
  assert (0 == c_dict_add (d, "four", "fourteen"));
  assert (4 == c_dict_size (d));
  assert (0 == c_dict_add (d, "one", "eleven"));
  assert (4 == c_dict_size (d));

  count = 0;
  i = c_dict_iterator (d);
  while (c_iterator_has_next (i)) {
    count += 1;
    item = (C_DICTITEM *) c_iterator_next (i);
    if (0 == strcmp (item -> value, "thirteen")) {
      assert (0 == strcmp (item -> key, "three"));
      c_iterator_remove (i);
    }
  }
  assert (4 == count);
  assert (3 == c_dict_size (d));
  assert (0 == c_dict_find (d, "three"));

  c_dict_clear (d);
  assert (0 == c_dict_size (d));

  c_dict_free (d);
  return 0;
}
