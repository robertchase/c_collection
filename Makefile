SRC := src
INC := inc
TEST := test
OBJ := obj
SHARED_LIB := $(HOME)/lib
SHARED_INC := $(HOME)/inc
SHARED_BIN := $(HOME)/bin

IFLAGS := -I $(INC) -I $(SHARED_INC)
CFLAGS := -g -O -Wuninitialized -Werror -Wall -Wmissing-prototypes -Wmissing-declarations -Wstrict-prototypes -Wunused
LFLAGS := 

c_collection.a: $(OBJ)/fnv.o $(OBJ)/hash_func.o $(OBJ)/c_buffer.o $(OBJ)/c_hash.o $(OBJ)/c_iterator.o $(OBJ)/c_keyedset.o $(OBJ)/c_list.o $(OBJ)/c_map.o $(OBJ)/c_symbol.o
	$(AR) ru c_collection.a $(OBJ)/fnv.o $(OBJ)/hash_func.o $(OBJ)/c_buffer.o $(OBJ)/c_hash.o $(OBJ)/c_iterator.o $(OBJ)/c_keyedset.o $(OBJ)/c_list.o $(OBJ)/c_map.o $(OBJ)/c_symbol.o
	ranlib c_collection.a

$(OBJ)/fnv.o: $(SRC)/fnv.c $(INC)/fnv.h
	gcc $(CFLAGS) $(IFLAGS) -c $< -o $@

$(OBJ)/hash_func.o: $(SRC)/hash_func.c $(SHARED_INC)/hash_func.h $(INC)/fnv.h
	gcc $(CFLAGS) $(IFLAGS) -c $< -o $@

$(OBJ)/c_buffer.o: $(SRC)/c_buffer.c $(SHARED_INC)/c_buffer.h
	gcc $(CFLAGS) $(IFLAGS) -c $< -o $@

$(OBJ)/c_hash.o: $(SRC)/c_hash.c $(SHARED_INC)/c_list.h \
  $(SHARED_INC)/c_iterator.h $(SHARED_INC)/c_hash.h
	gcc $(CFLAGS) $(IFLAGS) -c $< -o $@

$(OBJ)/c_iterator.o: $(SRC)/c_iterator.c $(SHARED_INC)/c_iterator.h
	gcc $(CFLAGS) $(IFLAGS) -c $< -o $@

$(OBJ)/c_keyedset.o: $(SRC)/c_keyedset.c $(SHARED_INC)/c_hash.h \
  $(SHARED_INC)/c_iterator.h $(SHARED_INC)/c_keyedset.h \
  $(SHARED_INC)/hash_func.h
	gcc $(CFLAGS) $(IFLAGS) -c $< -o $@

$(OBJ)/c_list.o: $(SRC)/c_list.c $(SHARED_INC)/c_list.h \
  $(SHARED_INC)/c_iterator.h
	gcc $(CFLAGS) $(IFLAGS) -c $< -o $@

$(OBJ)/c_map.o: $(SRC)/c_map.c $(SHARED_INC)/c_hash.h $(SHARED_INC)/c_iterator.h \
  $(SHARED_INC)/c_map.h $(SHARED_INC)/hash_func.h
	gcc $(CFLAGS) $(IFLAGS) -c $< -o $@

$(OBJ)/c_symbol.o: $(SRC)/c_symbol.c $(SHARED_INC)/c_hash.h \
  $(SHARED_INC)/c_iterator.h $(SHARED_INC)/c_symbol.h \
  $(SHARED_INC)/hash_func.h
	gcc $(CFLAGS) $(IFLAGS) -c $< -o $@

$(OBJ)/test_c_buffer.o: $(TEST)/test_c_buffer.c $(SHARED_INC)/c_buffer.h

	gcc $(CFLAGS) $(IFLAGS) -c $< -o $@

test_c_buffer: $(OBJ)/test_c_buffer.o c_collection.a
	gcc $(OBJ)/test_c_buffer.o c_collection.a $(LFLAGS) -o $@

$(OBJ)/test_c_hash.o: $(TEST)/test_c_hash.c $(SHARED_INC)/c_hash.h \
  $(SHARED_INC)/c_iterator.h $(SHARED_INC)/hash_func.h

	gcc $(CFLAGS) $(IFLAGS) -c $< -o $@

test_c_hash: $(OBJ)/test_c_hash.o c_collection.a
	gcc $(OBJ)/test_c_hash.o c_collection.a $(LFLAGS) -o $@

