
APP_NAME = hufpakk
TEST_FILE = proov.hup

CC = gcc
CFLAGS = -std=c99 -g


main: huffman.o main.c
	$(CC) $(CFLAGS) main.c huffman.o -o $(APP_NAME)

test: dictionary.o dictionary_test.c
	$(CC) $(CFLAGS) dictionary_test.c dictionary.o -o dictionary_test

run_test:
	make test
	./dictionary_test

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

#gcc -o hufpakk huffman.c main.c -I.
clean:
	rm -rf *.o $(APP_NAME) $(TEST_FILE) dictionary_test