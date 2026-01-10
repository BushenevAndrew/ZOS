CC = gcc
CFLAGS = -Wall -Wextra -pthread

all: rwlock_example

rwlock_example: rwlock_example.c
	$(CC) $(CFLAGS) -o rwlock_example rwlock_example.c

clean:
	rm -f rwlock_example
