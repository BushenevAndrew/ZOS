#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>

int main() {
    const char *fifo_path = "/tmp/myfifo";
    mkfifo(fifo_path, 0666);

    int fd = open(fifo_path, O_WRONLY);
    if (fd == -1) {
        perror("open");
        exit(1);
    }

    time_t t = time(NULL);
    char buffer[256];
    snprintf(buffer, sizeof(buffer), "Writer time: %ld, PID: %d", t, getpid());
    
    write(fd, buffer, strlen(buffer) + 1);
    close(fd);

    return 0;
}