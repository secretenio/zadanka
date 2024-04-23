#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <string.h>

#define SOCKET_PATH "/tmp/unix_socket"

int main() {
    int client_fd = socket(AF_UNIX, SOCK_STREAM, 0);

    struct sockaddr_un server_addr;
    
    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, SOCKET_PATH);

    connect(client_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));

    printf("Podaj tekst: ");
    char input[256];
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0;

    send(client_fd, input, strlen(input), 0);

    char buffer[256];
    ssize_t bytes_received = recv(client_fd, buffer, sizeof(buffer), 0);
    buffer[bytes_received] = '\0';
    printf("Otrzymane dane od serwera: %s\n", buffer);

    close(client_fd);

    return 0;
}
