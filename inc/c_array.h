#ifndef _C_ARRAY_H
#define _C_ARRAY_H

/*
 * A C_ARRAY implements a contiguous memory region of fixed-sized elements
 * which automatically grows to accomodate the data appended to it. This may
 * be useful when arrays of fixed size are inconvenient or restrictive. This
 * flexibility does not come without cost, but the ability to re-use the array
 * may reduce this cost over the life of the application.
 *
 * The c_array_create function creates a new C_ARRAY with a fixed element
 * size. The c_array_create function uses a default initial allocation of
 * memory and default growth pattern; this is usually sufficient. If you want
 * specific control over these values, use c_array_create_base. The
 * c_array_require function can be used to make sure the array is at least a
 * specified size.
 *
 * The c_array_append function adds data to the end of the array, allocating
 * memory when necessary. The array contents (c_array_get) and length
 * (c_array_length) can be inspected at any time, although these values will
 * not be preserved over the life of the array. The c_array_clear function
 * resets the length of the array to zero, but does not release any resources.
 *
 * The c_array_get function performs a memory-safe retrieval of an element in
 * the array by index. The c_array_set function performs a memory-safe
 * assignment of an element in the array by index.
 *
 * The c_array_free function frees the C_ARRAY and all internal resources.
 *

Since a C_ARRAY manages a contiguous memory region, it is possible to use
standard language operations to navigate the array. For instance:

    int *element = (int *) c_array_get (array, 0);
    int length = c_array_length (array);
    for (int i = 0; i < length; element ++, i ++) {
        ...
    }

It is also possible to use a C_ARRAY to establish a minimum-sized array
using c_array_require, and then operate on that array without using
c_array* functions. It is important to note that direct manipulation of the
array will mean that the length (c_array_length) may not reflect the actual
data length; in this case you will have to manage your own bounds checking.
 */

#include <sys/types.h>
#include "c_iterator.h"

typedef struct C_ARRAY C_ARRAY;

/*
 * Function  : c_array_create_base
 * Purpose   : creates a new array
 * Parameters: element size
 *             initial size of internal array
 *             linear: if zero grow as a multiplier of factor
 *                     else grow by increasing size by factor
 *             factor
 * Return    : pointer to a C_ARRAY or NULL if out of memory
 */
C_ARRAY *c_array_create_base(size_t, int, int, int);

/*
 * Function  : c_array_create
 * Purpose   : creates a new array with reasonable defaults
 * Parameters: element size
 * Return    : pointer to a C_ARRAY or NULL if out of memory
 */
C_ARRAY *c_array_create (size_t);

/*
 * Function  : c_array_free
 * Purpose   : frees C_ARRAY and all internal resources
 * Parameters: pointer to C_ARRAY
 * Return    : none
 */
void c_array_free (C_ARRAY *);

/*
 * Function  : c_array_require
 * Purpose   : make sure specified space is currently available in the array
 * Parameters: pointer to C_ARRAY
 *             minimum number of elements in the array
 * Return    : zero on success
 * Notes     :
 *
 * 1. if the specified space is not available it is acquired
 */
int c_array_require (C_ARRAY *, int);

/*
 * Function  : c_array_append
 * Purpose   : appends an element to the array
 * Parameters: pointer to C_ARRAY
 *             pointer to element
 * Return    : zero on success
 */
int c_array_append (C_ARRAY *, void *);

/*
 * Function  : c_array_clear
 * Purpose   : resets a C_ARRAY to have a zero length
 * Parameters: pointer to C_ARRAY
 * Return    : none
 * Notes     :
 *
 * 1. The contents of the array is unreliable after calling this function. The
 *    length of the array is zero, and subsequent calls to any of the append
 *    functions will begin at the start of the buffer. Calls to c_array_get
 *    and c_array_set will fail until the length of the array is greater than
 *    zero.
 *
 * 2. No resources are freed. In other words, the internal space allocated
 *    for the array remains the same size; only the "length" is reset.
 */
void c_array_clear (C_ARRAY *);

/*
 * Function  : c_array_get
 * Purpose   : returns a pointer to the element at the index-th position
 * Parameters: pointer to C_ARRAY
 *             index (starting at zero)
 * Return    : a pointer to the index-th element
 * Notes     :
 *
 * 1. If index is out-of-bounds, a NULL value is returned.
 */
void *c_array_get (C_ARRAY *, int);

/*
 * Function  : c_array_set
 * Purpose   : set a value in the array by index
 * Parameters: pointer to C_ARRAY
 *             index (starting at zero)
 *             pointer to value
 * Return    : a pointer to the index-th element
 * Notes     :
 *
 * 1. If index is out-of-bounds, a NULL value is returned.
 */
void *c_array_set (C_ARRAY *, int, void *);

/*
 * Function  : c_array_length
 * Purpose   : returns the number of elements in the array
 * Parameters: pointer to C_ARRAY
 * Return    : the number of elements in the array
 * Notes     :
 *
 * 1. (see c_array_get Note 1)
 *
 * 2. This value is unreliable if you choose to manipulate the array
 *    yourself.
 */
int c_array_length (C_ARRAY *);

/*
 * Function  : c_array_iterator
 * Purpose   : initializes the C_ARRAY iterator
 * Parameters: pointer to C_ARRAY
 * Return    : pointer to ITERATOR
 * Notes     :
 *
 * 1. The items in the C_ARRAY are visited in sequential order.
 *
 * 2. It is safe to remove the current item in the iterator using the
 *    iterator_remove function.
 *
 * 3. The data structures necessary for implementing an iterator on a C_ARRAY
 *    are wholly contained within the C_ARRAY and are reused if the iterator
 *    is traversed more than once. The c_array_free function releases any
 *    resources related to an iterator on the C_ARRAY.
 */
C_ITERATOR *c_array_iterator(C_ARRAY *);

#endif
