#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "c_list.h"
#include "c_hash.h"

typedef struct _NODE {
  unsigned int hash;
  char item [0]; // this gets properly sized in _c_hash_insert below
} _NODE;

struct C_HASH {
  C_HASH_CALCULATOR calculator;
  C_HASH_COMPARATOR comparator;
  C_HASH_GARBAGE garbage;
  C_HASH_ITERATOR_ITEM extractor;
  void *context;

  /* hash table */
  int table_size;
  int item_size;
  C_LIST **table;

  /* find */
  unsigned int fnd_hash;
  int fnd_index;
  C_ITERATOR *fnd_iterator;

  /* iterate */
  C_ITERATOR *iterator;
  int itr_index;
  C_ITERATOR *itr_iterator;
  _NODE *current;

  int size;
};

#define C_HASH_INITIAL_TABLE_SIZE 16
#define C_HASH_LOAD_FACTOR .75

C_HASH *
c_hash_create (size_t item_size, C_HASH_CALCULATOR cal, C_HASH_COMPARATOR com,
    C_HASH_GARBAGE garbage, void *context) {

  C_HASH *h = (C_HASH *) malloc (sizeof (C_HASH));
  if (h) {
    memset (h, 0x00, sizeof (C_HASH));
    h -> item_size = item_size;
    h -> calculator = cal;
    h -> comparator = com;
    h -> garbage = garbage;
    h -> context = context;
    h -> table_size = C_HASH_INITIAL_TABLE_SIZE;
    h -> table = (C_LIST **)
      malloc (sizeof (C_LIST *) * h -> table_size);
    if (h -> table) {
      memset (h -> table, 0x00, sizeof (C_LIST *) * h -> table_size);
    } else {
      free (h);
      h = NULL;
    }
  }

  return h;
}

static void
_c_hash_clear (C_HASH *h) {
  if (h) {
    int i;
    for (i = 0; i < h -> table_size; i ++) {
      C_LIST *list = h -> table [i];
      if (list) {
        while (c_list_size (list)) {
          _NODE *node = (_NODE *) c_list_take (list);
          if (h -> garbage) h -> garbage (&node -> item, h -> context);
          free (node);
        }
        c_list_free (list);
        h -> table [i] = NULL;
      }
    }
    h -> size = 0;
  }
}

void
c_hash_free (C_HASH *h) {

  if (h) {
    _c_hash_clear (h);
  
    free (h -> table);
    c_iterator_free (h -> iterator);
    free (h);
  }
}

void
c_hash_clear (C_HASH *h) {
  _c_hash_clear (h);
}

static _NODE *
_c_hash_find (C_HASH *h, void *item) {

  h -> fnd_hash = h -> calculator (item, h -> context);
  h -> fnd_index = h -> fnd_hash % h -> table_size;
  C_LIST *list = h -> table [h -> fnd_index];

  if (list) {
    h -> fnd_iterator = c_list_iterator (list);
    while (c_iterator_has_next (h -> fnd_iterator)) {
      _NODE *node = (_NODE *) c_iterator_next (h -> fnd_iterator);
      if (node -> hash == h -> fnd_hash) {
        if (0 == h -> comparator (&node -> item, item, h -> context))
          return node;
      }
    }
  }

  return NULL;
}

static int
_c_hash_rehash (C_HASH *h) {
  int i;
  size_t size = h -> table_size * 2;
  C_LIST **new_table = (C_LIST **) malloc (sizeof (C_LIST *) * size);
  if (!new_table) return C_HASH_ERROR_MEMORY;

  memset (new_table, 0x00, sizeof (C_LIST *) * size);
  for (i = 0; i < h -> table_size; i ++) {
    C_LIST *list = h -> table [i];
    if (list) {
      while (c_list_size (list)) {
        _NODE *node = (_NODE *) c_list_take (list);
        int index = node -> hash % size;
        C_LIST *new_list = new_table [index];
        if (NULL == new_list) new_list = new_table [index] = c_list_create ();
        if (!new_list) {
          int j;
          for (j = 0; j < size; j ++) c_list_free (new_table [j]);
          free (new_table);
          return C_HASH_ERROR_MEMORY;
        }
        c_list_add (new_list, node);
      }
      c_list_free (list);
    }
  }

  free (h -> table);
  h -> table = new_table;
  h -> table_size = size;

  return 0;
}

