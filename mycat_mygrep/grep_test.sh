#!/bin/bash

# Тест 1: Поиск паттерна в файле
echo "Тест 1: Поиск паттерна в файле"
echo -e "apple\nbanana\ncherry\napricot" > test_grep1.txt
./mygrep "ap" test_grep1.txt
echo

# Тест 2: Туннелирование через mycat
echo "Тест 2: Туннелирование через mycat"
echo -e "dog\ncat\nrabbit\ncatfish" > test_grep2.txt
./mycat test_grep2.txt | ./mygrep "cat"
echo

# Тест 3: Туннелирование через ls
echo "Тест 3: Туннелирование через ls"
ls -l | ./mygrep "test_grep"
echo

# Тест 4: Поиск с пустым выводом
echo "Тест 4: Поиск с пустым выводом"
./mygrep "xyz" test_grep1.txt
echo

# Тест 5: Поиск с регистром (если поддерживается)
echo "Тест 5: Поиск с регистром"
echo -e "Apple\nbanana\nAPRICOT" > test_grep3.txt
./mygrep "apple" test_grep3.txt
echo

# Удаление временных файлов
rm -f test_grep1.txt test_grep2.txt test_grep3.txt