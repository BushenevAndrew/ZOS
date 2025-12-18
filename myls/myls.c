#define _XOPEN_SOURCE 700  // Для корректной работы с функциями POSIX
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>

// ANSI коды цветов
#define RESET   "\x1B[0m"
#define BLUE    "\x1B[1;94m"
#define GREEN   "\x1B[1;92m"
#define CYAN    "\x1B[1;96m"

int show_all = 0; // -a
int long_list = 0; // -l

void print_permissions(mode_t mode) {
    printf((S_ISDIR(mode)) ? "d" : (S_ISLNK(mode)) ? "l" : "-");
    printf((mode & S_IRUSR) ? "r" : "-");
    printf((mode & S_IWUSR) ? "w" : "-");
    printf((mode & S_IXUSR) ? "x" : "-");
    printf((mode & S_IRGRP) ? "r" : "-");
    printf((mode & S_IWGRP) ? "w" : "-");
    printf((mode & S_IXGRP) ? "x" : "-");
    printf((mode & S_IROTH) ? "r" : "-");
    printf((mode & S_IWOTH) ? "w" : "-");
    printf((mode & S_IXOTH) ? "x" : "-");
}

void print_file_info(const char *path, const char *name) {
    struct stat st;
    char full_path[1024];
    
    // Формируем полный путь
    if (strcmp(path, ".") == 0) {
        snprintf(full_path, sizeof(full_path), "%s", name);
    } else {
        snprintf(full_path, sizeof(full_path), "%s/%s", path, name);
    }
    
    // Используем lstat для отслеживания символических ссылок
    if (lstat(full_path, &st) == -1) {
        perror("lstat");
        return;
    }

    if (long_list) {
        print_permissions(st.st_mode);
        printf(" %lu", (unsigned long)st.st_nlink);
        
        struct passwd *pw = getpwuid(st.st_uid);
        struct group *gr = getgrgid(st.st_gid);
        printf(" %s %s", pw ? pw->pw_name : "?", gr ? gr->gr_name : "?");
        
        printf(" %8ld", (long)st.st_size);
        
        char time_buf[64];
        struct tm *tm_info = localtime(&st.st_mtime);
        strftime(time_buf, sizeof(time_buf), "%b %d %H:%M", tm_info);
        printf(" %s", time_buf);
        
        printf(" ");
    }

    // Цветовое выделение
    if (S_ISDIR(st.st_mode)) {
        printf(BLUE "%s" RESET, name);
    } else if (S_ISLNK(st.st_mode)) {
        printf(CYAN "%s" RESET, name);
    } else if (st.st_mode & S_IXUSR) {
        printf(GREEN "%s" RESET, name);
    } else {
        printf("%s", name);
    }

    if (long_list && S_ISLNK(st.st_mode)) {
        char link_target[1024];
        ssize_t len = readlink(full_path, link_target, sizeof(link_target) - 1);
        if (len != -1) {
            link_target[len] = '\0';
            printf(BLUE " -> %s" RESET, link_target);
        }
    }

    printf("\n");
}

void list_directory(const char *path) {
    DIR *dir = opendir(path);
    if (!dir) {
        perror("opendir");
        return;
    }

    struct dirent *entry;
    char **names = NULL;
    int count = 0;

    // Сбор имён с учётом -a
    while ((entry = readdir(dir)) != NULL) {
        if (!show_all && entry->d_name[0] == '.') {
            continue;
        }
        names = realloc(names, (count + 1) * sizeof(char *));
        names[count] = strdup(entry->d_name);
        count++;
    }

    // Сортировка по алфавиту
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (strcmp(names[i], names[j]) > 0) {
                char *temp = names[i];
                names[i] = names[j];
                names[j] = temp;
            }
        }
    }

    // Вывод
    for (int i = 0; i < count; i++) {
        print_file_info(path, names[i]);
        free(names[i]);
    }

    free(names);
    closedir(dir);
}

int main(int argc, char *argv[]) {
    extern int optind;  // Объявление переменной из getopt
    int opt;
    
    while ((opt = getopt(argc, argv, "la")) != -1) {
        switch (opt) {
            case 'l':
                long_list = 1;
                break;
            case 'a':
                show_all = 1;
                break;
            default:
                fprintf(stderr, "Использование: %s [-l] [-a] [директория]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    const char *path = ".";
    if (optind < argc) {
        path = argv[optind];
    }

    list_directory(path);
    return 0;
}