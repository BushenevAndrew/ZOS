CC = gcc
CFLAGS = -Wall -Wextra -pthread

all: semaphore_posix

semaphore_posix: semaphore_posix.c
	$(CC) $(CFLAGS) -o semaphore_posix semaphore_posix.c

clean:
	rm -f semaphore_posix
