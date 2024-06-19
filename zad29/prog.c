#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    char name[] = "Twoje Imię i Nazwisko";  // Zastąp własnym imieniem i nazwiskiem
    char buffer[256];
    FILE *fp;

    // Odczytanie liczby procesów z /proc/stat
    fp = fopen("/proc/stat", "r");
    if (fp == NULL) {
        perror("Nie udało się otworzyć /proc/stat");
        return 1;
    }
    int processes = 0;
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        if (strncmp(buffer, "processes", 9) == 0) {
            sscanf(buffer, "processes %d", &processes);
            break;
        }
    }
    fclose(fp);

    // Wypisanie imienia i nazwiska oraz liczby procesów
    printf("Imię i nazwisko: %s\n", name);
    printf("Liczba procesów: %d\n", processes);

    return 0;
}
