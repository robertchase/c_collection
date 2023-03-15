#include <stdlib.h>
#include <string.h>

#include "c_hash.h"
#include "c_map.h"
#include "hash_func.h"

struct C_MAP {
  C_MAP_CALCULATOR calculator;
  C_MAP_COMPARATOR comparator;
  C_MAP_GARBAGE garbage;
  C_HASH *table;
};

static unsigned int
_calc (void *item, void *context) {
  C_MAP *m = (C_MAP *) context;
  C_MAPITEM *i = (C_MAPITEM *) item;
  return m -> calculator (i -> key); // C_MAPITEM to key
}

static int
_compare (void *item1, void *item2, void *context) {
  C_MAP *m = (C_MAP *) context;
  C_MAPITEM *i1 = (C_MAPITEM *) item1;
  C_MAPITEM *i2 = (C_MAPITEM *) item2;
  return m -> comparator (i1 -> key, i2 -> key); // C_MAPITEMs to keys
}

static void
_garbage (void *item, void *context) {
  C_MAP *m = (C_MAP *) context;
  C_MAPITEM *i = (C_MAPITEM *) item;

  if (m -> garbage) m -> garbage (i -> key, i -> value);
}

static void *
_extractor (void *item) {
  return item;
}

static void *
_key_extractor (void *item) {
  C_MAPITEM *i = (C_MAPITEM *) item;
  return i -> key;
}

static void *
_value_extractor (void *item) {
  C_MAPITEM *i = (C_MAPITEM *) item;
  return i -> value;
}

C_MAP *
c_map_create (C_MAP_CALCULATOR cal, C_MAP_COMPARATOR com,
    C_MAP_GARBAGE garbage) {

  C_MAP *m = (C_MAP *) malloc (sizeof (C_MAP));
  if (m) {
    memset (m, 0x00, sizeof (C_MAP));
    m -> calculator = cal;
    m -> comparator = com;
    m -> garbage = garbage;
    m -> table = c_hash_create (
      sizeof (C_MAPITEM),
      _calc,
      _compare,
      _garbage,
      (void *) m
    );
  }

  return m;
}

C_MAP *
c_map_dict_create (C_MAP_GARBAGE garbage) {
  return c_map_create (hash_string_calculator, hash_string_comparator,
    garbage);
}

void
c_map_free (C_MAP *m) {
  if (m) {
    c_hash_free (m -> table);
    free (m);
  }
}

void
c_map_clear (C_MAP *m) {
  c_hash_clear (m -> table);
}

int
c_map_add (C_MAP *m, void *key, void *value) {
  C_MAPITEM item = {key, value};
  C_MAPITEM *found = c_hash_find (m -> table, &item);

  if (found) {

    /* call garbage as long as keys/values don't share memory locations */
    if (m -> garbage) {
      if (found -> key != key) m -> garbage (key, NULL);
      if (found -> value != value) m -> garbage (NULL, found -> value);
    }

    /* old key points to new value, C_MAPITEM updated in place */
    found -> value = value;

  /* key not found in table, insert a new key-value pair */
  } else return c_hash_insert (m -> table, &item);

  return 0;
}

static C_MAPITEM *
_c_map_find (C_MAP *m, void *key) {
  C_MAPITEM find = {key, NULL};
  return c_hash_find (m -> table, &find);
}

void *
c_map_find (C_MAP *m, void *key) {
  C_MAPITEM *item = _c_map_find (m, key);
  return item ? item -> value : NULL;
}

void *
c_map_find_key (C_MAP *m, void *key) {
  C_MAPITEM *item = _c_map_find (m, key);
  return item ? item -> key : NULL;
}

int
c_map_exists (C_MAP *m, void *key) {
  return NULL != _c_map_find (m, key);
}

void
c_map_remove (C_MAP *m, void *key) {
  C_MAPITEM find = {key, NULL};
  c_hash_remove (m -> table, &find);
}

C_ITERATOR *
c_map_iterator (C_MAP *m) {
  return c_hash_iterator (m -> table, _extractor);
}

C_ITERATOR *
c_map_key_iterator (C_MAP *m) {
  return c_hash_iterator (m -> table, _key_extractor);
}

C_ITERATOR *
c_map_value_iterator (C_MAP *m) {
  return c_hash_iterator (m -> table, _value_extractor);
}

int
c_map_size (C_MAP *m) {
  return c_hash_size (m -> table);
}
