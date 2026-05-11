CC = gcc
CFLAGS = -Wall -Wextra -g

fat32: main.o fat32_utils.o fat32_init.o
	$(CC) $(CFLAGS) main.o fat32_utils.o fat32_init.o -o fat32

main.o: main.c fat32_utils.h
	$(CC) $(CFLAGS) -c main.c

fat32_utils.o: fat32_utils.c fat32_utils.h fat32_init.h
	$(CC) $(CFLAGS) -c fat32_utils.c

fat32_init.o: fat32_init.c fat32_init.h fat32_utils.h
	$(CC) $(CFLAGS) -c fat32_init.c

clean:
	rm -f *.o fat32