static int
_c_hash_check_rehash (C_HASH *h) {
  if ((float) h -> size / (float) h -> table_size > C_HASH_LOAD_FACTOR) {
    return _c_hash_rehash (h);
  }
  return 0;
}

static int
_c_hash_insert (C_HASH *h, void *item) {
  _NODE *node = (_NODE *) malloc (sizeof (_NODE) + h -> item_size);
  if (!node) return C_HASH_ERROR_MEMORY;

  node -> hash = h -> fnd_hash;
  memcpy (&node -> item, item, h -> item_size);
  C_LIST *list = h -> table [h -> fnd_index];
  if (!list) list = h -> table [h -> fnd_index] = c_list_create ();
  if (!list) {
    free (node);
    return C_HASH_ERROR_MEMORY;
  }
  c_list_add (list, node);

  h -> size += 1;
  if (h -> iterator && c_iterator_has_next (h -> iterator))
    return 0; // don't screw with things

  return _c_hash_check_rehash (h);
}

int
c_hash_insert (C_HASH *h, void *item) {
  _NODE *find = _c_hash_find (h, item);
  if (find) return C_HASH_ERROR_DUPLICATE;

  return _c_hash_insert (h, item);
}

int
c_hash_replace (C_HASH *h, void *item) {
  _NODE *find = _c_hash_find (h, item);
  if (!find) return C_HASH_ERROR_NOT_FOUND;

  if (h -> garbage) h -> garbage (&find -> item, h -> context);
  memcpy (&find -> item, item, h -> item_size);

  return 0;
}

void *
c_hash_find (C_HASH *h, void *item) {
  _NODE *found = _c_hash_find (h, item);
  return found ? &found -> item : NULL;
}

void
c_hash_remove (C_HASH *h, void *item) {
  _NODE *find = _c_hash_find (h, item);

  if (find) {
    if (h -> garbage) h -> garbage (&find -> item, h -> context);
    c_iterator_remove (h -> fnd_iterator);
    free (find);
    h -> size -= 1;
  }
}

static int
_itr_next_item (C_HASH *h) {

  while (h -> itr_index < h -> table_size) {
    C_LIST *list = h -> table [h -> itr_index ++];
    if (list) {
      if (c_list_size (list)) {
        h -> itr_iterator = c_list_iterator (list);
        return 1;
      }
    }
  }

  return 0;
}

static int
_itr_init (void *ctx) {
  C_HASH *h = (C_HASH *) ctx;
  h -> itr_index = 0;
  if (0 != _c_hash_check_rehash (h)) return 0; // safe time to try rehash
  return _itr_next_item (h);
}

static int
_itr_advance (void *ctx) {
  C_HASH *h = (C_HASH *) ctx;
  if (c_iterator_has_next (h -> itr_iterator)) return 1;
  return _itr_next_item (h);
}

static void *
_itr_retrieve (void *ctx) {
  C_HASH *h = (C_HASH *) ctx;
  h -> current = (_NODE *) c_iterator_next (h -> itr_iterator);
  if (h -> extractor)
    return h -> extractor ((void *) &h -> current -> item);
  return (void *) &h -> current -> item;
}

static int
_itr_remove (void *ctx) {
  C_HASH *h = (C_HASH *) ctx;
  c_iterator_remove (h -> itr_iterator);
  if (h -> garbage) h -> garbage (&h -> current -> item, h -> context);
  free (h -> current);
  h -> size -= 1;
  return _itr_advance (h);
}

C_ITERATOR *
c_hash_iterator (C_HASH *h, C_HASH_ITERATOR_ITEM extract) {
  if (h -> iterator) {
    c_iterator_reset (h -> iterator);
  } else {
    h -> extractor = extract;
    h -> iterator = c_iterator_create (
      _itr_init,
      _itr_advance,
      _itr_retrieve,
      _itr_remove,
      0,
      (void *) h
    );
  }
  return h -> iterator;
}

int
c_hash_size (C_HASH *h) {
  return h -> size;
}

int
c_hash_table_size (C_HASH *h) {
  return h -> table_size;
}
