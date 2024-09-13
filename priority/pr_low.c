#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <errno.h>
#include <unistd.h>  // Для sleep()

int main() {
    // NEED REFACTOR, SCHED_IDLE tmp..
    //
    struct sched_param param;

    // Убедитесь, что SCHED_IDLE определён на вашей системе
    #ifdef SCHED_IDLE
        // Для SCHED_IDLE приоритет всегда 0
        param.sched_priority = 0;

        // Устанавливаем планировщик SCHED_IDLE для текущего процесса
        if (sched_setscheduler(0, SCHED_IDLE, &param) == -1) {
            perror("Ошибка установки планировщика SCHED_IDLE");
            exit(EXIT_FAILURE);
        }

        printf("Успешно установлен SCHED_IDLE с приоритетом %d\n", param.sched_priority);

        // Основной код программы
        while (1) {
            printf("Очень низкий приоритет\n");
            sleep(1);
        }
    #else
        printf("SCHED_IDLE не поддерживается на этой системе.\n");
        exit(EXIT_FAILURE);
    #endif

    return 0;
}
