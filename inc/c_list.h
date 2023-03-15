#ifndef _C_LIST_H
#define _C_LIST_H

#include "c_iterator.h"

typedef struct C_LIST C_LIST;

C_LIST * c_list_create (void);
void c_list_free (C_LIST *);

void c_list_add (C_LIST *, void *); /* append */
void c_list_add_first (C_LIST *, void *);
void *c_list_take (C_LIST *); /* first item (lifo) */
void *c_list_take_last (C_LIST *);

C_ITERATOR *c_list_iterator (C_LIST *);

int c_list_size (C_LIST *);

#endif
