#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "SemaphoreStruct.h"

#define INT_SEG_SIZE 5
#define STRUCT_SEG_SIZE 10
#define CASHIER_QUEUE_SIZE 100
int *attachIntSegment(int pShmID);

int main()
{
    stopPrograms();
    destroySegments();
    destroySemaphores();
    return 0;
}
void stopPrograms(){
    key_t stopClientsKey = 5681;
    key_t stopSpecialClientsKey = 5682;

    int idClients, idSpecialClients;
    int iKey;
    idClients = locateSegment(stopClientsKey, INT_SEG_SIZE);
    idSpecialClients = locateSegment(stopSpecialClientsKey, INT_SEG_SIZE);

    int *stopClientsPtr = attachIntSegment(idClients);
    int *stopSpecialClientsPtr = attachIntSegment(idSpecialClients);
    *stopClientsPtr = 0;
    printf("Programa de clientes detenido\n");
    *stopSpecialClientsPtr = 0;
    printf("Programa de clientes especiales detenido\n");

}
void destroySemaphores(){
    char CHAIRS_SEM[] = "ChairsSem";
    char BARBERS_SEM[] = "BarbersSem";
    char CASHIER_SEM[] = "CashierSem";
    char FILE_SEM[] = "FileSem";
    char S_CLIENTS_COUNTER_SEM[] = "SpecialClientsCounter";
    char* semaphores[]= {CHAIRS_SEM,BARBERS_SEM,CASHIER_SEM,FILE_SEM,S_CLIENTS_COUNTER_SEM};
    destroySemaphore(CHAIRS_SEM);
    destroySemaphore(BARBERS_SEM);
    destroySemaphore(CASHIER_SEM);
    destroySemaphore(S_CLIENTS_COUNTER_SEM);
    destroySemaphore(FILE_SEM);

}

void destroySemaphore (char *pName){
    Semaphore *semaphore = malloc(sizeof(Semaphore));
    semaphore->name = pName;

    semaphore->mutex = sem_open(pName,0,0644,1);
    if(semaphore->mutex == SEM_FAILED){
        printf("Error destruyendo semaforo: %s \n", pName);
        sem_unlink(pName);
        exit(-1);
    }
    if (sem_destroy(semaphore->mutex)==0){
        printf("Semaforo %s eliminado... \n", pName);
    }
}
void destroySegments(){
    key_t chairsKey, barbersKey, cashierKey, specialClientsCounterKey, stopClientsKey, stopSpecialClientsKey,chairsQuantityKey,barbersQuantityKey;
    chairsKey = 5677;
    barbersKey = 5678;
    cashierKey = 5679;
    specialClientsCounterKey = 5680;
    stopClientsKey = 5681;
    stopSpecialClientsKey = 5682;
    chairsQuantityKey = 5683;
    barbersQuantityKey = 5684;

    int chairsSize = STRUCT_SEG_SIZE;
    int barbersSize = STRUCT_SEG_SIZE;
    int cashierSize = CASHIER_QUEUE_SIZE;
    int specialClientsCounterSize = INT_SEG_SIZE;
    int stopClientsSize = INT_SEG_SIZE;
    int stopSpecialClientsSize = INT_SEG_SIZE;
    int chairsQuantitySize = INT_SEG_SIZE;
    int barbersQuantitySize = INT_SEG_SIZE;

    int keysSize[] = { chairsSize, barbersSize, cashierSize, specialClientsCounterSize, stopClientsSize, chairsQuantitySize, barbersQuantitySize};
    key_t keys[]= {chairsKey, barbersKey, cashierKey, specialClientsCounterKey, stopClientsKey, stopSpecialClientsKey,chairsQuantityKey,barbersQuantityKey};

    int shmid;
    int iKey;
    for(iKey = 0; iKey!= sizeof(keys)/4; iKey++){
        shmid = locateSegment(keys[iKey], keysSize[iKey]);
        destroySegment(shmid,keys[iKey]);
    }
    key_t iKey_t = 0;
    for(iKey_t = 5685; iKey_t!= 6000; iKey_t++){
        shmid = locateSegment(iKey_t, STRUCT_SEG_SIZE);
        destroySegment(shmid,iKey_t);
    }

}
void destroySegment(int pShmid, int pKey){
    if (pShmid != -1){
        shmctl(pShmid, IPC_RMID, NULL);
        printf("Segmento %d eliminado\n",pKey);
    }
}
int locateSegment(key_t pKey, int pSize){
    int shmid;
    if ((shmid = shmget(pKey, pSize, 0666)) < 0) {
        //printf("Segmento %d no localizado\n",pKey);
        return -1;
    }
    return shmid;
}
/// <summary>
/// Attaches an int value to a segment
/// </summary>
int *attachIntSegment(int pShmID){

    int *pointer;
    if ((pointer = shmat(pShmID, NULL, 0)) == (int *) -1) {
        printf("Error adjuntando segmento con llave: %d \n",pShmID);
        //exit(1);
    }
    //printf("Segment attached ... \n");
    return pointer;
}

