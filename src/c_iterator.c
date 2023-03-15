#include "stdlib.h"
#include "string.h"
#include "c_iterator.h"

struct C_ITERATOR {
  void *context;
  unsigned char init;
  unsigned char ready;
  unsigned char removable;
  unsigned char done;
  int (*initialize) (void *);
  int (*advance) (void *);
  void *(*retrieve) (void *);
  int (*remove) (void *);
  void (*free) (void *);
};

C_ITERATOR *c_iterator_create (
    int (*init) (void *),
    int (*advance) (void *),
    void *(*retrieve) (void *),
    int (*remove) (void *),
    void (*free) (void *),
    void *context
    ){
  C_ITERATOR *i = (C_ITERATOR *) malloc (sizeof (C_ITERATOR));
  memset (i, 0x00, sizeof (C_ITERATOR));
  i -> initialize = init;
  i -> advance = advance;
  i -> retrieve = retrieve;
  i -> remove = remove;
  i -> free = free;
  i -> context = context;
  return i;
}

void
c_iterator_free (C_ITERATOR *i) {
  if (i) {
    if (i -> free)
      (*i -> free) (i -> context);
    free (i);
  }
}

void
c_iterator_reset (C_ITERATOR *i) {
  i -> init = 0;
  i -> ready = 0;
  i -> removable = 0;
  i -> done = 0;
}

int
c_iterator_has_next (C_ITERATOR *i) {

  if (!i -> done) {
    if (!i -> ready) {
      if (!i -> init) {
        i -> ready = (*i -> initialize) (i -> context);
        i -> init = 1;
      } else {
        i -> ready = (*i -> advance) (i -> context);
      }
      i -> removable = 0;
      i -> done = !i -> ready;
    }
  }

  return i -> ready; 
}

void *
c_iterator_next (C_ITERATOR *i) {
  void *value = NULL;

  if (c_iterator_has_next (i)) {
    value = (*i -> retrieve) (i -> context);
    i -> ready = 0;
    i -> removable = 1;
  }
  
  return value;
}

void
c_iterator_remove (C_ITERATOR *i) {

  if (i -> removable) {
    if (1 == (*i -> remove) (i -> context)) {
      i -> ready = 1;
    } else {
      i -> done = 1;
    }
    i -> removable = 0;
  }

}
