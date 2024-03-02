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
 * The c_array_free function frees the C_ARRAY and all internal resources.
 *
 * There are no functions to inspect the array, or iterate the contents as
 * these operations can be performed like any other array operation. For
 * instance:
 *
 *     int *element = (int *) c_array_get (array);
 *     for (int i = 0;
 *          i < c_array_length (array);
 *          element ++, i ++) { ... }
 *
 * It is reasonable to use a C_ARRAY to establish a minimum-sized array using
 * c_array_require, and then operate on that array with bounds-safe code. It
 * is important to note that direct manipulation of the array will mean that
 * length (c_array_length) may not reflect the actual data length.
 */

#include <sys/types.h>
#include "c_iterator.h"

typedef struct C_ARRAY C_ARRAY;

/*
 * Function  : c_array_create_base
 * Purpose   : creates a new buffer
 * Parameters: element size
 *             initial size of internal buffer
 *             linear: if zero grow as a multiplier of factor
 *                     else grow by increasing size by factor
 *             factor
 * Return    : pointer to a C_ARRAY or NULL if out of memory
 */
C_ARRAY *c_array_create_base(size_t, int, int, int);

/*
 * Function  : c_array_create
 * Purpose   : creates a new buffer with reasonable defaults
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
int c_array_require(C_ARRAY *, int);

/*
 * Function  : c_array_append
 * Purpose   : appends an element to the array
 * Parameters: pointer to C_ARRAY
 *             pointer to element
 * Return    : zero on success
 */
int c_array_append(C_ARRAY *, void *);

/*
 * Function  : c_array_clear
 * Purpose   : resets a C_ARRAY
 * Parameters: pointer to C_ARRAY
 * Return    : none
 * Notes     :
 *
 * 1. The contents of the array is unreliable after calling this function. The
 *    length of the array is zero, and subsequent calls to any of the append
 *    functions will begin at the start of the buffer.
 *
 * 2. No resources are freed. In other words, the internal space allocated
 *    for the array remains the same size; only the "length" is reset.
 */
void c_array_clear(C_ARRAY *);

/*
 * Function  : c_array_get
 * Purpose   : returns a pointer to the start of the array
 * Parameters: pointer to C_ARRAY
 * Return    : a pointer to the start of the array
 * Notes     :
 *
 * 1. This value is unreliable after a call to either of c_array_free,
 *    c_array_append.
 */
void *c_array_get(C_ARRAY *);

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
int c_array_length(C_ARRAY *);

#endif
