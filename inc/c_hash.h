#ifndef _C_HASH_H
#define _C_HASH_H

/*
 * A C_HASH implements a hash table. A C_HASH uses user-supplied hash and
 * comparison functions to efficiently manage a variable-sized hash table in
 * which values are stored.
 *
 * To minimize the use of malloc, the cell stored in the hash table is malloced
 * to include the space needed for user-specific data. The size of this user
 * data is one of the arguments to the hash create function. This means that
 * the user area can be any size, but it must be fixed. Variable length data
 * must be managed by the user, and may still result in multiple mallocs.
 *
 * To create a new C_HASH, use the c_hash_create function, supplying the hash,
 * comparison and cleanup (garbage) callback functions, as well as the size of
 * each item to be added to the table. To add a new value to the C_HASH,
 * use c_hash_insert or c_hash_replace. To locate an existing value, use the
 * c_hash_find function.
 *
 * The contents of the C_HASH can be (arbitrarily) traversed using the iterator
 * returned by the c_hash_iterator function. The iterator can be used multiple
 * times (with iterator_reset or by re-calling c_hash_iterator) without using
 * additional resources. The c_hash_free function will release any resources
 * associated with the iterator.
 *
 * Several convenience hash and comparison functions are provided in
 * u_hash_func.
 */

#define C_HASH_ERROR_MEMORY -1
#define C_HASH_ERROR_DUPLICATE -2
#define C_HASH_ERROR_NOT_FOUND -3

#include "c_iterator.h"

typedef struct C_HASH C_HASH;

/*
 * Typedef   : C_HASH_CALCULATOR
 * Purpose   : user callback that calculates a hash value from an item
 * Parameters: pointer to item
 *             context supplied in u_hash_create
 * Return    : hash value for item
 * Notes     :
 *
 * 1. If the C_HASH_COMPARATOR callback returns zero (equal) for two items,
 *    then the C_HASH_CALCULATOR callback must return the same hash for each
 *    item.
 *
 * 2. See u_hash_func_calculate and u_hash_func_accumulate for help in
 *    generating hash values.
 */
typedef unsigned int (*C_HASH_CALCULATOR) (void *item, void *context);

/*
 * Typedef   : C_HASH_COMPARATOR
 * Purpose   : user callback that compares two item
 * Parameters: pointer to first item
 *             pointer to second item
 *             context supplied in u_hash_create
 * Return    : 0 if keys are equal
 * Notes     : see C_HASH_CALCULATOR Note 1
 */
typedef int (*C_HASH_COMPARATOR) (void *item1, void *item2, void *context);

/*
 * Typedef   : C_HASH_GARBAGE
 * Purpose   : user callback collecting garbage when items are removed
 * Parameters: pointer to item
 *             context supplied in u_hash_create
 * Return    : none
 */
typedef void (*C_HASH_GARBAGE) (void *item, void *context);

/*
 * Typedef   : C_HASH_ITERATOR_ITEM
 * Purpose   : user callback that extracts an item for the iterator
 *
 *             The hash table manages a fixed size user area supplied during
 *             u_hash_insert or u_hash_replace. The size of this area is known,
 *             but the contents and layout are not. In order for an iterator
 *             to return a specific value from this user area, this callback
 *             is invoked, and whatever it returns is the result of the
 *             iterator-next call.
 *
 * Parameters: pointer to user defined variable
 * Return    : pointer to single value from user defined variable
 */
typedef void * (*C_HASH_ITERATOR_ITEM) (void *user_variable);

/*
 * Function  : c_hash_create
 * Purpose   : creates a new c_hash
 * Parameters: size of an item (Note 2)
 *             item hash calculator callback
 *             item comparison callback
 *             garbage collector (Note 1)
 *             context (supplied to callbacks; can be NULL)
 * Return    : C_HASH or NULL if out of memory
 * Notes     :
 *
 * 1. If the garbage collector is not NULL, it will be called each time
 *    a node is removed (c_hash_remove or c_hash_free), or when an item
 *    is replaced by a new item (c_hash_replace).
 *
 * 2. In order to manage items internally, the c_hash functions must rely on
 *    a pointer to the item, which is supplied in most of the function calls
 *    and a length, which is supplied here.
 */
