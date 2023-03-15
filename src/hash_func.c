#include <ctype.h>
#include <string.h>
#include "hash_func.h"
#include "fnv.h"

unsigned int
hash_func_calculate (void *value, size_t length) {
  return (unsigned int) fnv_32_buf (value, length, FNV1_32_INIT);
}

unsigned int
hash_func_accumulate (void *value, size_t length, unsigned int hash) {
  return (unsigned int) fnv_32_buf (value, length, (u_long) hash);
}

int
hash_string_comparator (void *s1, void *s2) {
  return strcmp ((char *) s1, (char *) s2);
}

int
hash_nocase_string_comparator (void *s1, void *s2) {
  int i = 0;
  char *str1 = (char *) s1;
  char *str2 = (char *) s2;
  char c1;
  char c2;

  for (i = 0;; i++) {

    if (str1 [i] == 0x00 && str2 [i] == 0x00)
      return 0;

    if (str2 [i] == 0x00)
      return -1;

    if (str1 [i] == 0x00)
      return 1;

    c1 = tolower (str1 [i]);
    c2 = tolower (str2 [i]);

    if (c1 > c2)
      return -1;

    if (c1 < c2)
      return 1;

  }

  return 0;
}

unsigned int
hash_string_calculator (void *string) {
  return hash_func_calculate ((void *) string, strlen ((char *) string));
}

unsigned int
hash_nocase_string_calculator (void *s) {
  unsigned int hash = 0;
  char *str = (char *) s;
  char c;
  int i;
  int len = strlen (str);

  for (i = 0; i < len; i ++) {
    c = tolower (str [i]);
    hash = hash_func_calculate (&c, 1);
  }

  return hash;
}

int
hash_uint_comparator (void *v1, void *v2) {
  unsigned int a = * (unsigned int *) v1;
  unsigned int b = * (unsigned int *) v2;
  if (a == b) return 0;
  if (a > b) return -1;
  return 1;
}

unsigned int
hash_uint_calculator (void *value) {
  return * (unsigned int *) value;
}
