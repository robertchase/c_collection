#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "c_list.h"

typedef struct LISTITEM LISTITEM;
struct LISTITEM {
  LISTITEM *prev;
  LISTITEM *next;
  void *value;
};

struct C_LIST {
  LISTITEM *head;
  LISTITEM *tail;
  int size;
  C_ITERATOR *iterator;
  LISTITEM *current;
};

static int
_itr_init (void *ctx) {
  C_LIST *l = (C_LIST *) ctx;
  l -> current = l -> head;
  return l -> current ? 1 : 0;
}

static int
_itr_advance (void *ctx) {
  C_LIST *l = (C_LIST *) ctx;
  l -> current = l -> current -> next;
  return l -> current ? 1 : 0;
}

static void *
_itr_retrieve (void *ctx) {
  C_LIST *l = (C_LIST *) ctx;
  return l -> current -> value;
}

static int
_itr_remove (void *ctx) {
  C_LIST *l = (C_LIST *) ctx;
  LISTITEM *next = l -> current -> next;
  LISTITEM *prev = l -> current -> prev;
  
  if (!prev) {
    l -> head = next;
  } else {
    prev -> next = next;
  }

  if (!next) {
    l -> tail = prev;
  } else {
    next -> prev = prev;
  }

  free (l -> current);
  l -> current = next;
  l -> size -= 1;
  return l -> current ? 1 : 0;
}

static void
_list_add (C_LIST *l, void *value, int append) {
  LISTITEM *i = (LISTITEM *) malloc (sizeof (LISTITEM));

  memset (i, 0x00, sizeof (LISTITEM));
  i -> value = value;
  if (0 == l -> size) {
    l -> head = l -> tail = i;
  } else {
    if (append) {
      l -> tail -> next = i;
      i -> prev = l -> tail;
      l -> tail = i;
    } else {
      l -> head -> prev = i;
      i -> next = l -> head;
      l -> head = i;
    }
  }
  l -> size += 1;
}

static void *
_list_take (C_LIST *l, int first) {
  LISTITEM *i;
  void *value = NULL;

  if (0 == l -> size) return NULL;

  if (first) {
    i = l -> head;
    l -> head = i -> next;
    if (l -> head) l -> head -> prev = NULL;
  } else {
    i = l -> tail;
    l -> tail = i -> prev;
    if (l -> tail) l -> tail -> next = NULL;
  }

  value = i -> value;
  free (i);
  l -> size -= 1;

  return value;
}

C_LIST *
c_list_create (void) {
  C_LIST *l = (C_LIST *) malloc (sizeof (C_LIST));
  if (l) {
    memset (l, 0x00, sizeof (C_LIST));
  }
  return l;
}

void
c_list_free (C_LIST *l) {
  LISTITEM *i;

  if (l) {
    for (i = l -> head; i; i = l -> head) {
      l -> head = i -> next;
      free (i);
    }
    c_iterator_free (l -> iterator);
    free (l);
  }

}

void
c_list_add (C_LIST *l, void *value) {
  _list_add (l, value, 1);
}

void
c_list_add_first (C_LIST *l, void *value) {
  _list_add (l, value, 0);
}

void *
c_list_take (C_LIST *l) {
  return _list_take (l, 1);
}

void *
c_list_take_last (C_LIST *l) {
  return _list_take (l, 0);
}

C_ITERATOR *
c_list_iterator (C_LIST *l) {
  if (l -> iterator) {
    c_iterator_reset (l -> iterator);
  } else {
    l -> iterator = c_iterator_create (
      _itr_init,
      _itr_advance,
      _itr_retrieve,
      _itr_remove,
      0,
      (void *) l
    );
  }
  return l -> iterator;
}

int
c_list_size (C_LIST *l) {
  return l -> size;
}