C_HASH *c_hash_create (size_t, C_HASH_CALCULATOR, C_HASH_COMPARATOR,
   C_HASH_GARBAGE, void *);

/*
 * Function  : c_hash_free
 * Purpose   : frees a C_HASH and all internal resources
 * Parameters: pointer to C_HASH
 * Return    : none
 * Notes     : see c_hash_create Note 1
 */
void c_hash_free (C_HASH *);

/*
 * Function  : c_hash_clear
 * Purpose   : removes all items from the C_HASH
 * Parameters: pointer to C_HASH
 * Return    : none
 * Notes     : see c_hash_create Note 1
 */
void c_hash_clear (C_HASH *);

/*
 * Function  : c_hash_insert
 * Purpose   : adds an item to a C_HASH
 * Parameters: pointer to C_MAP
 *             pointer to an item
 * Return    : 0 on success
 *             U_HASH_ERROR_MEMORY
 *             U_HASH_ERROR_DUPLICATE on item already exists in C_HASH
 */
int c_hash_insert (C_HASH *, void *item);

/*
 * Function  : c_hash_replace
 * Purpose   : replaces an item already in a C_HASH with a new one
 * Parameters: pointer to C_HASH
 *             pointer to an item
 * Return    : 0 on success
 *             U_HASH_ERROR_NOT_FOUND on item not in C_HASH
 */
int c_hash_replace (C_HASH *, void *item);

/*
 * Function  : c_hash_find
 * Purpose   : finds a matching item in the C_HASH
 * Parameters: pointer to C_HASH
 *             pointer to item
 * Return    : pointer to matching item, or NULL if not found
 */
void *c_hash_find (C_HASH *, void *item);

/*
 * Function  : c_hash_remove
 * Purpose   : removes a matching item from the C_HASH
 * Parameters: pointer to C_HASH
 *             pointer to item
 * Return    : none
 */
void c_hash_remove (C_HASH *, void *item);

/*
 * Function  : c_hash_iterator
 * Purpose   : initializes or re-starts the C_HASH iterator
 * Parameters: pointer to C_HASH
 *             C_HASH_ITERATOR_ITEM (Note 3)
 * Return    : pointer to ITERATOR
 * Notes     :
 *
 * 1. The items in the C_HASH are not visited in any particular order.
 *
 * 2. The use of c_hash_insert, c_hash_replace or c_hash_remove while the
 *    iterator is still in use will produce unpredictable results. It is safe
 *    to remove the current item in the iterator using the iterator_remove
 *    function.
 *
 * 3. The iterator_next function returns the value returned from 
 *    C_HASH_ITERATOR_ITEM.
 *
 * 4. The data structures necessary for implementing an iterator on a
 *    C_HASH are wholly contained within the C_HASH and are reused if the
 *    iterator is traversed more than once. The c_hash_free function releases
 *    any resources related to an iterator on the C_HASH.
 *
 * 5. Subsequent calls to c_hash_iterator will not change the initial value
 *    of C_HASH_ITERATOR_ITEM; instead, the iterator will be reset to the
 *    beginning and the inital C_HASH_ITERATOR_ITEM will continue to be 
 *    called.
 */
C_ITERATOR *c_hash_iterator (C_HASH *, C_HASH_ITERATOR_ITEM);

/*
 * Function  : c_hash_size
 * Purpose   : returns the number of items in the C_HASH
 * Parameters: pointer to C_MAP
 * Return    : the number of items in the C_HASH
 */
int c_hash_size (C_HASH *);

/*
 * Function  : c_hash_table_size
 * Purpose   : returns the size of an internal table for testing purposes
 * Parameters: pointer to C_HASH
 * Return    : size of an internal table
 */
int c_hash_table_size (C_HASH *);
#endif
