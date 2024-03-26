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
    struct msg_buffer message;

    // Tworzenie unikalnego klucza
    key = ftok("prog1", 65);
    // Tworzenie kolejki komunikatów
    msgid = msgget(key, 0666 | IPC_CREAT);

    printf("Wpisz numer indeksu: ");
    fgets(message.msg_text, MSG_SIZE, stdin);
    // Usunięcie znaku nowej linii na końcu wprowadzonego tekstu
    message.msg_text[strcspn(message.msg_text, "\n")] = '\0';
    message.msg_type = 1;

    // Wysłanie komunikatu do kolejki
    msgsnd(msgid, &message, sizeof(message), 0);

    printf("Wiadomość wysłana: %s\n", message.msg_text);

    return 0;
    
  }
