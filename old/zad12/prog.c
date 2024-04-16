#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/sem.h>
#include <sys/shm.h>
 
#define SHM_SIZE 2
 

int main() {
    key_t key_shm = ftok("/tmp/prog", 65);
    key_t key1 = ftok("/tmp/prog", 65);
    key_t key2 = ftok("/tmp/prog", 65);
    int shmid, semid1, semid2;
    int *shared_memory;
    struct sembuf sop1, sop2;
 

    shmid = shmget(key_shm, SHM_SIZE * sizeof(int), IPC_CREAT | 0666);
 
    shared_memory = shmat(shmid, NULL, 0);
    semid1 = semget(key1, 1, IPC_CREAT | 0666);
    semid2 = semget(key2, 1, IPC_CREAT | 0666);
    
    semctl(semid1, 0, SETVAL, 0);
    semctl(semid2, 0, SETVAL, 0);
    
    printf("1 - Zajęcie semafora 1\n");
    printf("q - Zwolnienie semafora 1\n");
    printf("2 - Zajęcie semafora 2\n");
    printf("w - Zwolnienie semafora 2\n");
    printf("Ctrl+C - Wyjście\n");
 
    while (1) {
        char input;
        scanf(" %c", &input);
 
        switch (input) {
            case '1':
                sop1.sem_num = 0;
                sop1.sem_op = -1;
                sop1.sem_flg = IPC_NOWAIT;
                
                semop(semid1, &sop1, 1);
                
                printf("Semafor 1 zajęty\n");
                shared_memory[0] = 1;
                break;
            case 'q':
                sop1.sem_num = 0;
                sop1.sem_op = 1;
                sop1.sem_flg = 0;
                
                semop(semid1, &sop1, 1);
                
                printf("Semafor 1 zwolniony\n");
                shared_memory[0] = 0;
                break;
            case '2':
                sop2.sem_num = 0;
                sop2.sem_op = -1;
                sop2.sem_flg = IPC_NOWAIT;
                
                semop(semid2, &sop2, 1);
                
                printf("Semafor 2 zajęty\n");
                shared_memory[1] = 1;
                break;
            case 'w':
                sop2.sem_num = 0;
                sop2.sem_op = 1;
                sop2.sem_flg = 0;
                
                semop(semid2, &sop2, 1);
                
                printf("Semafor 2 zwolniony\n");
                shared_memory[1] = 0;
                break;
            default:
                printf("Niepoprawny wybór\n");
        }
 
        if (shared_memory[0] == 1 && shared_memory[1] == 1) {
            printf("Zakleszczenie! Oba semafory są zajęte.\n");
        }
    }
 
    return 0;
}
