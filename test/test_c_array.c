#include <assert.h>
#include <string.h>

#include "c_array.h"
#include "c_iterator.h"
#include "../inc/c_array.h"
#include "../inc/c_iterator.h"

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
    assert (1 == * (int *) c_array_get (a, 0));
    assert (0 == c_array_append (a, data + 1));
    assert (2 == c_array_length (a));
    assert (2 == * (int *) c_array_get (a, 1));
    assert (NULL == c_array_get (a, 3));
    assert (NULL == c_array_get (a, 10));

    /* negative index */
    assert (2 == * (int *) c_array_get (a, -1));
    assert (1 == * (int *) c_array_get (a, -2));
    assert (NULL == c_array_get (a, -3));
    assert (NULL == c_array_get (a, -10));

    /* reset */
    c_array_clear (a);
    assert (0 == c_array_length (a));
    assert (0 == c_array_append (a, data + 2));
    assert (3 == * (int *) c_array_get (a, 0));

    /* set values directly */
    char *ptr = (char *) c_array_set(a, 0, data + 4);
    assert (ptr != NULL);
    assert ( * ptr == data[4]);
    assert (NULL == c_array_set(a, 1, data));

    /* negative set */
    c_array_clear (a);
    c_array_append (a, data);
    c_array_append (a, data);
    c_array_append (a, data);
    assert (data[2] == * (char *) c_array_set (a, -1, data + 2));
    assert (data[3] == * (char *) c_array_set (a, -3, data + 3));
    assert (NULL == c_array_set (a, -10, data));

    /* force non-linear (default) buffer growth */
    c_array_clear (a);
    int buffer_length = ((TEST_ARRAY *) a) -> buffer_length;
    for (int i = 0; i < buffer_length + 1; i++) {
        c_array_append (a, &i);
    }
    assert(((TEST_ARRAY *) a) -> buffer_length == buffer_length * ((TEST_ARRAY *) a) -> factor);

    int *ele = (int *) c_array_get (a, 0);
    for (int i = 0; i < c_array_length (a); ele++, i++) {
        assert ((* ele) == i); // directly step through array
        assert (* (int *) c_array_get(a, i) == i); // access element via function
    }

    /* iterator */
    int i = 0;
    C_ITERATOR *it = c_array_iterator (a);
    while (c_iterator_has_next(it)) {
        assert (i++ == * (int *) c_iterator_next (it));
    }
    assert (i == c_array_length (a));

    /* iterator + remove */
    int length = c_array_length (a);
    i = 0;
    c_iterator_reset (it);
    while (c_iterator_has_next (it)) {
        if (5 == * (int *) c_iterator_next (it)) {
            c_iterator_remove (it);
            break;
        }
    }
    assert (length == c_array_length (a) + 1);
    assert (6 == * (int *) c_array_get (a, 5));

    /* iterator + remove last element */
    length = c_array_length (a);
    int last = * (int *) c_array_get(a, length - 1);
    i = 0;
    c_iterator_reset (it);
    while (c_iterator_has_next (it)) {
        if (last == * (int *) c_iterator_next (it)) {
            c_iterator_remove (it);
            break;
        }
    }
    assert (length == c_array_length (a) + 1);
    assert (last - 1 == * (int *) c_array_get (a, c_array_length (a) - 1));

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
    assert (data[1] == * ((int *) c_array_get (a, 1)));
    assert (0 == c_array_append (a, data + 2));
    assert (3 == ((TEST_ARRAY *) a) -> buffer_length);
    assert (data[2] == * ((int *) c_array_get (a, 2)));
    assert (3 == c_array_length (a));

  return 0;
}
