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

    key = ftok("/tmp/prog1", 65);
    msgid = msgget(key, 0666 | IPC_CREAT);

    msgrcv(msgid, message, MSG_SIZE, 0, 0);

    for (int i = 0; i < strlen(message); i++) {
        message[i] = toupper(message[i]);
    }

    msgsnd(msgid, message, strlen(message)+1, 0);
    
    printf("Wiadomość wysłana!\n");

    return 0;
}