$(OBJ)/test_c_iterator.o: $(TEST)/test_c_iterator.c $(SHARED_INC)/c_iterator.h

	gcc $(CFLAGS) $(IFLAGS) -c $< -o $@

test_c_iterator: $(OBJ)/test_c_iterator.o c_collection.a
	gcc $(OBJ)/test_c_iterator.o c_collection.a $(LFLAGS) -o $@

$(OBJ)/test_c_keyedset.o: $(TEST)/test_c_keyedset.c $(SHARED_INC)/c_keyedset.h \
  $(SHARED_INC)/c_iterator.h $(SHARED_INC)/c_map.h

	gcc $(CFLAGS) $(IFLAGS) -c $< -o $@

test_c_keyedset: $(OBJ)/test_c_keyedset.o c_collection.a
	gcc $(OBJ)/test_c_keyedset.o c_collection.a $(LFLAGS) -o $@

$(OBJ)/test_c_list.o: $(TEST)/test_c_list.c $(SHARED_INC)/c_list.h \
  $(SHARED_INC)/c_iterator.h

	gcc $(CFLAGS) $(IFLAGS) -c $< -o $@

test_c_list: $(OBJ)/test_c_list.o c_collection.a
	gcc $(OBJ)/test_c_list.o c_collection.a $(LFLAGS) -o $@

$(OBJ)/test_c_map.o: $(TEST)/test_c_map.c $(SHARED_INC)/c_map.h \
  $(SHARED_INC)/c_iterator.h $(SHARED_INC)/hash_func.h

	gcc $(CFLAGS) $(IFLAGS) -c $< -o $@

test_c_map: $(OBJ)/test_c_map.o c_collection.a
	gcc $(OBJ)/test_c_map.o c_collection.a $(LFLAGS) -o $@

$(OBJ)/test_c_symbol.o: $(TEST)/test_c_symbol.c $(SHARED_INC)/c_iterator.h \
  $(SHARED_INC)/c_symbol.h

	gcc $(CFLAGS) $(IFLAGS) -c $< -o $@

test_c_symbol: $(OBJ)/test_c_symbol.o c_collection.a
	gcc $(OBJ)/test_c_symbol.o c_collection.a $(LFLAGS) -o $@

test: test_c_buffer test_c_hash test_c_iterator test_c_keyedset test_c_list test_c_map test_c_symbol c_collection.a
	./test_c_buffer
	rm test_c_buffer
	./test_c_hash
	rm test_c_hash
	./test_c_iterator
	rm test_c_iterator
	./test_c_keyedset
	rm test_c_keyedset
	./test_c_list
	rm test_c_list
	./test_c_map
	rm test_c_map
	./test_c_symbol
	rm test_c_symbol

install: c_collection.a
	-mkdir $(SHARED_LIB) 2>/dev/null || true
	-cp c_collection.a $(SHARED_LIB)/
	-mkdir $(SHARED_INC) 2>/dev/null || true
	-cp $(INC)/hash_func.h $(SHARED_INC)/
	-cp $(INC)/c_buffer.h $(SHARED_INC)/
	-cp $(INC)/c_hash.h $(SHARED_INC)/
	-cp $(INC)/c_iterator.h $(SHARED_INC)/
	-cp $(INC)/c_keyedset.h $(SHARED_INC)/
	-cp $(INC)/c_list.h $(SHARED_INC)/
	-cp $(INC)/c_map.h $(SHARED_INC)/
	-cp $(INC)/c_symbol.h $(SHARED_INC)/

clean:
	-rm -f c_collection.a
	-rm -f $(OBJ)/fnv.o
	-rm -f $(OBJ)/hash_func.o
	-rm -f $(OBJ)/c_buffer.o
	-rm -f $(OBJ)/c_hash.o
	-rm -f $(OBJ)/c_iterator.o
	-rm -f $(OBJ)/c_keyedset.o
	-rm -f $(OBJ)/c_list.o
	-rm -f $(OBJ)/c_map.o
	-rm -f $(OBJ)/c_symbol.o
	-rm -f $(OBJ)/test_c_buffer.o
	-rm -f $(OBJ)/test_c_hash.o
	-rm -f $(OBJ)/test_c_iterator.o
	-rm -f $(OBJ)/test_c_keyedset.o
	-rm -f $(OBJ)/test_c_list.o
	-rm -f $(OBJ)/test_c_map.o
	-rm -f $(OBJ)/test_c_symbol.o
	-rm -f test_c_buffer
	-rm -f test_c_hash
	-rm -f test_c_iterator
	-rm -f test_c_keyedset
	-rm -f test_c_list
	-rm -f test_c_map
	-rm -f test_c_symbol
