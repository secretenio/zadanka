#include <stdio.h>
#include <signal.h>
#include <unistd.h>

volatile sig_atomic_t flag = 0;

void signal_handler(int signal) {
    flag = 1;
}

int main() {
    signal(SIGALRM, signal_handler);
    alarm(1);

    while(1) {
        sleep(5);
        if (flag) {
            printf("wersja 2\n");
            printf("IMIE NAZWISKO\n");
            printf("INDEX\n\n");
            flag = 0;
            alarm(1);
        }
    }

    return 0;
}
