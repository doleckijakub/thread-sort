#include "thread-sort.h"

#include <stdlib.h>
// #include <stdbool.h>
#include <pthread.h>

static int *in_base;

// static size_t count_sorted;
// static pthread_mutex_t count_sorted_mutex;
static pthread_t *comp_threads;
static pthread_mutex_t *el_mutexes;

// static void update_sorted_count(size_t delta) {
//     pthread_mutex_lock(&count_sorted);
//     count_sorted += delta;
//     pthread_mutex_unlock(&count_sorted);
// }

static void swap(size_t a, size_t b) {
    int temp = in_base[a];
    in_base[a] = in_base[b];
    in_base[b] = temp;
}

static void *comp_op(void *arg) {
    size_t i = (size_t) arg;

    // bool updated_ok = false;

    while (1 /* not sorted */) {
        if (in_base[i] > in_base[i + 1]) {
            // if (!updated_ok) { update_sorted_count(1); updated_ok = true }
            pthread_mutex_lock(&el_mutexes[i]);
            pthread_mutex_lock(&el_mutexes[i + 1]);
            swap(i, i + 1);
            pthread_mutex_unlock(&el_mutexes[i]);
            pthread_mutex_unlock(&el_mutexes[i + 1]);
        } // else if (!updated_ok) { update_sorted_count(-1); updated_ok = true }
    }
}

int thread_sort(int *base, size_t nmemb) {
    // count_sorted = 0;

    // if (pthread_mutex_init(&count_sorted_mutex, NULL) != 0) {
    //     return 1;
    // }

    in_base = base; // TODO: pass to thread

    comp_threads = (pthread_t *) calloc(nmemb - 1, sizeof(pthread_t));
    for (size_t i = 0; i < nmemb - 1; i++) {
        if(pthread_create(&comp_threads[i], NULL, &comp_op, (void *) i) != 0) {
            return 2;
        }
    }
    
    el_mutexes = (pthread_mutex_t *) calloc(nmemb, sizeof(pthread_mutex_t));
    for (size_t i = 0; i < nmemb; i++) {
        if (pthread_mutex_init(&el_mutexes[i], NULL) != 0) {
            return 1;
        }
    }

    for (size_t i = 0; i < nmemb - 1; i++) {
        if(pthread_join(comp_threads[i], NULL) != 0) {
            return 3;
        }
    }

    return 0;
}