#include<stdio.h>
#include<sys/sem.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>

union sem_num {

    int val;
    struct semid_ds * buf;
    unsigned short int * array;
};

int main()
{
    key_t key1;
    key1=ftok(".",'z');
    int semid1=semget(key1,1,IPC_CREAT|0666);
    union sem_num arg1;
    arg1.val=1;
    semctl(semid1,0,SETVAL,arg1);
}