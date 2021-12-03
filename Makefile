
APP_NAME = hufpakk
TEST_APPS = dictionary_test huffman_test
TEST_FILE = proov.hup proov1.txt test

CC = gcc
CFLAGS = -std=c99 -g


main: dictionary.o huffman.o main.c
	$(CC) $(CFLAGS) main.c huffman.o dictionary.o -o $(APP_NAME)


run_test: test
	./dictionary_test
	./huffman_test

test: dictionary_test huffman_test

dictionary_test: dictionary.o dictionary_test.c
	$(CC) $(CFLAGS) dictionary_test.c dictionary.o -o dictionary_test

huffman_test: dictionary.o huffman.o huffman_test.c
	$(CC) $(CFLAGS) huffman_test.c huffman.o dictionary.o -o huffman_test

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

#gcc -o hufpakk huffman.c main.c -I.
clean:
	rm -rf *.o $(APP_NAME) $(TEST_FILE) $(TEST_APPS)