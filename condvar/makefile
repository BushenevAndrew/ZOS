CC = gcc
CFLAGS = -Wall -Wextra -pthread

all: condvar_example

condvar_example: condvar_example.c
	$(CC) $(CFLAGS) -o condvar_example condvar_example.c

clean:
	rm -f condvar_example
