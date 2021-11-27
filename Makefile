CC = gcc
CFLAGS = -std=c99 -g
APP_NAME = hufpakk


main: huffman.o main.c
	$(CC) $(CFLAGS) main.c huffman.o -o $(APP_NAME)


%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

#gcc -o hufpakk huffman.c main.c -I.
clean:
	rm -rf *.o $(APP_NAME)