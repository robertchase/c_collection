#include <assert.h>
#include <string.h>

#include "c_iterator.h"
#include "c_symbol.h"

int main (int argc, char **argv) {
  C_SYMBOL *s = c_symbol_create ();
  char *symbol;

  assert (s);

  symbol = c_symbol_add (s, "akk");
  assert (0 == strcmp ("akk", symbol));
  assert (symbol == c_symbol_add (s, "akk"));
  assert (1 == c_symbol_size (s));

  assert (c_symbol_add (s, "foo"));
  assert (2 == c_symbol_size (s));

  assert (c_symbol_add (s, "bar"));
  assert (3 == c_symbol_size (s));

  assert (c_symbol_add (s, "foo"));
  assert (3 == c_symbol_size (s));

  C_ITERATOR *it = c_symbol_iterator (s);
  while (c_iterator_has_next (it)) {
    if (0 == strcmp ("foo", c_iterator_next (it))) {
      c_iterator_remove (it);
    }
  }

  assert (2 == c_symbol_size (s));
  assert (NULL == c_symbol_find (s, "foo"));
  assert (c_symbol_add (s, "foo"));
  assert (3 == c_symbol_size (s));
  
  c_symbol_clear (s);
  assert (0 == c_symbol_size (s));

  c_symbol_free (s);
  return 0;
}
