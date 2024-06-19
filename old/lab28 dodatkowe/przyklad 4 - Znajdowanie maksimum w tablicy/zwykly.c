#include <stdio.h>
#include <time.h>

#define N 1000000

int main() {
    int i;
    double max = -1.0;
    double array[N];
    struct timespec start, end;

    // Inicjalizacja tablicy
    for (i = 0; i < N; i++) {
        array[i] = (double)i;
    }

    // Mierzenie czasu rozpoczęcia
    clock_gettime(CLOCK_MONOTONIC, &start);

    // Znajdowanie maksimum w tablicy szeregowo
    for (i = 0; i < N; i++) {
        if (array[i] > max) {
            max = array[i];
        }
    }

    // Mierzenie czasu zakończenia
    clock_gettime(CLOCK_MONOTONIC, &end);

    // Obliczanie czasu wykonania
    double time_taken = (end.tv_sec - start.tv_sec) * 1e9;
    time_taken = (time_taken + (end.tv_nsec - start.tv_nsec)) * 1e-9;

    printf("Maksimum w tablicy (szeregowo): %f\n", max);
    printf("Czas wykonania: %f sekund\n", time_taken);

    return 0;
}
