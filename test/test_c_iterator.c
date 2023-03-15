#include <assert.h>
#include <stdlib.h>
#include "c_iterator.h"

typedef struct CONTEXT {
  int *base;
  int size;
  int current;
} CONTEXT;

static int
_init (void *context) {
  CONTEXT *ctx = (CONTEXT *) context;
  if (0 == ctx -> size) return 0;
  ctx -> current = 1;
  return 1;
}

static int
_advance (void *context) {
  CONTEXT *ctx = (CONTEXT *) context;
  if (ctx -> current == ctx -> size) return 0;
  ctx -> current += 1;
  return 1;
}

static void *
_retrieve (void *context) {
  CONTEXT *ctx = (CONTEXT *) context;
  return ctx -> base + (ctx -> current - 1);
}

static int
_remove (void *context) {
  CONTEXT *ctx = (CONTEXT *) context;
  int i = ctx -> current;
  while (i < ctx -> size) {
    ctx -> base [i -1] = ctx -> base [i];
    i += 1;
  }
  ctx -> size -= 1;
  if (ctx -> current > ctx -> size) return 0;
  return 1;
}

static void
_free (void *context) {
  free (context);
}

int main (void) {
  int a [] = {1,3,5,7,11,13};
  CONTEXT *ctx = (CONTEXT *) malloc (sizeof (CONTEXT));
  ctx -> base = a;
  ctx -> size = 6;
  ctx -> current = 1;
  C_ITERATOR *i = c_iterator_create (_init, _advance, _retrieve, _remove, _free, ctx);
  int item;
  int count = 0;
  while (c_iterator_has_next (i)) {
    count += 1;
    item = *(int *)c_iterator_next (i);
    if (3 == item) {
      c_iterator_remove (i);
    }
  }
  assert (6 == count);
  assert (5 == ctx -> size);
  c_iterator_reset (i);
  while (c_iterator_has_next (i)) {
    item = *(int *)c_iterator_next (i);
    c_iterator_remove (i);
  }
  assert (0 == ctx -> size);

  c_iterator_free (i);
  return 0;
}
