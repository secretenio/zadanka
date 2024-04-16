#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/sem.h>
 
#define SHM_SIZE 1024
 
union semun {
    int              val;
    struct semid_ds *buf;
    unsigned short  *array;
    struct seminfo  *__buf;
};
 
 
int main() {
    key_t key = ftok("/tmp/prog", 65);
    int shmid, semid;
    char *shmaddr;
    struct sembuf sop;
    union semun arg;
 
    shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666);
    shmaddr = shmat(shmid, NULL, 0);
    semid = semget(key, 1, IPC_CREAT | 0666);
    
    arg.val = 1;
    semctl(semid, 0, SETVAL, arg);

 
    while (1) {
        sop.sem_num = 0;
        sop.sem_op = -1;
        sop.sem_flg = 0;
        semop(semid, &sop, 1);
        
        printf("Wpisz tekst: ");
        fgets(shmaddr, SHM_SIZE, stdin);
 
        sop.sem_op = 1;
        semop(semid, &sop, 1);

    }
 
    return 0;
}
