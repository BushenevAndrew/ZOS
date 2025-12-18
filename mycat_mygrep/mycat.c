#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>

void print_file(FILE *file, int number_lines, int number_nonblank_lines, int show_ends) {
    char buffer[1024];
    int line_number = 1;

    while (fgets(buffer, sizeof(buffer), file)) {
        int is_empty = (buffer[0] == '\n');

        if (number_nonblank_lines && !is_empty) {
            printf("%6d  ", line_number++);
        } else if (number_lines) {
            printf("%6d  ", line_number++);
        }

        if (show_ends) {
            size_t len = strlen(buffer);
            if (len > 0 && buffer[len - 1] == '\n') {
                buffer[len - 1] = '$';
                printf("%s\n", buffer);
            } else {
                printf("%s", buffer);
            }
        } else {
            printf("%s", buffer);
        }
    }
}

int main(int argc, char *argv[]) {
    int number_lines = 0;
    int number_nonblank_lines = 0;
    int show_ends = 0;
    int opt;

    while ((opt = getopt(argc, argv, "nbeE")) != -1) {
        switch (opt) {
            case 'n':
                number_lines = 1;
                break;
            case 'b':
                number_nonblank_lines = 1;
                break;
            case 'e':
            case 'E':
                show_ends = 1;
                break;
            default:
                fprintf(stderr, "Usage: %s [-n] [-b] [-E] [file ...]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    if (optind < argc) {
        for (int i = optind; i < argc; i++) {
            FILE *file = fopen(argv[i], "r");
            if (!file) {
                perror(argv[i]);
                exit(EXIT_FAILURE);
            }
            print_file(file, number_lines, number_nonblank_lines, show_ends);
            fclose(file);
        }
    } else {
        print_file(stdin, number_lines, number_nonblank_lines, show_ends);
    }

    return EXIT_SUCCESS;
}
