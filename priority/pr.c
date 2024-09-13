#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <errno.h>

int main() {
    struct sched_param param;
    int max_priority;

    // Получаем максимальный приоритет для SCHED_FIFO
    max_priority = sched_get_priority_max(SCHED_FIFO);
    if (max_priority == -1) {
        perror("Ошибка получения максимального приоритета");
        exit(EXIT_FAILURE);
    }

    // Устанавливаем максимальный приоритет для SCHED_FIFO
    param.sched_priority = max_priority;

    // Устанавливаем планировщик SCHED_FIFO для текущего процесса
    if (sched_setscheduler(0, SCHED_FIFO, &param) == -1) {
        perror("Ошибка установки планировщика SCHED_FIFO");
        exit(EXIT_FAILURE);
    }

    printf("Успешно установлен SCHED_FIFO с приоритетом %d\n", param.sched_priority);

    // Основной код программы
    while (1) {
        // Ваш код, который требует выполнения в реальном времени
        // Например, сервер или важная обработка данных
		printf("Very good priotiy");
	}

    return 0;
}

