#include <string.h>
#include <stdlib.h>

#include "c_hash.h"
#include "c_keyedset.h"

#include "hash_func.h"

struct C_KEYEDSET {
  unsigned int key; // next available key value
  C_HASH *table;    // underlying hash table
};

typedef struct _C_ITEM {
  unsigned int key;
  void *value;
} _C_ITEM;

static unsigned int
_calc (void *item, void *context) {
  _C_ITEM *i = (_C_ITEM *) item;
  return hash_uint_calculator (&i -> key);
}

static int
_compare (void *item1, void *item2, void *context) {
  _C_ITEM *i1 = (_C_ITEM *) item1;
  _C_ITEM *i2 = (_C_ITEM *) item2;
  return hash_uint_comparator (&i1 -> key, &i2 -> key);
}

static void *
_extractor (void *item) {
  _C_ITEM *i = (_C_ITEM *) item;
  return i -> value;
}

C_KEYEDSET *
c_keyedset_create (void) {
  C_KEYEDSET *k = NULL;

  C_HASH *h = c_hash_create (sizeof (_C_ITEM), _calc, _compare, NULL, NULL);

  if (h) {
    k = (C_KEYEDSET *) malloc (sizeof (C_KEYEDSET));
    if (k) {
      memset (k, 0x00, sizeof (C_KEYEDSET));
      k -> key = 1;
      k -> table = h;
    } else {
      c_hash_free (h);
    }
  }

  return k;
}

void
c_keyedset_free (C_KEYEDSET *k) {
  if (k) {
    c_hash_free (k -> table);
    free (k);
  }
}

int
c_keyedset_add (C_KEYEDSET *k, void *item) {
  unsigned int key = k -> key ++;
  _C_ITEM add = {key, item};

  /* get next non-zero, unused key value */
  while ((0 == add.key) || c_hash_find (k -> table, &add))
    add.key = k -> key ++;

  c_hash_insert (k -> table, &add);
  
  return add.key;
}

void *
c_keyedset_get (C_KEYEDSET *k, int key) {
  _C_ITEM find = {key, NULL};
  _C_ITEM *found = (_C_ITEM *) c_hash_find (k -> table, &find);
  return found ? found -> value : NULL;
}

void
c_keyedset_remove (C_KEYEDSET *k, int key) {
  _C_ITEM remove = {key, NULL};
  c_hash_remove (k -> table, &remove);
}

C_ITERATOR *
c_keyedset_iterator (C_KEYEDSET *k) {
  return c_hash_iterator (k -> table, _extractor);
}

int
c_keyedset_size (C_KEYEDSET *k) {
  return c_hash_size (k -> table);
}
