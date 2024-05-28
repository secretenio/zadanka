#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdint.h>

#define GPIO2_START_ADDR 0x481ac000
#define GPIO_SIZE 0x00001000

#define GPIO_OE 0x134
#define GPIO_DATAIN 0x138
#define GPIO_SETDATAOUT 0x194
#define GPIO_CLEARDATAOUT 0x190

#define GPIO_66 (1<<2)  // GPIO 66 to bit 2 of GPIO2
#define GPIO_67 (1<<3)  // GPIO 67 to bit 3 of GPIO2
#define GPIO_68 (1<<4)  // GPIO 68 to bit 4 of GPIO2
#define GPIO_69 (1<<5)  // GPIO 69 to bit 5 of GPIO2

int main(int argc, char *argv[]) {
    int fd;
    volatile void *gpio_addr;
    volatile uint32_t *gpio_oe_addr;
    volatile uint32_t *gpio_datain_addr;
    volatile uint32_t *gpio_setdataout_addr;
    volatile uint32_t *gpio_cleardataout_addr;

    // Otwórz /dev/mem
    if ((fd = open("/dev/mem", O_RDWR)) < 0) {
        perror("Unable to open /dev/mem");
        exit(1);
    }

    // Mapuj GPIO
    gpio_addr = mmap(0, GPIO_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, GPIO2_START_ADDR);
    if (gpio_addr == MAP_FAILED) {
        perror("Unable to map GPIO");
        close(fd);
        exit(1);
    }

    gpio_oe_addr = gpio_addr + GPIO_OE;
    gpio_datain_addr = gpio_addr + GPIO_DATAIN;
    gpio_setdataout_addr = gpio_addr + GPIO_SETDATAOUT;
    gpio_cleardataout_addr = gpio_addr + GPIO_CLEARDATAOUT;

    // Skonfiguruj GPIO_66 i GPIO_67 jako wyjścia
    *gpio_oe_addr &= ~(GPIO_66 | GPIO_67);

    // Skonfiguruj GPIO_68 i GPIO_69 jako wejścia
    *gpio_oe_addr |= (GPIO_68 | GPIO_69);

    while (1) {
        // Odczytaj GPIO_68 i kontroluj GPIO_66
        if (*gpio_datain_addr & GPIO_68) {
            *gpio_setdataout_addr = GPIO_66;  // Zapal LED GPIO66
        } else {
            *gpio_cleardataout_addr = GPIO_66;  // Zgaś LED GPIO66
        }

        // Odczytaj GPIO_69 i kontroluj GPIO_67
        if (*gpio_datain_addr & GPIO_69) {
            *gpio_setdataout_addr = GPIO_67;  // Zapal LED GPIO67
        } else {
            *gpio_cleardataout_addr = GPIO_67;  // Zgaś LED GPIO67
        }

        usleep(100000);  // Czekaj 100ms
    }

    // Sprzątaj
    munmap((void *)gpio_addr, GPIO_SIZE);
    close(fd);

    return 0;
}

}
