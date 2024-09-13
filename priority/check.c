#include <stdio.h>
#include <stdlib.h>
#include <sched.h>

int main() {
    #ifdef SCHED_IDLE
        printf("SCHED_IDLE поддерживается на этой системе.\n");
    #else
        printf("SCHED_IDLE НЕ поддерживается на этой системе.\n");
    #endif

    return 0;
}

