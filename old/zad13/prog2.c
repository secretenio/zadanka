#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

#define SOCKET_PATH "/tmp/unix_socket"

int main() {
    int server_fd = socket(AF_UNIX, SOCK_STREAM, 0);

    struct sockaddr_un server_addr;
    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, SOCKET_PATH);

    bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));

    listen(server_fd, 5);

    printf("Serwer nasłuchuje...\n");

    int client_fd = accept(server_fd, NULL, NULL);

    char buffer[256];
    ssize_t bytes_received = recv(client_fd, buffer, sizeof(buffer), 0);

    for (int i = 0; i < bytes_received; ++i) {
        buffer[i] = toupper(buffer[i]);
    }

    send(client_fd, buffer, bytes_received, 0);
    printf("Dane zostały wysłane do klienta.\n");

    close(client_fd);
    close(server_fd);
    unlink(SOCKET_PATH);

    return 0;
}
