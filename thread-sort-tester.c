#include "thread-sort.h"

#include <stdio.h>
#include <pthread.h>

int els[] = { 9, 1, 3, 20, 13, 15, 30, 10 };
size_t nmemb = sizeof(els) / sizeof(*els);

void *log_els(void *) {
    while (1) {
        for (int i = 0; i < nmemb; i++) {
            printf("%d ", els[i]);
        }
        printf("\n");
    }
}

int main() {
    pthread_t log_thread;
    if (pthread_create(&log_thread, NULL, &log_els, NULL) != 0) {
        return 1;
    }

    thread_sort(els, nmemb);
}