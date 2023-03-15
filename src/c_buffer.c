#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "c_buffer.h"

struct C_BUFFER {
  int is_linear;
  int factor;
  int buffer_length;
  int length;
  char *buffer;
};

#define C_BUFFER_INITIAL_BUFFER_LENGTH 16

C_BUFFER *
c_buffer_create_base (int initial, int is_linear, int factor) {
  C_BUFFER *b;

  b = (C_BUFFER *) malloc (sizeof (C_BUFFER));
  if (b) {
    memset (b, 0x00, sizeof (C_BUFFER));
    b -> is_linear = is_linear;
    b -> factor = factor;
    b -> buffer_length = initial;
    b -> buffer = (char *) malloc (b -> buffer_length);
    if (!b -> buffer) {
      free (b);
      b = NULL;
    }
  }

  return b;
}

C_BUFFER *
c_buffer_create (void) {
  return c_buffer_create_base (C_BUFFER_INITIAL_BUFFER_LENGTH, 0, 2);
}

void
c_buffer_free (C_BUFFER *b) {
  if (b) {
    free (b -> buffer);
    free (b);
  }
}

int
c_buffer_require (C_BUFFER *b, int required) {

  if (required > b -> buffer_length) {
    int length = b -> buffer_length;
    if (b -> is_linear) {
      length += b -> factor;
    } else {
      length *= b -> factor;
    }
    if (required > length) length = required;

    char *new_buffer = (char *) realloc (b -> buffer, length);
    if (!new_buffer) return 1; // fubar

    b -> buffer_length = length;
    b -> buffer = new_buffer;
  }

  return 0;
}

int
c_buffer_append (C_BUFFER *b, char *c, int len) {

  if (c_buffer_require (b, b -> length + len)) return 1;

  memcpy (b -> buffer + b -> length, c, len);
  b -> length += len;

  return 0;
}

int
c_buffer_append_str (C_BUFFER *b, char *c) {
  return c_buffer_append (b, c, strlen (c));
}

int
c_buffer_append_char (C_BUFFER *b, char c) {

  if (c_buffer_require (b, b -> length + 1)) return 1;

  b -> buffer [b -> length] = c;
  b -> length ++;

  return 0;
}

int
c_buffer_append_int (C_BUFFER *b, int n) {
  char convert [32];

  sprintf (convert, "%d", n);

  return c_buffer_append_str (b, convert);
}

int
c_buffer_shift (C_BUFFER *b, int shift) {
  if (shift < 1) return 1;
  int amount = b -> length - shift;
  if (amount < 0) {
    c_buffer_clear (b);
  } else {
    memmove (b -> buffer, b -> buffer + shift, amount);
    b -> length = amount;
  }
  return 0;
}

void
c_buffer_clear (C_BUFFER *b) {
  b -> length = 0;
}

char *
c_buffer_get (C_BUFFER *b) {
  return b -> buffer;
}

int
c_buffer_length (C_BUFFER *b) {
  return b -> length;
}
