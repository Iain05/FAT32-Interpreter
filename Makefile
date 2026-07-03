CC = gcc
CFLAGS = -Wall -Wextra -g -Iutil

SRCS = main.c fat32_utils.c fat32_init.c util/debug.c util/directory_cache.c
OBJS = $(SRCS:.c=.o)
TARGET = fat32

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: clean
