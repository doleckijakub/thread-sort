#include "thread-sort.h"

#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>

static int *in_base;
static size_t in_size;
static pthread_t *comp_threads;
static pthread_mutex_t *el_mutexes;
static bool sorted = false;

static void swap(size_t a, size_t b) {
    int temp = in_base[a];
    in_base[a] = in_base[b];
    in_base[b] = temp;
}

static void *comp_op(void *arg) {
    size_t i = (size_t) arg;

    while (!sorted) {
        pthread_mutex_lock(&el_mutexes[i]);
        pthread_mutex_lock(&el_mutexes[i + 1]);

        if (in_base[i] > in_base[i + 1]) {
            swap(i, i + 1);
        }

        pthread_mutex_unlock(&el_mutexes[i]);
        pthread_mutex_unlock(&el_mutexes[i + 1]);

        sched_yield();
    }

    return NULL;
}

int thread_sort(int *base, size_t nmemb) {
    in_base = base;
    in_size = nmemb;
    sorted = false;

    el_mutexes = (pthread_mutex_t *) calloc(nmemb, sizeof(pthread_mutex_t));
    if (!el_mutexes) {
        return 1;
    }
    for (size_t i = 0; i < nmemb; i++) {
        if (pthread_mutex_init(&el_mutexes[i], NULL) != 0) {
            return 1;
        }
    }

    comp_threads = (pthread_t *) calloc(nmemb - 1, sizeof(pthread_t));
    if (!comp_threads) {
        return 2;
    }
    for (size_t i = 0; i < nmemb - 1; i++) {
        if (pthread_create(&comp_threads[i], NULL, &comp_op, (void *) i) != 0) {
            return 2;
        }
    }

    while (1) {
        bool is_sorted = true;
        for (size_t i = 0; i < nmemb - 1; i++) {
            pthread_mutex_lock(&el_mutexes[i]);
            pthread_mutex_lock(&el_mutexes[i + 1]);

            if (in_base[i] > in_base[i + 1]) {
                is_sorted = false;
            }

            pthread_mutex_unlock(&el_mutexes[i]);
            pthread_mutex_unlock(&el_mutexes[i + 1]);
        }
        if (is_sorted) {
            sorted = true;
            break;
        }
    }

    for (size_t i = 0; i < nmemb - 1; i++) {
        pthread_join(comp_threads[i], NULL);
    }

    for (size_t i = 0; i < nmemb; i++) {
        pthread_mutex_destroy(&el_mutexes[i]);
    }
    free(el_mutexes);
    free(comp_threads);

    return 0;
}
