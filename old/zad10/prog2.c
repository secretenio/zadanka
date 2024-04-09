#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
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
        sleep(5);
        printf("TICK\n");

        while (msgrcv(msgid, &msg, sizeof(msg), 1, IPC_NOWAIT) != -1) {
            printf("Odebrano: %s\n", msg.msg_text);
        }

        if (errno != ENOMSG) {
            perror("msgrcv");
            exit(1);
        }
    }

    return 0;
}
