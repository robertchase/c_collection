#include <assert.h>
#include <string.h>

#include "c_buffer.h"

// a little cheating so we can examine operation for tests
typedef struct TEST_BUFFER {
  int is_linear;
  int factor;
  int buffer_length;
  int length;
  char *buffer;
} TEST_BUFFER;

int main (int argc, char **argv) {
  C_BUFFER *b = c_buffer_create ();

  assert (b);
  assert (0 == c_buffer_append (b, "test", 4));
  assert (c_buffer_length (b) == 4);
  assert (0 == c_buffer_append_char (b, 0x00));
  assert (c_buffer_length (b) == 5);
  assert (0 == strcmp (c_buffer_get (b), "test"));
  assert (0 == c_buffer_append (b, "01234567890123456", 16));

  c_buffer_clear (b);
  assert (c_buffer_length (b) == 0);
  assert (0 == c_buffer_append_char (b, 'f'));
  assert (1 == c_buffer_length (b));
  assert (0 == c_buffer_append_char (b, 'r'));
  assert (0 == c_buffer_append_char (b, 'e'));
  assert (0 == c_buffer_append_char (b, 'd'));
  assert (c_buffer_length (b) == 4);
  assert (0 == c_buffer_append_char (b, 0x00));
  assert (c_buffer_length (b) == 5);
  assert (0 == strcmp ("fred", c_buffer_get (b)));
  assert (0 == c_buffer_shift (b, 2));
  assert (3 == c_buffer_length (b));
  assert (0 == strcmp ("ed", c_buffer_get (b)));

  c_buffer_clear (b);
  assert (0 == c_buffer_append_str (b, "test-"));
  assert (0 == c_buffer_append_int (b, 1234));
  assert (0 == c_buffer_append_char (b, 0x00));
  assert (0 == strcmp ("test-1234", c_buffer_get (b)));

  c_buffer_free (b);

  // test normal growth
  b = c_buffer_create ();
  assert (16 == ((TEST_BUFFER *) b) -> buffer_length);
  assert (0 == c_buffer_append_str (b, "1234567890123456"));
  assert (16 == ((TEST_BUFFER *) b) -> buffer_length);
  assert (0 == c_buffer_append_str (b, "1"));
  assert (32 == ((TEST_BUFFER *) b) -> buffer_length);
  c_buffer_free (b);

  // test linear growth
  b = c_buffer_create_base (10, 1, 100);
  assert (10 == ((TEST_BUFFER *) b) -> buffer_length);
  assert (0 == c_buffer_append_str (b, "1234567890"));
  assert (10 == ((TEST_BUFFER *) b) -> buffer_length);
  assert (0 == c_buffer_append_str (b, "1"));
  assert (110 == ((TEST_BUFFER *) b) -> buffer_length);
  c_buffer_free (b);

  // test required
  b = c_buffer_create ();
  assert (0 == c_buffer_require (b, 40));
  assert (40 == ((TEST_BUFFER *) b) -> buffer_length);
  assert (0 == c_buffer_append_str (b, "1234567890123456789012345678901234567890"));
  assert (40 == ((TEST_BUFFER *) b) -> buffer_length);
  assert (0 == c_buffer_append_str (b, "1"));
  assert (80 == ((TEST_BUFFER *) b) -> buffer_length);
  c_buffer_free (b);

  return 0;
}
