CC = gcc
CFLAGS = -Wall -Wextra -std=c11
FIFO_PATH = /tmp/myfifo

all: pipe_example writer reader

pipe_example: pipeFork.c
	$(CC) $(CFLAGS) -o $@ $<

writer: writer.c
	$(CC) $(CFLAGS) -o $@ $<

reader: reader.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f writer pipe_example reader $(FIFO_PATH)

run_pipe: pipe_example
	./pipe_example

run_fifo: writer reader
	@echo "Запуск writer в фоне..."
	./writer &
	@sleep 2
	@echo "Запуск reader..."
	./reader

test: all
	@echo "=== Тест pipe ==="
	./pipe_example
	@echo -e "\n=== Тест FIFO ==="
	./writer &
	@sleep 2
	./reader

.PHONY: all clean run_pipe run_fifo test