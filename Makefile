CC=gcc
CFLAGS=-I./include -Wall

SRCS=src/main.c src/file_traversal.c src/byte_search.c src/error_handling.c
OBJS=$(SRCS:.c=.o)
EXEC=lab11

.PHONY: all clean

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(EXEC)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(EXEC)
