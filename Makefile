IFLAGS = -I inc -I /Users/bob/inc
CFLAGS = -g -Werror -Wall -Wmissing-prototypes -Wmissing-declarations -Wstrict-prototypes -Wunused

collection.a: obj/fnv.o obj/hash_func.o obj/c_buffer.o obj/c_hash.o obj/c_iterator.o obj/c_keyedset.o obj/c_list.o obj/c_map.o obj/c_symbol.o
	$(AR) ru collection.a obj/fnv.o obj/hash_func.o obj/c_buffer.o obj/c_hash.o obj/c_iterator.o obj/c_keyedset.o obj/c_list.o obj/c_map.o obj/c_symbol.o
	ranlib collection.a

obj/fnv.o: src/fnv.c inc/fnv.h
	gcc $(CFLAGS) $(IFLAGS) -c $< -o $@

obj/hash_func.o: src/hash_func.c inc/hash_func.h inc/fnv.h
	gcc $(CFLAGS) $(IFLAGS) -c $< -o $@

obj/c_buffer.o: src/c_buffer.c inc/c_buffer.h
	gcc $(CFLAGS) $(IFLAGS) -c $< -o $@

obj/c_hash.o: src/c_hash.c inc/c_list.h inc/c_iterator.h inc/c_hash.h
	gcc $(CFLAGS) $(IFLAGS) -c $< -o $@

obj/c_iterator.o: src/c_iterator.c inc/c_iterator.h
	gcc $(CFLAGS) $(IFLAGS) -c $< -o $@

obj/c_keyedset.o: src/c_keyedset.c inc/c_hash.h inc/c_iterator.h \
  inc/c_keyedset.h inc/hash_func.h
	gcc $(CFLAGS) $(IFLAGS) -c $< -o $@

obj/c_list.o: src/c_list.c inc/c_list.h inc/c_iterator.h
	gcc $(CFLAGS) $(IFLAGS) -c $< -o $@

obj/c_map.o: src/c_map.c inc/c_hash.h inc/c_iterator.h inc/c_map.h \
  inc/hash_func.h
	gcc $(CFLAGS) $(IFLAGS) -c $< -o $@

obj/c_symbol.o: src/c_symbol.c inc/c_hash.h inc/c_iterator.h inc/c_symbol.h \
  inc/hash_func.h
	gcc $(CFLAGS) $(IFLAGS) -c $< -o $@

obj/test_c_buffer.o: test/test_c_buffer.c inc/c_buffer.h
	gcc $(CFLAGS) $(IFLAGS) -c $< -o $@

obj/test_c_hash.o: test/test_c_hash.c inc/c_hash.h inc/c_iterator.h \
  inc/hash_func.h
	gcc $(CFLAGS) $(IFLAGS) -c $< -o $@

obj/test_c_iterator.o: test/test_c_iterator.c inc/c_iterator.h
	gcc $(CFLAGS) $(IFLAGS) -c $< -o $@

obj/test_c_keyedset.o: test/test_c_keyedset.c inc/c_keyedset.h \
  inc/c_iterator.h inc/c_iterator.h inc/c_map.h
	gcc $(CFLAGS) $(IFLAGS) -c $< -o $@

obj/test_c_list.o: test/test_c_list.c inc/c_list.h inc/c_iterator.h
	gcc $(CFLAGS) $(IFLAGS) -c $< -o $@

obj/test_c_map.o: test/test_c_map.c inc/c_map.h inc/c_iterator.h \
  inc/hash_func.h
	gcc $(CFLAGS) $(IFLAGS) -c $< -o $@

obj/test_c_symbol.o: test/test_c_symbol.c inc/c_iterator.h inc/c_symbol.h \
  inc/c_iterator.h
	gcc $(CFLAGS) $(IFLAGS) -c $< -o $@

clean: 
	-rm -f collection.a
	-rm -f obj/fnv.o
	-rm -f obj/hash_func.o
	-rm -f obj/c_buffer.o
	-rm -f obj/c_hash.o
	-rm -f obj/c_iterator.o
	-rm -f obj/c_keyedset.o
	-rm -f obj/c_list.o
	-rm -f obj/c_map.o
	-rm -f obj/c_symbol.o
	-rm -f obj/test_c_buffer.o
	-rm -f obj/test_c_hash.o
	-rm -f obj/test_c_iterator.o
	-rm -f obj/test_c_keyedset.o
	-rm -f obj/test_c_list.o
	-rm -f obj/test_c_map.o
	-rm -f obj/test_c_symbol.o
	-rm -f test_c_buffer
	-rm -f test_c_hash
	-rm -f test_c_iterator
	-rm -f test_c_keyedset
	-rm -f test_c_list
	-rm -f test_c_map
	-rm -f test_c_symbol

test: test_c_buffer test_c_hash test_c_iterator test_c_keyedset test_c_list test_c_map test_c_symbol collection.a
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

test_c_buffer: obj/test_c_buffer.o collection.a
	gcc obj/test_c_buffer.o collection.a $(LFLAGS) -o $@

test_c_hash: obj/test_c_hash.o collection.a
	gcc obj/test_c_hash.o collection.a $(LFLAGS) -o $@

test_c_iterator: obj/test_c_iterator.o collection.a
	gcc obj/test_c_iterator.o collection.a $(LFLAGS) -o $@

test_c_keyedset: obj/test_c_keyedset.o collection.a
	gcc obj/test_c_keyedset.o collection.a $(LFLAGS) -o $@

test_c_list: obj/test_c_list.o collection.a
	gcc obj/test_c_list.o collection.a $(LFLAGS) -o $@

test_c_map: obj/test_c_map.o collection.a
	gcc obj/test_c_map.o collection.a $(LFLAGS) -o $@

test_c_symbol: obj/test_c_symbol.o collection.a
	gcc obj/test_c_symbol.o collection.a $(LFLAGS) -o $@

install: collection.a 
	-cp collection.a $(HOME)/lib/
	-mkdir $(HOME)/inc/collection
	-cp inc/hash_func.h $(HOME)/inc/collection/
	-cp inc/c_buffer.h $(HOME)/inc/collection/
	-cp inc/c_hash.h $(HOME)/inc/collection/
	-cp inc/c_iterator.h $(HOME)/inc/collection/
	-cp inc/c_keyedset.h $(HOME)/inc/collection/
	-cp inc/c_list.h $(HOME)/inc/collection/
	-cp inc/c_map.h $(HOME)/inc/collection/
	-cp inc/c_symbol.h $(HOME)/inc/collection/
