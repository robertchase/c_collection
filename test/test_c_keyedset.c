#include <assert.h>
#include <string.h>

#include "c_keyedset.h"
#include "c_iterator.h"
#include "c_map.h"

int main (int argc, char **argv) {

  char *a = "hello";
  char *b = "world";

  C_KEYEDSET *k = c_keyedset_create ();
  assert (k);
  assert (0 == c_keyedset_size (k));
  assert (1 == c_keyedset_add (k, a));
  assert (0 == strcmp ((char *) c_keyedset_get (k, 1), a));
  assert (2 == c_keyedset_add (k, b));
  assert (0 == strcmp ((char *) c_keyedset_get (k, 2), b));
  assert (3 == c_keyedset_add (k, a));
  assert (0 == strcmp ((char *) c_keyedset_get (k, 1), a));
  assert (0 == strcmp ((char *) c_keyedset_get (k, 3), a));
  assert (3 == c_keyedset_size (k));
  
  C_ITERATOR *it = c_keyedset_iterator (k);
  int i = 0;
  while (c_iterator_has_next (it)) {
    char *value = (char *) c_iterator_next (it);
    if (0 == strcmp (value, b)) c_iterator_remove (it);
    i += 1;
  }
  assert (3 == i);
  assert (2 == c_keyedset_size (k));
  assert (0 == strcmp ((char *) c_keyedset_get (k, 1), a));
  assert (NULL == c_keyedset_get (k, 2));
  assert (0 == strcmp ((char *) c_keyedset_get (k, 3), a));
  
  c_keyedset_remove (k, 1);
  assert (1 == c_keyedset_size (k));
  assert (NULL == c_keyedset_get (k, 1));
  assert (NULL == c_keyedset_get (k, 2));
  assert (0 == strcmp ((char *) c_keyedset_get (k, 3), a));
  c_keyedset_free (k);

  return 0;
}
