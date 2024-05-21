#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define GPIO_BUTTON_YELLOW "68"
#define GPIO_BUTTON_BLUE "69"
#define GPIO_PATH "/sys/class/gpio"
#define EXPORT_PATH GPIO_PATH "/export"
#define UNEXPORT_PATH GPIO_PATH "/unexport"
#define DIRECTION_PATH GPIO_PATH "/gpio%s/direction"
#define VALUE_PATH GPIO_PATH "/gpio%s/value"

void write_to_file(const char *file, const char *value) {
    int fd = open(file, O_WRONLY);
    if (fd < 0) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }
    if (write(fd, value, strlen(value)) != strlen(value)) {
        perror("Failed to write to file");
        close(fd);
        exit(EXIT_FAILURE);
    }
    close(fd);
}

char read_from_file(const char *file) {
    char value;
    int fd = open(file, O_RDONLY);
    if (fd < 0) {
        perror("Failed to open file for reading");
        exit(EXIT_FAILURE);
    }
    if (read(fd, &value, 1) != 1) {
        perror("Failed to read from file");
        close(fd);
        exit(EXIT_FAILURE);
    }
    close(fd);
    return value;
}

int main() {
    char path[128], valYellow, valBlue;

    // Eksportujemy piny
    write_to_file(EXPORT_PATH, GPIO_BUTTON_YELLOW);
    write_to_file(EXPORT_PATH, GPIO_BUTTON_BLUE);

    // Ustawiamy kierunki pinów na ‘in’
    snprintf(path, sizeof(path), DIRECTION_PATH, GPIO_BUTTON_YELLOW);
    write_to_file(path, "in");
    snprintf(path, sizeof(path), DIRECTION_PATH, GPIO_BUTTON_BLUE);
    write_to_file(path, "in");

    printf("Monitor przyciskow: \n");

    while (1) {
        snprintf(path, sizeof(path), VALUE_PATH, GPIO_BUTTON_YELLOW);
        valYellow = read_from_file(path);
        snprintf(path, sizeof(path), VALUE_PATH, GPIO_BUTTON_BLUE);
        valBlue = read_from_file(path);
        
        printf("rzułty papieski: %c, niebieski1: %c\n", valYellow, valBlue);

        usleep(500000); // Odczyt stanu przycisków co 500 ms
    }

    // Przewidywanie miejsca na kod unexport’u (w przypadku wyjścia z pętli)
    write_to_file(UNEXPORT_PATH, GPIO_BUTTON_YELLOW);
    write_to_file(UNEXPORT_PATH, GPIO_BUTTON_BLUE);

    return 0;
}
