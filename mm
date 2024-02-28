BUILD c_collection.a

CFLAGS -g
#CFLAGS -O -Wuninitialized
CFLAGS -Werror -Wall -Wmissing-prototypes -Wmissing-declarations -Wstrict-prototypes -Wunused

SOURCE fnv.c
SOURCE hash_func.c

SOURCE c_buffer.c
SOURCE c_hash.c
SOURCE c_iterator.c
SOURCE c_keyedset.c
SOURCE c_list.c
SOURCE c_map.c
SOURCE c_symbol.c

TEST test_c_buffer.c
TEST test_c_hash.c
TEST test_c_iterator.c
TEST test_c_keyedset.c
TEST test_c_list.c
TEST test_c_map.c
TEST test_c_symbol.c

INSTALL hash_func.h

INSTALL c_buffer.h
INSTALL c_hash.h
INSTALL c_iterator.h
INSTALL c_keyedset.h
INSTALL c_list.h
INSTALL c_map.h
INSTALL c_symbol.h
