#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define MSG_SIZE 256

int main() {
    key_t key;
    int msgid;
    char message[MSG_SIZE];

    // Tworzenie unikalnego klucza
    key = ftok("/tmp/prog1", 65);
    // Tworzenie kolejki komunikatów
    msgid = msgget(key, 0666 | IPC_CREAT);

    printf("Wpisz imię i nazwisko: ");
    fgets(message, MSG_SIZE, stdin);
    // Usunięcie znaku nowej linii na końcu wprowadzonego tekstu
    message[strcspn(message, "\n")] = '\0';

    // Wysłanie komunikatu do kolejki
    msgsnd(msgid, message, strlen(message)+1, 0);

    printf("Wiadomość wysłana: %s\n", message);

    return 0;
}
