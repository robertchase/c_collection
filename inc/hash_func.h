#ifndef _HASH_FNV_H
#define _HASH_FNV_H

/*
 * A collection of convenience functions for calculating hash values and for
 * handling simple HASH keys.
 *
 * To calculate a hash value for an area of memory, use the hash_calculate
 * function. To calculate a hash value for several non-contiguous areas of
 * memory, use the hash_accumulate function, passing a previously calculated
 * hash value.
 *
 * To create a HASH with a simple key composed of a string, a string without
 * respect to case, or an unsigned int, use the repectively named convenience
 * functions for the HASH_CALCULATOR and HASH_COMPARATOR arguments to the
 * hash_create function.
 */

#include <stdlib.h>
#include <sys/types.h>

unsigned int hash_func_calculate (void *value, size_t length);
unsigned int hash_func_accumulate (void *value, size_t length, unsigned int hash);

int hash_string_comparator (void *s1, void *s2);
unsigned int hash_string_calculator (void *string);

int hash_nocase_string_comparator (void *s1, void *s2);
unsigned int hash_nocase_string_calculator (void *string);

int hash_uint_comparator (void *v1, void *v2);
unsigned int hash_uint_calculator (void *value);

#endif
