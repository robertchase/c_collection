#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "c_array.h"

struct C_ARRAY {
    size_t element_size;
    int is_linear;
    int factor;
    int buffer_length;
    int length;
    void *buffer;

    int current;
    C_ITERATOR *iterator;
};

#define C_ARRAY_INITIAL_BUFFER_LENGTH 16

static int
_itr_init (void *ctx) {
    C_ARRAY *a = (C_ARRAY *) ctx;
    a -> current = 0;
    return a -> length ? 1 : 0;
}

static int
_itr_advance (void *ctx) {
    C_ARRAY *a = (C_ARRAY *) ctx;
    a -> current ++;
    return a -> current < a -> length ? 1 : 0;
}

static void *
_itr_retrieve (void *ctx) {
    C_ARRAY *a = (C_ARRAY *) ctx;
    return a -> buffer + a -> current * a -> element_size;
}

static int
_itr_remove (void *ctx) {
    C_ARRAY *a = (C_ARRAY *) ctx;
    if (a -> current + 1 < a -> length) {
        memmove(
            a -> buffer + a -> current * a -> element_size,
            a -> buffer + (a -> current + 1) * a -> element_size,
            (a -> length - a -> current - 1) * a -> element_size
        );
    }
    a -> length --;
    return a -> current < a -> length ? 1 : 0;
}

C_ARRAY *
c_array_create_base (size_t element_size, int initial, int is_linear, int factor) {
  C_ARRAY *a;

  a = (C_ARRAY *) malloc (sizeof (C_ARRAY));
  if (a) {
    memset (a, 0x00, sizeof (C_ARRAY));
    a -> element_size = element_size;
    a -> is_linear = is_linear;
    a -> factor = factor;
    a -> buffer_length = initial;
    a -> buffer = (char *) malloc (element_size * a -> buffer_length);
    if (!a -> buffer) {
      free (a);
      a = NULL;
    }
  }

  return a;
}

C_ARRAY *
c_array_create (size_t element_size) {
  return c_array_create_base (element_size, C_ARRAY_INITIAL_BUFFER_LENGTH, 0, 2);
}

void
c_array_free (C_ARRAY *a) {
  if (a) {
    free (a -> buffer);
    free (a);
  }
}

int
c_array_require (C_ARRAY *a, int required) {

  if (required > a -> buffer_length) {
    int length = a -> buffer_length;
    if (a -> is_linear) {
      length += a -> factor;
    } else {
      length *= a -> factor;
    }
    if (required > length) length = required;

    void *new_buffer = realloc (a -> buffer, length * a -> element_size);
    if (!new_buffer) return 1; // fubar

    a -> buffer_length = length;
    a -> buffer = new_buffer;
  }

  return 0;
}

int
c_array_append (C_ARRAY *a, void *item) {

  if (c_array_require (a, a -> length + 1)) return 1;

  memcpy (a -> buffer + a -> length * a -> element_size , item, a -> element_size);
  a -> length ++;

  return 0;
}

void
c_array_clear (C_ARRAY *a) {
  a -> length = 0;
}

void *
c_array_get (C_ARRAY *a, int index) {
  return a -> buffer + index * a -> element_size;
}

void *
c_array_set (C_ARRAY *a, int index, void *item) {
    void * result = NULL;
    if (index < a -> length) {
        result = a -> buffer + index * a -> element_size;
        memcpy(result, item, a -> element_size);
    }
    return result;
}

int
c_array_length (C_ARRAY *a) {
  return a -> length;
}

C_ITERATOR *
c_array_iterator (C_ARRAY *a) {
    if (a -> iterator) {
        c_iterator_reset (a -> iterator);
    } else {
        a -> iterator = c_iterator_create (
            _itr_init,
            _itr_advance,
            _itr_retrieve,
            _itr_remove,
            0,
            (void *) a
        );
    }
    return a -> iterator;
}
