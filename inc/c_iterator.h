#ifndef _C_ITERATOR_H
#define _C_ITERATOR_H

typedef struct C_ITERATOR C_ITERATOR;

/*
 * Function  : c_iterator_create
 * Purpose   : creates a new c_iterator
 * Parameters: a set of function pointers and a context (see Notes)
 * Return    : pointer to c_iterator
 * Notes     :
 *
 * 1. A C_ITERATOR wraps logic around a set of primitive operations
 *    on an underlying data set. The primitive operations are supplied
 *    by the specified function pointers. The idea is to reduce the trick(s)
 *    of stepping through a set of items to a few simple operations while
 *    leaving the more complex state management to a generic set of useful
 *    calls.
 *
 * 2. The 'init' function sets the iterator to the beginning of the set of
 *    items to iterate and returns a '1' if there is at least one item in
 *    the set.
 *
 * 3. The 'advance' function moves the iterator to the next item in the set
 *    and returns a '1' as long as the end of the set has not been reached.
 *
 * 4. The 'retrieve' function returns the value of the current item in the
 *    set. This function will be called at most one time after each
 *    successful call to 'advance'.
 *
 * 5. The 'remove' function removes the current item (last item retrieved)
 *    from the set. This function will be called at most one time after each
 *    successful call to 'retrieve'. After the remove is performed, the
 *    iterator points to the next item in the set, and the function returns
 *    a '1' as long as the end of the set has not been reached.
 *
 * 6. The 'free' function, if specified, is called when the c_iterator_free
 *    function is called, providing an opportunity to free up any resources
 *    associated with the iterator context.
 */
C_ITERATOR *c_iterator_create (
  int (*init) (void *),       /* return 0 if no items */
  int (*advance) (void *),    /* return 0 if no remaining items */
  void *(*retrieve) (void *), /* return current item */
  int (*remove) (void *),     /* remove current item, return 1 if more */
  void (*free) (void *),      /* free context resources (can be NULL) */
  void *context
);

/*
 * Function  : c_iterator_free
 * Purpose   : frees a c_iterator and its resources
 * Parameters: pointer to C_ITERATOR
 * Return    : none
 */
void c_iterator_free (C_ITERATOR *);

/*
 * Function  : c_iterator_reset
 * Purpose   : sets the c_iterator back to the beginning
 * Parameters: pointer to C_ITERATOR
 * Return    : none
 */
void c_iterator_reset (C_ITERATOR *);

/*
 * Function  : c_iterator_has_next
 * Purpose   : returns '1' if the c_iterator has more items
 * Parameters: pointer to C_ITERATOR
 * Return    : none
 */
int c_iterator_has_next (C_ITERATOR *);

/*
 * Function  : c_iterator_next
 * Purpose   : returns the next item in the set
 * Parameters: pointer to C_ITERATOR
 * Return    : next item in set, or NULL if no more items (see Note 2)
 * Note      :
 *
 * 1. The iterator is automatically advanced to the next item in the set.
 *
 * 2. If a set being iterated contains NULL items, then there is no way to
 *    tell if a NULL return from c_iterator_next means end-of-iterator or NULL
 *    item. In this case use the return from c_iterator_has_next to positively
 *    identify end-of-iterator.
 */
void *c_iterator_next (C_ITERATOR *);

/*
 * Function  : c_iterator_remove
 * Purpose   : removes the last item returned from the c_iterator from the set
 * Parameters: pointer to C_ITERATOR
 * Return    : none
 * Notes     : see Note 5 under c_iterator_create
 */
void c_iterator_remove (C_ITERATOR *);

#endif
