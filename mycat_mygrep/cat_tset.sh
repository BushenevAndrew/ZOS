#!/bin/bash

# Тест 1: Простой вывод файла
echo "Тест 1: Простой вывод файла"
echo "Hello, World!" > test1.txt
./mycat test1.txt
echo

# Тест 2: Флаг -n (нумерация всех строк)
echo "Тест 2: Флаг -n"
echo -e "First\nSecond\nThird" > test2.txt
./mycat -n test2.txt
echo

# Тест 3: Флаг -b (нумерация только непустых строк)
echo "Тест 3: Флаг -b"
echo -e "First\n\nSecond\n\nThird" > test3.txt
./mycat -b test3.txt
echo

# Тест 4: Флаг -E (показать символ $ в конце строк)
echo "Тест 4: Флаг -E"
echo -e "Line 1\nLine 2" > test4.txt
./mycat -E test4.txt
echo

# Тест 5: Комбинированные флаги -nE
echo "Тест 5: Комбинированные флаги -nE"
./mycat -nE test2.txt
echo

# Тест 6: Комбинированные флаги -b -E
echo "Тест 6: Комбинированные флаги -b -E"
./mycat -b -E test3.txt
echo

# Удаление временных файлов
rm -f test1.txt test2.txt test3.txt test4.txt