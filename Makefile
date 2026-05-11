CC = gcc
CFLAGS = -Wall -Wextra -g

fat32: main.o fat32_utils.o
	$(CC) $(CFLAGS) main.o fat32_utils.o -o fat32

main.o: main.c fat32_utils.h
	$(CC) $(CFLAGS) -c main.c

fat32_utils.o: fat32_utils.c fat32_utils.h
	$(CC) $(CFLAGS) -c fat32_utils.c

clean:
	rm -f *.o fat32
