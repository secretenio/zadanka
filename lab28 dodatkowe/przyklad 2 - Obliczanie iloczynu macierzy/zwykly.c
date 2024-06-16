#include <stdio.h>
#include <time.h>

#define N 500

void matrix_multiply(double a[N][N], double b[N][N], double c[N][N]) {
    int i, j, k;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            c[i][j] = 0.0;
            for (k = 0; k < N; k++) {
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    }
}

int main() {
    double a[N][N], b[N][N], c[N][N];
    int i, j;
    struct timespec start, end;

    // Inicjalizacja macierzy
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            a[i][j] = (double)(i + j);
            b[i][j] = (double)(i - j);
        }
    }

    // Mierzenie czasu rozpoczęcia
    clock_gettime(CLOCK_MONOTONIC, &start);

    // Obliczanie iloczynu macierzy szeregowo
    matrix_multiply(a, b, c);

    // Mierzenie czasu zakończenia
    clock_gettime(CLOCK_MONOTONIC, &end);

    // Obliczanie czasu wykonania
    double time_taken = (end.tv_sec - start.tv_sec) * 1e9;
    time_taken = (time_taken + (end.tv_nsec - start.tv_nsec)) * 1e-9;

    printf("Iloczyn macierzy (szeregowo) zakończony.\n");
    printf("Czas wykonania: %f sekund\n", time_taken);

    return 0;
}
