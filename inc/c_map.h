#ifndef _C_MAP_H
#define _C_MAP_H

/*
 * A C_MAP implements a mapping between a key and a value. A C_MAP uses
 * user-supplied hash and comparison functions to efficiently manage a
 * variable-sized hash table in which keys and values are stored.
 *
 * To create a new C_MAP, use the c_map_create function, supplying the hash,
 * comparison and cleanup (garbage) callback functions. To add a new key/value
 * pair to the C_MAP, use c_map_add. To locate an existing value, use the
 * c_map_find function. To find a specific key, use the c_map_find_key
 * function. To determine if a key exists in the C_MAP, use the c_map_exists
 * function. To remove a key/value pair from the C_MAP, use the c_map_remove
 * function.
 *
 * The contents of the C_MAP can be (arbitrarily) traversed using the iterator
 * returned by the c_map_iterator function. The iterator can be used multiple
 * times (with iterator_reset or by re-calling c_map_iterator) without using
 * additional resources. The c_map_free function will release any resources
 * associated with the iterator.
 *
 * Several convenience hash and comparison functions are provided in
 * u_hash_func. The c_map_dict_create is a convenience constructor which
 * creates a C_MAP with a string (null terminated) key.
 */

#include "c_iterator.h"

typedef struct C_MAP C_MAP;

/*
 * Typedef   : C_MAPITEM
 * Purpose   : returned by iterator
 */
typedef struct C_MAPITEM {
  void *key;
  void *value;
} C_MAPITEM;

/*
 * Typedef   : C_MAP_CALCULATOR
 * Purpose   : user callback that calculates a hash value from a key
 * Parameters: pointer to key
 * Return    : hash value for key
 * Notes     :
 *
 * 1. If the C_MAP_COMPARATOR callback returns zero (equal) for two keys, then
 *    the C_MAP_CALCULATOR callback must return the same hash for each key.
 *
 * 2. See u_hash_func_calculate and u_hash_func_accumulate for help in
 *    generating hash values.
 */
typedef unsigned int (*C_MAP_CALCULATOR) (void *key);

/*
 * Typedef   : C_MAP_COMPARATOR
 * Purpose   : user callback that compares two keys
 * Parameters: pointer to first key
 *             pointer to second key
 * Return    : 0 if keys are equal
 * Notes     : see C_MAP_CALCULATOR Note 1
 */
typedef int (*C_MAP_COMPARATOR) (void *k1, void *k2);

/*
 * Typedef   : C_MAP_GARBAGE
 * Purpose   : user callback collecting garbage when items are removed
 * Parameters: pointer to key (may be NULL)
 *             pointer to value (may be NULL)
 * Return    : none
 */
typedef void (*C_MAP_GARBAGE) (void *key, void *value);

/*
 * Function  : c_map_create
 * Purpose   : creates a new c_map
 * Parameters: key hash calculator callback
 *             key comparison callback
 *             garbage collector (Note 1)
 * Return    : C_MAP or NULL if out of memory
 * Notes     :
 *
 * 1. If the garbage collector is not NULL, it will be called each time
 *    a node is removed (c_map_remove or c_map_free) or when the key and
 *    value are replaced (c_map_add) in order to provide opportunity
 *    to free any resources associated with the user supplied data.
 *
 * 2. The c_map_add function may call the garbabge collector:
 *
 *      In the case where the key of the adding item matches an existing key
 *      in the map, and they share the same memory location, then the garbage
 *      collector is called with the existing value. If the values share the
 *      same memory location, then the garbage collector is not called.
 *
 *      In the case where the key of the adding item matches an existing key
 *      in the map, and the memory locations are different, the garbage
 *      collector is called with the new key and the old value. If the values
 *      share the same memory location, then the garbage collector is called
 *      with only the new key.
 */
C_MAP *c_map_create (C_MAP_CALCULATOR, C_MAP_COMPARATOR, C_MAP_GARBAGE);

/*
 * Function  : c_map_dict_create
 * Purpose   : creates a new c_map with a null-terminated string key
 * Parameters: garbage collector (see c_map_create Note 1)
 * Return    : C_MAP or NULL if out of memory
 */
C_MAP *c_map_dict_create (C_MAP_GARBAGE);

/*
 * Function  : c_map_free
 * Purpose   : frees a C_MAP and all internal resources
 * Parameters: pointer to C_MAP
 * Return    : none
 * Notes     : see c_map_create Note 1
 */
void c_map_free (C_MAP *);

/*
 * Function  : c_map_clear
 * Purpose   : removes all items from the C_MAP
 * Parameters: pointer to C_MAP
 * Return    : none
 * Notes     : see c_map_create Note 1
 */
