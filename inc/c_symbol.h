#ifndef _C_SYMBOL_H
#define _C_SYMBOL_H

/*
 * A C_SYMBOL is a symbol table implementation holding at most one instance of
 * every string. Once a string is added to a C_SYMBOL, it does not change
 * memory location.
 *
 * To create a new C_SYMBOL, use the c_symbol_create function. To add a new
 * symbol (a new string), use the c_symbol_add function which returns a
 * pointer to the symbol. To locate an existing value, use the c_symbol_find
 * function.
 *
 * The contents of the C_SYMBOL can be (arbitrarily) traversed using the
 * C_ITERATOR returned from the c_symbol_iterator function.
 */

#include "c_iterator.h"

typedef struct C_SYMBOL C_SYMBOL;

/*
 * Function  : c_symbol_create
 * Purpose   : creates a new symbol table
 * Parameters: none
 * Return    : C_SYMBOL or NULL if out of memory
 */
C_SYMBOL *c_symbol_create (void);

/*
 * Function  : c_symbol_free
 * Purpose   : frees a C_SYMBOL and all internal resources
 * Parameters: pointer to C_SYMBOL
 * Return    : none
 */
void c_symbol_free (C_SYMBOL *);

/*
 * Function  : c_symbol_add
 * Purpose   : adds a string to a symbol table
 * Parameters: pointer to C_SYMBOL
 *             string
 * Return    : pointer to symbol (string) on success
 *             0 on out of memory
 * Notes     :
 *
 * 1. If the string is already in the symbol table, then a pointer to the
 *    existing symbol is returned; otherwise a new symbol is added and a
 *    pointer to THAT symbol is returned.
 */
char *c_symbol_add (C_SYMBOL *, char *string);

/*
 * Function  : c_symbol_find
 * Purpose   : finds the symbol representing a string
 * Parameters: pointer to C_SYMBOL
 *             string
 * Return    : pointer to symbol (string), or NULL if not found
 */
char *c_symbol_find (C_SYMBOL *, char *string);

/*
 * Function  : c_symbol_remove
 * Purpose   : removes a symbol from a table
 * Parameters: pointer to C_SYMBOL
 *             string
 * Return    : none
 */
void c_symbol_remove (C_SYMBOL *, char *string);

/*
 * Function  : c_symbol_clear
 * Purpose   : removes all symbols from the table
 * Parameters: pointer to C_SYMBOL
 * Return    : none
 */
void c_symbol_clear (C_SYMBOL *);

/*
 * Function  : c_symbol_iterate
 * Purpose   : sets the symbol table iterator to the beginning
 * Parameters: pointer to C_SYMBOL
 * Return    : pointer to C_ITERATOR
 * Notes     :
 *
 * 1. The symbols in the C_SYMBOL are not visited in any particular order.
 */
C_ITERATOR *c_symbol_iterator (C_SYMBOL *);

/*
 * Function  : c_symbol_size
 * Purpose   : returns the number of symbols in the table
 * Parameters: pointer to C_SYMBOL
 * Return    : the number of symbols in the table
 */
int c_symbol_size (C_SYMBOL *);

#endif
