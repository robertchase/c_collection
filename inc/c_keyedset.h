#ifndef _C_KEYEDSET_H
#define _C_KEYEDSET_H

#include "c_iterator.h"

/*
 * A C_KEYEDSET implements a container with the feature that each element
 * added to the container is assigned a unique key by which that and only
 * that element can be referenced (retrieved or deleted).
 *
 * To create a new C_KEYEDSET, use the c_keyedset_create function. To add a
 * new element, use the c_keyedset_add; use c_keyedset_remove to remove an
 * element by key. The key can be used to find an element within the set
 * by using c_keyedset_get.
 *
 * The contents of the C_KEYEDSET can be arbitrarily traversed using the
 * iterator from c_keyedset_iterator.
 *
 * When c_keyedset_remove is called directly or via a c_iterator, or when
 * c_keyedset_free is called, the internal resources are freed, but not the
 * stored elements themselves. The elements must be freed explicitly.
 */

typedef struct C_KEYEDSET C_KEYEDSET;

/*
 * Function  : c_keyedset_create
 * Purpose   : creates a new keyed set
 * Parameters: none
 * Return    : C_KEYEDSET or NULL if out of memory
 */
C_KEYEDSET *c_keyedset_create (void);

/*
 * Function  : c_keyedset_free
 * Purpose   : frees a C_KEYEDSET and all internal resources
 * Parameters: pointer to C_KEYEDSET
 * Return    : none
 */
void c_keyedset_free (C_KEYEDSET *);

/*
 * Function  : c_keyedset_add
 * Purpose   : adds a new element to a keyedset
 * Parameters: pointer to C_KEYEDSET
 *             pointer to element
 * Return    : key on success; otherwise, zero on out of memory
 */
int c_keyedset_add (C_KEYEDSET *, void *element);

/*
 * Function  : c_keyedset_get
 * Purpose   : finds the element associated with a key
 * Parameters: pointer to C_KEYEDSET
 *             key
 * Return    : pointer to element, or NULL if not found
 */
void *c_keyedset_get (C_KEYEDSET *, int key);

/*
 * Function  : c_keyedset_remove
 * Purpose   : removes an element identified by its key
 * Parameters: pointer to C_KEYEDSET
 *             key
 * Return    : none
 */
void c_keyedset_remove (C_KEYEDSET *, int key);

/*
 * Function  : c_keyedset_iterator
 * Purpose   : initializes or re-starts the C_KEYEDSET iterator
 * Parameters: pointer to C_KEYEDSET
 * Return    : pointer to C_ITERATOR
 * Note      :
 *
 * 1. The iterator does not return elements of the set in any particular order.
 *
 * 2. Every call to this function will return the same C_ITERATOR object.
 */
C_ITERATOR *c_keyedset_iterator (C_KEYEDSET *);

/*
 * Function  : c_keyedset_size
 * Purpose   : returns the number of elements in the set
 * Parameters: pointer to C_KEYEDSET
 * Return    : the number of elements in the set
 */
int c_keyedset_size (C_KEYEDSET *);
#endif
