#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

void print_usage() {
    fprintf(stderr, "Использование: ./mychmod [режим] файл\n");
    fprintf(stderr, "Примеры:\n");
    fprintf(stderr, "  ./mychmod +x file.txt\n");
    fprintf(stderr, "  ./mychmod u-r file.txt\n");
    fprintf(stderr, "  ./mychmod g+rw file.txt\n");
    fprintf(stderr, "  ./mychmod ug+rw file.txt\n");
    fprintf(stderr, "  ./mychmod uga+rwx file.txt\n");
    fprintf(stderr, "  ./mychmod 766 file.txt\n");
    exit(EXIT_FAILURE);
}

void change_mode(const char *filename, mode_t mode) {
    if (chmod(filename, mode) == -1) {
        perror("chmod");
        exit(EXIT_FAILURE);
    }
}

mode_t parse_numeric_mode(const char *mode_str) {
    unsigned int mode = strtoul(mode_str, NULL, 8);
    return (mode_t)mode;
}

mode_t parse_symbolic_mode(const char *mode_str, mode_t current_mode) {
    mode_t new_mode = current_mode;
    const char *p = mode_str;
    
    // Проверяем простой случай без указания категории (например, +x)
    if (mode_str[0] == '+' || mode_str[0] == '-' || mode_str[0] == '=') {
        // Это форма без указания категории, значит применяется ко всем
        char op = mode_str[0];
        p = mode_str + 1;
        
        mode_t perm_mask = 0;
        while (*p) {
            if (*p == 'r') perm_mask |= (S_IRUSR | S_IRGRP | S_IROTH);
            else if (*p == 'w') perm_mask |= (S_IWUSR | S_IWGRP | S_IWOTH);
            else if (*p == 'x') perm_mask |= (S_IXUSR | S_IXGRP | S_IXOTH);
            p++;
        }
        
        switch (op) {
            case '+':
                new_mode |= perm_mask;
                break;
            case '-':
                new_mode &= ~perm_mask;
                break;
            case '=':
                // Для = нужно сбросить все, а потом установить указанные
                // Сначала сбрасываем все стандартные права
                new_mode &= ~(S_IRWXU | S_IRWXG | S_IRWXO);
                // Устанавливаем указанные
                new_mode |= perm_mask;
                break;
        }
        
        return new_mode;
    }
    
    // Обработка формы с указанием категории (u, g, o, a)
    while (*p) {
        // Определяем, какие категории пользователей затронуты
        mode_t who_mask = 0;
        while (*p) {
            if (*p == 'u') who_mask |= (S_IRUSR | S_IWUSR | S_IXUSR);
            else if (*p == 'g') who_mask |= (S_IRGRP | S_IWGRP | S_IXGRP);
            else if (*p == 'o') who_mask |= (S_IROTH | S_IWOTH | S_IXOTH);
            else if (*p == 'a') who_mask |= (S_IRWXU | S_IRWXG | S_IRWXO);
            else break;
            p++;
        }
        
        // Если не указаны категории, по умолчанию 'a' (all)
        if (who_mask == 0) {
            who_mask = (S_IRWXU | S_IRWXG | S_IRWXO);
        }
        
        // Операция: +, - или =
        char op = *p;
        if (op != '+' && op != '-' && op != '=') {
            fprintf(stderr, "Неверная операция: %c\n", op);
            exit(EXIT_FAILURE);
        }
        p++;
        
        // Определяем, какие права меняем
        mode_t perm_mask = 0;
        while (*p && *p != ',') {
            if (*p == 'r') perm_mask |= (S_IRUSR | S_IRGRP | S_IROTH);
            else if (*p == 'w') perm_mask |= (S_IWUSR | S_IWGRP | S_IWOTH);
            else if (*p == 'x') perm_mask |= (S_IXUSR | S_IXGRP | S_IXOTH);
            p++;
        }
        
        // Применяем операцию только к указанным категориям
        mode_t effective_perm = perm_mask & who_mask;
        
        switch (op) {
            case '+':
                new_mode |= effective_perm;
                break;
            case '-':
                new_mode &= ~effective_perm;
                break;
            case '=':
                // Сначала сбрасываем все права для указанных категорий
                new_mode &= ~who_mask;
                // Затем устанавливаем указанные права
                new_mode |= effective_perm;
                break;
        }
        
        // Пропускаем запятую для следующей операции
        if (*p == ',') p++;
    }
    
    return new_mode;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        print_usage();
    }

    const char *mode_str = argv[1];
    const char *filename = argv[2];

    struct stat file_stat;
    if (stat(filename, &file_stat) == -1) {
        perror("stat");
        exit(EXIT_FAILURE);
    }

    mode_t new_mode;
    if (mode_str[0] >= '0' && mode_str[0] <= '7') {
        // Числовой режим
        new_mode = parse_numeric_mode(mode_str);
    } else {
        // Символьный режим
        new_mode = parse_symbolic_mode(mode_str, file_stat.st_mode);
    }

    change_mode(filename, new_mode);

    printf("Права доступа для файла '%s' успешно изменены.\n", filename);
    return EXIT_SUCCESS;
}