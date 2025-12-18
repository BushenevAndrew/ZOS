#!/bin/bash

# Название тестируемой утилиты (полный или относительный путь)
MYCHMOD="$(pwd)/mychmod"

# Проверяем, существует ли бинарник mychmod
if [[ ! -f "$MYCHMOD" ]]; then
    echo "Ошибка: $MYCHMOD не найден. Соберите утилиту перед тестированием."
    exit 1
fi

# Проверяем, есть ли права на выполнение
if [[ ! -x "$MYCHMOD" ]]; then
    echo "Утилита $MYCHMOD не исполняемая. Добавляем права..."
    chmod +x "$MYCHMOD"
fi

# Функция для сравнения прав доступа
compare_perms() {
    local file=$1
    local expected=$2
    local actual=$(stat -c "%a" "$file")
    if [[ "$actual" == "$expected" ]]; then
        echo "✅ OK: $file — $actual"
    else
        echo "❌ FAIL: $file — ожидалось $expected, получено $actual"
    fi
}

# Создаём временный каталог для тестов
TEST_DIR=$(mktemp -d)
echo "Тестируем в директории: $TEST_DIR"

echo "=== Тест 1: +x (добавление права на выполнение для всех) ==="
touch "$TEST_DIR/file1.txt"
chmod 644 "$TEST_DIR/file1.txt"
"$MYCHMOD" +x "$TEST_DIR/file1.txt"
compare_perms "$TEST_DIR/file1.txt" 755

echo "=== Тест 2: u-r (удаление чтения у владельца) ==="
touch "$TEST_DIR/file2.txt"
chmod 755 "$TEST_DIR/file2.txt"
"$MYCHMOD" u-r "$TEST_DIR/file2.txt"
compare_perms "$TEST_DIR/file2.txt" 355

echo "=== Тест 3: g+rw (добавление чтения и записи для группы) ==="
touch "$TEST_DIR/file3.txt"
chmod 500 "$TEST_DIR/file3.txt"
"$MYCHMOD" g+rw "$TEST_DIR/file3.txt"
compare_perms "$TEST_DIR/file3.txt" 560

echo "=== Тест 4: ug+rw (чтение и запись для владельца и группы) ==="
touch "$TEST_DIR/file4.txt"
chmod 000 "$TEST_DIR/file4.txt"
"$MYCHMOD" ug+rw "$TEST_DIR/file4.txt"
compare_perms "$TEST_DIR/file4.txt" 660

echo "=== Тест 5: uga+rwx (все права для всех) ==="
touch "$TEST_DIR/file5.txt"
chmod 000 "$TEST_DIR/file5.txt"
"$MYCHMOD" uga+rwx "$TEST_DIR/file5.txt"
compare_perms "$TEST_DIR/file5.txt" 777

echo "=== Тест 6: числовой режим 766 ==="
touch "$TEST_DIR/file6.txt"
chmod 000 "$TEST_DIR/file6.txt"
"$MYCHMOD" 766 "$TEST_DIR/file6.txt"
compare_perms "$TEST_DIR/file6.txt" 766

echo "=== Тест 7: комбинированный (g-w,o+r) ==="
touch "$TEST_DIR/file7.txt"
chmod 777 "$TEST_DIR/file7.txt"
"$MYCHMOD" g-w,o+r "$TEST_DIR/file7.txt"
compare_perms "$TEST_DIR/file7.txt" 757

echo "=== Очистка тестовой директории ==="
rm -rf "$TEST_DIR"
echo "Тестирование завершено."