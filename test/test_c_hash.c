#include <assert.h>
#include <string.h>
#include "c_hash.h"
#include "hash_func.h"

typedef struct STRING {
  int whatever;
  float yep;
  char *value;
} STRING;

static unsigned int
_calc (void *item, void *context) {
  STRING *s = (STRING *) item;
  return hash_string_calculator (s -> value);
}

static int
_compare (void *item1, void *item2, void *context) {
  STRING *s1 = (STRING *) item1;
  STRING *s2 = (STRING *) item2;
  return hash_string_comparator (s1 -> value, s2 -> value);
}

static void
_garbage (void *item, void *context) {
  STRING *s = (STRING *) item;
  free (s -> value);
}

static void *
_extractor (void *item) {
  STRING *s = (STRING *) item;
  return s -> value;
}

int main (void) {
  STRING s;

  C_HASH *h = c_hash_create (sizeof (STRING), _calc, _compare, 0, 0);
  assert (0 == c_hash_size (h));
  s.value = "test string";
  assert (C_HASH_ERROR_NOT_FOUND == c_hash_replace (h, &s));
  assert (0 == c_hash_insert (h, &s));
  assert (C_HASH_ERROR_DUPLICATE == c_hash_insert (h, &s));
  assert (0 == c_hash_replace (h, &s));
  assert (1 == c_hash_size (h));
  c_hash_clear (h);
  assert (0 == c_hash_size (h));

  s.value = "foo";
  assert (0 == c_hash_insert (h, &s));
  s.value = "bar";
  assert (0 == c_hash_insert (h, &s));
  assert (2 == c_hash_size (h));

  s.value = "3";
  assert (0 == c_hash_insert (h, &s));
  s.value = "4";
  assert (0 == c_hash_insert (h, &s));
  s.value = "5";
  assert (0 == c_hash_insert (h, &s));
  s.value = "6";
  assert (0 == c_hash_insert (h, &s));
  s.value = "7";
  assert (0 == c_hash_insert (h, &s));
  s.value = "8";
  assert (0 == c_hash_insert (h, &s));
  s.value = "9";
  assert (0 == c_hash_insert (h, &s));
  s.value = "10";
  assert (0 == c_hash_insert (h, &s));
  s.value = "11";
  assert (0 == c_hash_insert (h, &s));
  s.value = "12";
  assert (0 == c_hash_insert (h, &s));
  assert (12 == c_hash_size (h));
  assert (16 == c_hash_table_size (h));
  s.value = "13";
  assert (0 == c_hash_insert (h, &s));
  assert (13 == c_hash_size (h));
  assert (32 == c_hash_table_size (h));

  s.value = "10";
  assert (c_hash_find (h, &s));
  int count = 0;
  C_ITERATOR *it = c_hash_iterator (h, _extractor);
  while (c_iterator_has_next (it)) {
    char *item = (char *) c_iterator_next (it);
    if (0 == strcmp ("10", item)) c_iterator_remove (it);
    count += 1;
  }
  assert (13 == count);
  assert (12 == c_hash_size (h));
  assert (NULL == c_hash_find (h, &s));

  s.value = "5";
  assert (c_hash_find (h, &s));
  c_hash_remove (h, &s);
  assert (NULL == c_hash_find (h, &s));
  assert (11 == c_hash_size (h));

  c_hash_clear (h);
  assert (0 == c_hash_size (h));
  c_hash_free (h);

  h = c_hash_create (sizeof (STRING), _calc, _compare, 0, 0);

  s.value = "1";
  assert (0 == c_hash_insert (h, &s));
  s.value = "2";
  assert (0 == c_hash_insert (h, &s));
  s.value = "3";
  assert (0 == c_hash_insert (h, &s));
  s.value = "4";
  assert (0 == c_hash_insert (h, &s));
  s.value = "5";
  assert (0 == c_hash_insert (h, &s));
  s.value = "6";
  assert (0 == c_hash_insert (h, &s));
  s.value = "7";
  assert (0 == c_hash_insert (h, &s));
  s.value = "8";
  assert (0 == c_hash_insert (h, &s));
  s.value = "9";
  assert (0 == c_hash_insert (h, &s));
  s.value = "10";
  assert (0 == c_hash_insert (h, &s));
  s.value = "11";
  assert (0 == c_hash_insert (h, &s));
  s.value = "12";
  assert (0 == c_hash_insert (h, &s));
  assert (12 == c_hash_size (h));
  assert (16 == c_hash_table_size (h));

  it = c_hash_iterator (h, _extractor);
  c_iterator_next (it); // get the iterator going (stops subsequent rehash)

  s.value = "13";
  assert (0 == c_hash_insert (h, &s)); // rehash stopped by running iterator
  assert (13 == c_hash_size (h));
  assert (16 == c_hash_table_size (h));

  it = c_hash_iterator (h, _extractor);
  c_iterator_next (it); // runs hash iterator init function (auto rehash)

  assert (32 == c_hash_table_size (h));
  c_hash_free (h);

  h = c_hash_create (sizeof (STRING), _calc, _compare, _garbage, 0);
  s.value = (char *) malloc (7);
  strcpy (s.value, "foobar");
  assert (0 == c_hash_insert (h, &s));
  s.value = (char *) malloc (7);
  strcpy (s.value, "foobar");
  assert (0 == c_hash_replace (h, &s)); // orphans previous "foobar"
  c_hash_free (h); // valgrind will tell you if everything was freed

  return 0;
}
