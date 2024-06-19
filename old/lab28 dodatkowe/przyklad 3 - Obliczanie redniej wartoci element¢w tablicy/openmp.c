#include <stdio.h>
#include <omp.h>
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

    // Obliczanie sumy elementów tablicy równolegle
    #pragma omp parallel for reduction(+:sum)
    for (i = 0; i < N; i++) {
        sum += array[i];
    }
    double average = sum / N;

    // Mierzenie czasu zakończenia
    clock_gettime(CLOCK_MONOTONIC, &end);

    // Obliczanie czasu wykonania
    double time_taken = (end.tv_sec - start.tv_sec) * 1e9;
    time_taken = (time_taken + (end.tv_nsec - start.tv_nsec)) * 1e-9;

    printf("Średnia wartość elementów tablicy (równolegle): %f\n", average);
    printf("Czas wykonania: %f sekund\n", time_taken);

    return 0;
}
