#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void search_pattern(FILE *file, const char *pattern) {
    char buffer[1024];

    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        if (strstr(buffer, pattern) != NULL) {
            printf("%s", buffer);
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Использование: %s <шаблон> [файл]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *pattern = argv[1];
    FILE *file = stdin;

    if (argc > 2) {
        file = fopen(argv[2], "r");
        if (file == NULL) {
            perror("Ошибка открытия файла");
            exit(EXIT_FAILURE);
        }
    }

    search_pattern(file, pattern);

    if (file != stdin) {
        fclose(file);
    }

    return EXIT_SUCCESS;
}
