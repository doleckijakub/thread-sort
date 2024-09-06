#include "thread-sort.h"

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void log_els(int *els, size_t nmemb) {
    for (size_t i = 0; i < nmemb; i++) {
        printf("%d ", els[i]);
    }
    printf("\n");
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <integers...>\n", argv[0]);
        return 1;
    }

    size_t nmemb = argc - 1;
    int *els = (int *) malloc(nmemb * sizeof(int));

    if (!els) {
        fprintf(stderr, "Failed to allocate memory.\n");
        return 1;
    }

    for (size_t i = 0; i < nmemb; i++) {
        els[i] = atoi(argv[i + 1]);
    }

    printf("Before sorting: ");
    log_els(els, nmemb);

    thread_sort(els, nmemb);

    printf("After sorting: ");
    log_els(els, nmemb);

    free(els);
    return 0;
}
