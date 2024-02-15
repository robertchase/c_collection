#include <stdlib.h>
#include <string.h>

#include "c_hash.h"
#include "c_symbol.h"
#include "hash_func.h"

struct C_SYMBOL {
  C_HASH *table;
};

typedef struct _C_SYMBOL {
  char *symbol;
} _C_SYMBOL;

static unsigned int
_calc (void *item, void *context) {
  _C_SYMBOL *s = (_C_SYMBOL *) item;
  return hash_string_calculator (s -> symbol);
}

static int
_compare (void *item1, void *item2, void *context) {
  _C_SYMBOL *s1 = (_C_SYMBOL *) item1;
  _C_SYMBOL *s2 = (_C_SYMBOL *) item2;
  return hash_string_comparator (s1 -> symbol, s2 -> symbol);
}

static void
_garbage (void *item, void *context) {
  _C_SYMBOL *s = (_C_SYMBOL *) item;
  free (s -> symbol);
}

static void *
_extractor (void *item) {
  _C_SYMBOL *s = (_C_SYMBOL *) item;
  return s -> symbol;
}

C_SYMBOL *
c_symbol_create (void) {

  C_SYMBOL *s = (C_SYMBOL *) malloc (sizeof (C_SYMBOL));
  if (s) {
    memset (s, 0x00, sizeof (C_SYMBOL));
    s -> table = c_hash_create (sizeof (_C_SYMBOL), _calc, _compare, _garbage,
      NULL);
    if (!s -> table) {
      free (s);
      s = NULL;
    }
  }

  return s;
}

void
c_symbol_free (C_SYMBOL *s) {
  if (s) {
    c_hash_free (s -> table);
    free (s);
  }
}

char *
c_symbol_add (C_SYMBOL *s, char *string) {
  _C_SYMBOL add = {string};
  _C_SYMBOL *symbol = (_C_SYMBOL *) c_hash_find (s -> table, &add);

  if (symbol) return symbol -> symbol;

  add.symbol = (char *) malloc (strlen (string) + 1);
  if (add.symbol) {
    strcpy (add.symbol, string);
    c_hash_insert (s -> table, &add);
  }

  return add.symbol;
}

char *
c_symbol_find (C_SYMBOL *s, char *string) {
  _C_SYMBOL find = {string};
  _C_SYMBOL *symbol = (_C_SYMBOL *) c_hash_find (s -> table, &find);

  return symbol ? symbol -> symbol : NULL;
}

void
c_symbol_clear (C_SYMBOL *s) {
  c_hash_clear (s -> table);
}

void
c_symbol_remove (C_SYMBOL *s, char *string) {
  _C_SYMBOL remove = {string};
  c_hash_remove (s -> table, &remove);
}

C_ITERATOR *
c_symbol_iterator (C_SYMBOL *s) {
  return c_hash_iterator (s -> table, _extractor);
}

int
c_symbol_size (C_SYMBOL *s) {
  return c_hash_size (s -> table);
}
