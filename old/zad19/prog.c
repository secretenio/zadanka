#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define LED_RED "66"
#define LED_GREEN "67"
#define GPIO_PATH "/sys/class/gpio"
#define EXPORT_PATH GPIO_PATH "/export"
#define UNEXPORT_PATH GPIO_PATH "/unexport"
#define DIRECTION_PATH GPIO_PATH "/gpio%s/direction"
#define VALUE_PATH GPIO_PATH "/gpio%s/value"

#define TIME_TO_BLINK 10 // Czas miganie w sekundach
#define BLINK_INTERVAL 500000 // Interwał miganie w mikrosekundach (500ms)

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

int main() {
    char path[128];

    // Eksportujemy piny
    write_to_file(EXPORT_PATH, LED_RED);
    write_to_file(EXPORT_PATH, LED_GREEN);
    
    // Ustawiamy kierunek pinów na ‘out’
    snprintf(path, sizeof(path), DIRECTION_PATH, LED_RED);
    write_to_file(path, "out");
    snprintf(path, sizeof(path), DIRECTION_PATH, LED_GREEN);
    write_to_file(path, "out");

    // Migamy LED-ami
    for (int i = 0; i < (TIME_TO_BLINK * 1000000 / BLINK_INTERVAL); i++) {
        // LED czerwona ON, LED zielona OFF
        snprintf(path, sizeof(path), VALUE_PATH, LED_RED);
        write_to_file(path, "1");
        snprintf(path, sizeof(path), VALUE_PATH, LED_GREEN);
        write_to_file(path, "0");

        usleep(BLINK_INTERVAL);
        
        // LED czerwona OFF, LED zielona ON
        snprintf(path, sizeof(path), VALUE_PATH, LED_RED);
        write_to_file(path, "0");
        snprintf(path, sizeof(path), VALUE_PATH, LED_GREEN);
        write_to_file(path, "1");

        usleep(BLINK_INTERVAL);
    }

    // Odrejestrowujemy piny
    write_to_file(UNEXPORT_PATH, LED_RED);
    write_to_file(UNEXPORT_PATH, LED_GREEN);

    return 0;
}
