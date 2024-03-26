#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <ctype.h>

#define MSG_SIZE 256

int main() {
    key_t key;
    int msgid;
    char message[MSG_SIZE];

    // Tworzenie unikalnego klucza
    key = ftok("/tmp/prog1", 65);
    // Dołączenie do kolejki komunikatów
    msgid = msgget(key, 0666 | IPC_CREAT);

    // Odbieranie komunikatu z kolejki
    msgrcv(msgid, message, MSG_SIZE, 0, 0);

    // Zamiana na duże litery
    for (int i = 0; i < strlen(message); i++) {
        message[i] = toupper(message[i]);
    }

    printf("Otrzymano: %s\n", message);

    // Usunięcie kolejki
    msgctl(msgid, IPC_RMID, NULL);

    return 0;
}
