#ifndef C_BUFFER_H
#define C_BUFFER_H

/*
 * A C_BUFFER implements a contiguous memory region which automatically grows
 * to accomodate the data appended to it. This may be useful when buffers of
 * fixed size are inconvenient or restrictive. This flexibility does not come
 * without cost, but the ability to re-use the buffer may reduce this cost over
 * the life of the application.
 *
 * The c_buffer_append* functions add data to the end of the buffer, allocating
 * memory when necessary. The buffer contents and length can be inspected at
 * any time, although these values will not be preserved over the life of the
 * buffer. The c_buffer_clear function resets the length of the buffer to zero,
 * but does not release any resources.
 *
 * The c_buffer_create_base function allows control over the initial
 * size and growth pattern of the buffer, if you think that sort
 * of thing is warranted by your expected use. As well, c_buffer_require can
 * prevent extra realloc calls when you know something about the buffer size
 * requirements beforehand. Normal use would be to call c_buffer_create and
 * ignore guesses about underlying memory requirements.
 *
 * Within the bounds of the current buffer (from c_buffer_get to
 * c_buffer_get [c_buffer_length - 1]) it is safe to modify any of the
 * contents. Modifications will be retained until c_buffer_free,
 * c_buffer_clear or, potentially, c_buffer_shift.
 */

typedef struct C_BUFFER C_BUFFER;

/*
 * Function  : c_buffer_create
 * Purpose   : creates a new buffer
 * Parameters: initial size of internal buffer
 *             linear: if zero grow as a multiplier of factor
 *                     else grow by increasing size by factor
 *             factor
 * Return    : pointer to a C_BUFFER or NULL if out of memory
 */
C_BUFFER *c_buffer_create_base (int, int, int);

/*
 * Function  : c_buffer_create
 * Purpose   : creates a new buffer
 * Parameters: none
 * Return    : pointer to a C_BUFFER or NULL if out of memory
 */
C_BUFFER *c_buffer_create (void);

/*
 * Function  : c_buffer_free
 * Purpose   : frees C_BUFFER and all internal resources
 * Parameters: pointer to C_BUFFER
 * Return    : none
 */
void c_buffer_free (C_BUFFER *);

/*
 * Function  : c_buffer_require
 * Purpose   : make sure specified space is currently available in the buffer
 * Parameters: pointer to C_BUFFER
 *             length of space that must be available
 * Return    : zero on success
 * Notes     :
 *
 * 1. if the specified space is not availble it is acquired
 */
int c_buffer_require (C_BUFFER *, int length);

/*
 * Function  : c_buffer_append
 * Purpose   : appends data to a C_BUFFER
 * Parameters: pointer to C_BUFFER
 *             pointer to data to be appended
 *             length of data to be appended
 * Return    : zero on success
 */
int c_buffer_append (C_BUFFER *, char *, int length);

/*
 * Function  : c_buffer_append_str
 * Purpose   : appends a string to a C_BUFFER
 * Parameters: pointer to C_BUFFER
 *             pointer to a NULL terminated string
 * Return    : zero on success
 * Note      :
 *
 * 1. The NULL is not appended to the buffer
 */
int c_buffer_append_str (C_BUFFER *, char *);

/*
 * Function  : c_buffer_append_char
 * Purpose   : appends a char to a C_BUFFER
 * Parameters: pointer to C_BUFFER
 *             character to be appended
 * Return    : zero on success
 */
int c_buffer_append_char (C_BUFFER *, char);

/*
 * Function  : c_buffer_append_int
 * Purpose   : appends an integer to a C_BUFFER
 *
 *             The integer is converted to a string representation.
 *
 * Parameters: pointer to C_BUFFER
 *             character to be appended
 * Return    : zero on success
 */
int c_buffer_append_int (C_BUFFER *, int);

/*
 * Function  : c_buffer_shift
 * Purpose   : shifts (truncating) the contents of a buffer to the left
 *
 * Parameters: pointer to C_BUFFER
 *             number of bytes to shift
 * Return    : 0 on success
 *             1 if shift value less than or equal to zero
 * Note      :
 *
 * 1. If the shift value is greater than the length of the buffer, then the
 *    buffer is cleared.
 */
int c_buffer_shift (C_BUFFER *, int);

/*
 * Function  : c_buffer_clear
 * Purpose   : resets a C_BUFFER
 * Parameters: pointer to C_BUFFER
 * Return    : none
 * Note      :
 *
 * 1. The contents of the buffer is undefined after calling this function. The
 *    length of the buffer is zero, and subsequent calls to any of the append
 *    functions will begin at the start of the buffer.
 */
void c_buffer_clear (C_BUFFER *);

/*
 * Function  : c_buffer_get
 * Purpose   : returns a pointer to the start of the buffer
 * Parameters: pointer to C_BUFFER
 * Return    : a pointer to the start of the buffer
 * Note      :
 *
 * 1. This value is undefined after a call to any of c_buffer_free,
 *    c_buffer_clear or c_buffer_append*.
 */
char *c_buffer_get (C_BUFFER *);

/*
 * Function  : c_buffer_length
 * Purpose   : returns the length of the contents of the buffer
 * Parameters: pointer to C_BUFFER
 * Return    : the length of the contents of the buffer
 * Note      : (see c_buffer_get Note 1)
 */
int c_buffer_length (C_BUFFER *);

#endif
