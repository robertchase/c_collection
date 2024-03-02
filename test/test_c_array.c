#include <assert.h>
#include <string.h>

#include "c_array.h"
#include "../inc/c_array.h"

// a little cheating so we can examine operation for tests
typedef struct TEST_ARRAY {
    size_t element_size;
    int is_linear;
    int factor;
    int buffer_length;
    int length;
    char *buffer;
} TEST_ARRAY;

int main (int argc, char **argv) {
    C_ARRAY *a = c_array_create (sizeof(int));
    assert (a);

    int data[] = {1, 2, 3, 4, 5};

    /* simple append */
    assert (0 == c_array_append (a, data));
    assert (1 == c_array_length (a));
    assert (1 == ((int *) c_array_get (a))[0]);
    assert (0 == c_array_append (a, data + 1));
    assert (2 == c_array_length (a));
    assert (2 == ((int *) c_array_get (a))[1]);
    c_array_clear (a);
    assert (0 == c_array_length (a));
    assert (0 == c_array_append (a, data + 2));
    assert (3 == ((int *) c_array_get (a))[0]);

    /* force non-linear (default) buffer growth */
    c_array_clear (a);
    int buffer_length = ((TEST_ARRAY *) a) -> buffer_length;
    for (int i = 0; i < buffer_length + 1; i++) {
        c_array_append (a, &i);
    }
    assert(((TEST_ARRAY *) a) -> buffer_length == buffer_length * ((TEST_ARRAY *) a) -> factor);
    int *ele = (int *) c_array_get (a);
    for (int i = 0; i < c_array_length (a); ele++, i++) {
        assert ((* ele) == i);
    }

    /* require */
    buffer_length = ((TEST_ARRAY *) a) -> buffer_length;
    assert (0 == c_array_require(a, buffer_length - 1));
    assert (buffer_length == ((TEST_ARRAY *) a) -> buffer_length); // no change
    assert (0 == c_array_require(a, buffer_length));
    assert (buffer_length == ((TEST_ARRAY *) a) -> buffer_length); // no change
    assert (0 == c_array_require(a, buffer_length + 1));
    assert (buffer_length < ((TEST_ARRAY *) a) -> buffer_length); // got bigger

    c_array_free (a);

    /* linear growth */
    a = c_array_create_base (sizeof(int), 1, 1, 1);
    assert (1 == ((TEST_ARRAY *) a) -> buffer_length);
    assert (0 == c_array_append (a, data));
    assert (1 == ((TEST_ARRAY *) a) -> buffer_length);
    assert (0 == c_array_append (a, data + 1));
    assert (2 == ((TEST_ARRAY *) a) -> buffer_length);
    assert (data[1] == * ((int *) c_array_get (a)) + 1);
    assert (0 == c_array_append (a, data + 2));
    assert (3 == ((TEST_ARRAY *) a) -> buffer_length);
    assert (data[2] == * ((int *) c_array_get (a)) + 2);
    assert (3 == c_array_length (a));

  return 0;
}
