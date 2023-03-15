#include <assert.h>
#include <string.h>
#include "c_list.h"

int main (void) {
  char * data [] = {"zero", "one", "two", "three", "four", "five", "6", "7", "8", "9", "10", "11", "12", "13"};
  C_LIST *l = c_list_create ();
  C_ITERATOR *i;

  assert (0 == c_list_size (l));

  c_list_add (l, data [1]);
  assert (1 == c_list_size (l));
  i = c_list_iterator (l);
  assert (0 == strcmp ("one", c_iterator_next (i)));
  assert (NULL == c_iterator_next (i));
  assert (0 == c_iterator_has_next (i));

  c_list_add (l, data [2]);
  assert (2 == c_list_size (l));
  i = c_list_iterator (l);
  assert (0 == strcmp ("one", c_iterator_next (i)));
  assert (0 == strcmp ("two", c_iterator_next (i)));
  assert (0 == c_iterator_has_next (i));

  c_list_add_first (l, data [0]);
  assert (3 == c_list_size (l));
  c_iterator_reset (i);
  assert (0 == strcmp ("zero", c_iterator_next (i)));
  assert (0 == strcmp ("one", c_iterator_next (i)));
  assert (0 == strcmp ("two", c_iterator_next (i)));
  assert (0 == c_iterator_has_next (i));

  assert (0 == strcmp ("zero", c_list_take (l)));
  assert (2 == c_list_size (l));
  c_iterator_reset (i);
  assert (0 == strcmp ("one", c_iterator_next (i)));
  assert (0 == strcmp ("two", c_iterator_next (i)));
  assert (0 == c_iterator_has_next (i));

  assert (0 == strcmp ("two", c_list_take_last (l)));
  assert (1 == c_list_size (l));
  c_iterator_reset (i);
  assert (0 == strcmp ("one", c_iterator_next (i)));
  assert (0 == c_iterator_has_next (i));

  assert (0 == strcmp ("one", c_list_take (l)));
  assert (0 == c_list_size (l));
  c_iterator_reset (i);
  assert (0 == c_iterator_has_next (i));

  c_list_add (l, data [0]);
  c_list_add (l, data [1]);
  c_list_add (l, data [2]);
  c_list_add (l, data [3]);
  assert (4 == c_list_size (l));
  c_iterator_reset (i);
  assert (0 == strcmp ("zero", c_iterator_next (i)));
  assert (0 == strcmp ("one", c_iterator_next (i)));
  assert (0 == strcmp ("two", c_iterator_next (i)));
  assert (0 == strcmp ("three", c_iterator_next (i)));
  assert (0 == c_iterator_has_next (i));

  assert (0 == strcmp ("zero", c_list_take (l)));
  assert (0 == strcmp ("one", c_list_take (l)));
  assert (0 == strcmp ("two", c_list_take (l)));
  assert (0 == strcmp ("three", c_list_take (l)));
  assert (NULL == c_list_take (l));
  assert (0 == c_list_size (l));

  c_list_add (l, data [0]);
  c_list_add (l, data [1]);
  c_list_add (l, data [2]);
  c_list_add (l, data [3]);
  c_list_add (l, data [4]);
  c_list_add (l, data [5]);
  c_list_add (l, data [6]);
  c_list_add (l, data [7]);
  c_list_add (l, data [8]);
  c_list_add (l, data [9]);
  c_list_add (l, data [10]);
  c_list_add (l, data [11]);
  c_list_add (l, data [12]);
  c_list_add (l, data [13]);
  assert (14 == c_list_size (l));

  c_list_free (l);
  return 0;
}
