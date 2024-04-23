#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void signal_handler(int signal) {
    printf("wersja 1\n");
    printf("IMIE NAZWISKO\n");
    printf("INDEX\n\n");
    alarm(1);
}

int main() {
    signal(SIGALRM, signal_handler);
    alarm(1);

    while(1) {
        pause();
    }

    return 0;
}
