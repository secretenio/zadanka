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

    key = ftok("/tmp/prog1", 65);
    msgid = msgget(key, 0666 | IPC_CREAT);

    printf("Wpisz imię i nazwisko: ");
    fgets(message, MSG_SIZE, stdin);

    message[strcspn(message, "\n")] = '\0';

    msgsnd(msgid, message, strlen(message)+1, 0);

    printf("Wiadomość wysłana: %s\n", message);
    
    msgrcv(msgid, message, MSG_SIZE, 0, 0);
    
    printf("Wiadomość odebrana: %s\n", message);
    
    
    msgctl(msgid, IPC_RMID, NULL);

    return 0;
}
