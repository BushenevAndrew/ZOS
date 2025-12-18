
CC = gcc
CFLAGS = -Wall -Wextra -std=c11

all: sender receiver

# Правило для сборки sender
sender: sender.c
	$(CC) $(CFLAGS) -o sender sender.c

# Правило для сборки receiver
receiver: receiver.c
	$(CC) $(CFLAGS) -o receiver receiver.c

# Очистка временных файлов
clean:
	rm -f sender receiver

# Удаление сегмента разделяемой памяти (опционально)
clean_shm:
	ipcrm -M 1234 2>/dev/null || echo "Сегмент разделяемой памяти не найден."

.PHONY: all clean clean_shm
