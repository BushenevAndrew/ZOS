#!/bin/bash

PROGRAM="./myfork"

# Проверка наличия программы
if [ ! -f "$PROGRAM" ]; then
    echo "Ошибка: программа $PROGRAM не найдена"
    echo "Скомпилируйте: gcc -o myfork myfork.c"
    exit 1
fi

echo "=== Простой тест программы myfork ==="
echo "Запуск программы:"
echo "------------------"

# Запускаем программу
"$PROGRAM"

echo "------------------"
echo "Тест завершён."
echo ""
echo "Если программа не запускается, проверьте:"
echo "1. Файл myfork.c существует"
echo "2. Компиляция без ошибок: gcc -o myfork myfork.c"
echo "3. Программа имеет права на выполнение: chmod +x myfork"