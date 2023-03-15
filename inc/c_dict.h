#ifndef _C_DICT_H
#define _C_DICT_H

/*
 * A C_DICT implements a string-based dictionary. Both the key and the value
 * are null terminated strings that are copied and maintained by the C_DICT
 * on the c_dict_add function.
 *
 * To create a new C_DICT, use the c_dict_create function. To add a new
 * key/value to the C_DICT, use c_dict_add. To locate an existing value,
 * use c_dict_find, providing the key. To remove a key/value pair, use
 * c_dict_remove.
 *
 * The contents of the C_DICT can be (arbitrarily) traversed using the iterator
 * returned by the c_dict_iterator function. The iterator can be used multiple
 * times (with iterator_reset or by re-calling c_dict_iterator) without using
 * additional resources. The c_dict_free function will release any resources
 * associated with the iterator.
 */

#include "c_map.h"
#include "c_iterator.h"

typedef struct C_DICT C_DICT;

/*
 * Typedef   : C_DICTITEM
 * Purpose   : returned by iterator
 */
typedef C_MAPITEM C_DICTITEM;

/*
 * Function  : c_dict_create
 * Purpose   : creates a new c_dict
 * Parameters: none
 * Return    : C_DICT or NULL if out of memory
 */
C_DICT *c_dict_create (void);

/*
 * Function  : c_dict_free
 * Purpose   : frees a C_DICT and all internal resources
 * Parameters: pointer to C_DICT
 * Return    : none
 */
void c_dict_free (C_DICT *);

/*
 * Function  : c_dict_clear
 * Purpose   : removes all items from the C_DICT
 * Parameters: pointer to C_DICT
 * Return    : none
 */
void c_dict_clear (C_DICT *);

/*
 * Function  : c_dict_add
 * Purpose   : adds a key-value pair to a c_map
 * Parameters: pointer to C_DICT
 *             pointer to key string (cannot be 0 length)
 *             pointer to value string (cannot be 0 length)
 * Return    : 0 on success; otherwise, out of memory or 0 length key/value
 */
int c_dict_add (C_DICT *, char *key, char *value);

/*
 * Function  : c_dict_find
 * Purpose   : finds the value associated with a key
 * Parameters: pointer to C_DICT
 *             pointer to key
 * Return    : pointer to value, or NULL if not found
 */
char *c_dict_find (C_DICT *, char *key);

/*
 * Function  : c_dict_remove
 * Purpose   : removes a key-value pair
 * Parameters: pointer to C_DICT
 *             pointer to key
 * Return    : none
 */
void c_dict_remove (C_DICT *, char *key);

/*
 * Function  : c_dict_iterator
 * Purpose   : initializes or re-starts the C_DICT iterator
 * Parameters: pointer to C_DICT
 * Return    : pointer to ITERATOR
 * Notes     :
 *
 * 1. The key/value pairs in the C_DICT are not visited in any particular
 *    order.
 *
 * 2. The use of c_dict_add or c_dict_remove while the iterator is still
 *    in use will produce unpredictable results. It is safe to remove the
 *    current item in the iterator using the iterator_remove function.
 *
 * 3. The iterator_next function returns a C_DICTITEM pointer.
 *
 * 4. The data structures necessary for implementing an iterator on a
 *    C_DICT are wholly contained within the C_DICT and are reused if the
 *    iterator is traversed more than once. The c_dict_free function releases
 *    any resources related to an iterator on the C_DICT.
 */
C_ITERATOR *c_dict_iterator (C_DICT *);

/*
 * Function  : c_dict_size
 * Purpose   : returns the number of key-value pairs in the c_dict
 * Parameters: pointer to C_DICT
 * Return    : the number of key-value pairs in the c_dict
 */
int c_dict_size (C_DICT *);

#endif
