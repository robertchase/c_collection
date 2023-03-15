#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "hash_func.h"

int main (void) {

  u_long hash;
  char *string;

  string = "5690748";
  hash = hash_calculate (string, strlen (string));
  assert (hash == 2810351874u);
  
  string = "4513559";
  hash = hash_calculate (string, strlen (string));
  assert (hash == 921738403u);
  
  return 0;
}
