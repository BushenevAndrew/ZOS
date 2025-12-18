#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>

int main() {
    const char *fifo_path = "/tmp/myfifo";

    sleep(10); // Задержка 10 секунд

    int fd = open(fifo_path, O_RDONLY);
    if (fd == -1) {
        perror("open");
        exit(1);
    }

    char buffer[256];
    read(fd, buffer, sizeof(buffer));
    close(fd);

    time_t t = time(NULL);
    printf("Reader time: %ld, PID: %d\n", t, getpid());
    printf("Received: %s\n", buffer);

    unlink(fifo_path); // Удаляем FIFO

    return 0;
}