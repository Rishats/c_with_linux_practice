#!/bin/bash

echo "Starting script for process which writes to file"

# Открываем файл один раз для записи и привязываем его к дескриптору
exec 3>>logger.log

while true; do
    sleep 1
    TIME=$(date +%T)
    # Записываем время в файл, используя открытый файловый дескриптор 3
    echo $TIME >&3
done

# Не забываем закрыть дескриптор после завершения программы (в реальности не потребуется, так как бесконечный цикл)
exec 3>&-

