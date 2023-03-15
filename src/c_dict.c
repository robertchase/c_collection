#include <stdlib.h>
#include <string.h>

#include "c_map.h"
#include "c_symbol.h"
#include "c_dict.h"

struct C_DICT {
  C_SYMBOL *symbols;
  C_MAP *dict;
};

C_DICT *
c_dict_create (void) {

  C_DICT *d = (C_DICT *) malloc (sizeof (C_DICT));
  if (d) {
    memset (d, 0x00, sizeof (C_DICT));
    d -> symbols = c_symbol_create ();
    if (!d -> symbols) {
      free (d);
      d = NULL;
    } else {
      d -> dict = c_map_dict_create (NULL);
      if (!d -> dict) {
        c_symbol_free (d -> symbols);
        d = NULL;
      }
    }
  }

  return d;
}

void
c_dict_free (C_DICT *d) {
  if (d) {
    c_symbol_free (d -> symbols);
    c_map_free (d -> dict);
    free (d);
  }
}

void
c_dict_clear (C_DICT *d) {
  c_symbol_clear (d -> symbols);
  c_map_clear (d -> dict);
}

int
c_dict_add (C_DICT *d, char *key, char *value) {

  if (NULL == key) return -1;
  if (NULL == value) return -2;
  if (0 == strlen (key)) return -3;
  if (0 == strlen (value)) return -4;

  return c_map_add (d -> dict,
    c_symbol_add (d -> symbols, key),
    c_symbol_add (d -> symbols, value)
  );
}

char *
c_dict_find (C_DICT *d, char *key) {
  return (char *) c_map_find (d -> dict, key);
}

void
c_dict_remove (C_DICT *d, char *key) {
  c_map_remove (d -> dict, key);
}

C_ITERATOR *
c_dict_iterator (C_DICT *d) {
  return c_map_iterator (d -> dict);
}

int
c_dict_size (C_DICT *d) {
  return c_map_size (d -> dict);
}
