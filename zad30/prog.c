#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>

#define DEVICE_NAME "/dev/simple_driver"

#define IOCTL_SET_FIRSTNAME _IOW(0, 0, char *)
#define IOCTL_SET_LASTNAME _IOW(0, 1, char *)
#define IOCTL_SET_INDEXNUMBER _IOW(0, 2, int)

#define IOCTL_GET_FIRSTNAME _IOR(0, 3, char *)
#define IOCTL_GET_LASTNAME _IOR(0, 4, char *)
#define IOCTL_GET_INDEXNUMBER _IOR(0, 5, int)

int main() {
    int fd;
    char firstname[100] = "Jan";
    char lastname[100] = "Kowalski";
    int index_number = 123456;

    char read_firstname[100];
    char read_lastname[100];
    int read_index_number;

    fd = open(DEVICE_NAME, O_RDWR);

    // Set values
    ioctl(fd, IOCTL_SET_FIRSTNAME, firstname);
    ioctl(fd, IOCTL_SET_LASTNAME, lastname);
    ioctl(fd, IOCTL_SET_INDEXNUMBER, &index_number);

    // Get values
    ioctl(fd, IOCTL_GET_FIRSTNAME, read_firstname);
    ioctl(fd, IOCTL_GET_LASTNAME, read_lastname);
    ioctl(fd, IOCTL_GET_INDEXNUMBER, &read_index_number);

    printf("Odczytane dane:\n");
    printf("Imię: %s\n", read_firstname);
    printf("Nazwisko: %s\n", read_lastname);
    printf("Numer indeksu: %d\n", read_index_number);

    close(fd);
    return 0;
}