void c_map_clear (C_MAP *);

/*
 * Function  : c_map_add
 * Purpose   : adds a key-value pair to a c_map
 * Parameters: pointer to C_MAP
 *             pointer to key
 *             pointer to value
 * Return    : 0 on success; otherwise, out of memory
 * Notes     :
 *
 * 1. If the key is already in the c_map , then the associated key and
 *    value are replaced. If a garbage collector is defined it will be called
 *    with the new key and old value. See c_map_create Note 1. If the new key
 *    matches the old key (same memory location), then the garbage collector
 *    will be called with a NULL key.
 */
int c_map_add (C_MAP *, void *key, void *value);

/*
 * Function  : c_map_find
 * Purpose   : finds the value associated with a key
 * Parameters: pointer to C_MAP
 *             pointer to key
 * Return    : pointer to value, or NULL if not found (Note 1)
 * Notes     :
 *
 * 1. If NULL is supplied as the value for c_map entries, then the value 'NULL'
 *    would be indistinquishable from the 'no value found' indicator. In this
 *    case, c_map_exists must be used to tell if an entry exists.
 */
void *c_map_find (C_MAP *, void *key);

/*
 * Function  : c_map_find_key
 * Purpose   : returns the key that matches the supplied key
 *             (useful when treating the c_map keys as a Set)
 * Parameters: pointer to C_MAP
 *             pointer to key
 * Return    : pointer to matching key, or NULL if not found (Note 1)
 * Notes     :
 *
 * 1. If NULL is supplied as the key for c_map entries, then the value 'NULL'
 *    would be indistinquishable from the 'no key found' indicator. In this
 *    case, c_map_exists must be used to tell if an entry exists.
 */
void *c_map_find_key (C_MAP *, void *key);

/*
 * Function  : c_map_exists
 * Purpose   : indicates if a key exists in the c_map
 * Parameters: pointer to C_MAP
 *             pointer to key
 * Return    : non-zero if the key is found
 * Notes     : see c_map_find Note 1
 */
int c_map_exists (C_MAP *, void *key);

/*
 * Function  : c_map_remove
 * Purpose   : removes a key-value pair
 * Parameters: pointer to C_MAP
 *             pointer to key
 * Return    : none
 * Notes     : see c_map_create Note 1
 */
void c_map_remove (C_MAP *, void *key);

/*
 * Function  : c_map_iterator
 * Purpose   : initializes or re-starts the C_MAP iterator
 * Parameters: pointer to C_MAP
 * Return    : pointer to ITERATOR
 * Notes     :
 *
 * 1. The key/value pairs in the C_MAP are not visited in any particular
 *    order.
 *
 * 2. The use of c_map_add or c_map_remove while the iterator is still
 *    in use will produce unpredictable results. It is safe to remove the
 *    current item in the iterator using the iterator_remove function.
 *
 * 3. The iterator_next function returns a C_MAPITEM pointer.
 *
 * 4. The data structures necessary for implementing an iterator on a
 *    C_MAP are wholly contained within the C_MAP and are reused if the
 *    iterator is traversed more than once. The c_map_free function releases
 *    any resources related to an iterator on the C_MAP.
 */
C_ITERATOR *c_map_iterator (C_MAP *);

/*
 * Function  : c_map_key_iterator
 * Purpose   : creates an iterator across the C_MAP keys
 * Parameters: pointer to C_MAP
 * Return    : pointer to ITERATOR
 * Notes     :
 *
 * 1. This is identical to a c_map_iterator except that a pointer to the
 *    C_MAPITEM.key is returned instead of the C_MAPITEM itself. Whichever
 *    iterator is created first (key, value or C_MAPITEM) will remain the
 *    iterator for the life of the C_MAP; in other words, you can't switch.
 */
C_ITERATOR *c_map_key_iterator (C_MAP *);

/*
 * Function  : c_map_value_iterator
 * Purpose   : creates an iterator across the C_MAP values
 * Parameters: pointer to C_MAP
 * Return    : pointer to ITERATOR
 * Notes     :
 *
 * 1. See c_map_value_iterator.
 */
C_ITERATOR *c_map_value_iterator (C_MAP *);

/*
 * Function  : c_map_size
 * Purpose   : returns the number of key-value pairs in the c_map
 * Parameters: pointer to C_MAP
 * Return    : the number of key-value pairs in the c_map
 */
int c_map_size (C_MAP *);

/*
 * Function  : c_map_table_size
 * Purpose   : returns the size of an internal table for testing purposes
 * Parameters: pointer to C_MAP
 * Return    : size of an internal table
 */
int c_map_table_size (C_MAP *);
#endif
