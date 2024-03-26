#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define MSG_SIZE 256
#define MSG_KEY 1234

struct msg_buffer {
    long msg_type;
    char msg_text[MSG_SIZE];
};

int main() {
    key_t key;
    int msgid;
    struct msg_buffer msg;

    key = ftok("/tmp/key", MSG_KEY);
    msgid = msgget(key, IPC_CREAT | 0666);
 

    while (1) {
        printf("Wpisz tekst: ");
        fgets(msg.msg_text, MSG_SIZE, stdin);
        msg.msg_text[strcspn(msg.msg_text, "\n")] = '\0';
        msg.msg_type = 1;
        msgsnd(msgid, &msg, sizeof(msg), 0) ;

    }

    return 0;
}
