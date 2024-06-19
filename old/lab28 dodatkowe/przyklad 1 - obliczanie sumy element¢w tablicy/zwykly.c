#include <stdio.h>
#include <time.h>

#define N 1000000

int main() {
    int i;
    double sum = 0.0;
    double array[N];
    struct timespec start, end;

    // Inicjalizacja tablicy
    for (i = 0; i < N; i++) {
        array[i] = (double)i;
    }

    // Mierzenie czasu rozpoczęcia
    clock_gettime(CLOCK_MONOTONIC, &start);

    // Obliczanie sumy elementów tablicy szeregowo
    for (i = 0; i < N; i++) {
        sum += array[i];
    }

    // Mierzenie czasu zakończenia
    clock_gettime(CLOCK_MONOTONIC, &end);

    // Obliczanie czasu wykonania
    double time_taken = (end.tv_sec - start.tv_sec) * 1e9;
    time_taken = (time_taken + (end.tv_nsec - start.tv_nsec)) * 1e-9;

    printf("Suma elementów tablicy (szeregowo): %f\n", sum);
    printf("Czas wykonania: %f sekund\n", time_taken);

    return 0;
}
