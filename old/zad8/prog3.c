#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define MSG_SIZE 256

struct msg_buffer {
    long msg_type;
    char msg_text[MSG_SIZE];
};

int main() {
    key_t key;
    int msgid;
    struct msg_buffer message1, message2;

    // Tworzenie unikalnego klucza
    key = ftok("prog1", 65);
    // Dołączenie do kolejki komunikatów
    msgid = msgget(key, 0666 | IPC_CREAT);

    // Odbieranie pierwszego komunikatu
    msgrcv(msgid, &message1, sizeof(message1), 1, 0);

    // Odbieranie drugiego komunikatu
    msgrcv(msgid, &message2, sizeof(message2), 2, 0);
    
    printf("Numer indeksu: %s, Imie i nazwisko: %s", message1.msg_text, message2.msg_text);

    // Usunięcie kolejki
    msgctl(msgid, IPC_RMID, NULL);

    return 0;
}
