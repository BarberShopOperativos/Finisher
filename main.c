#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#define SHMSZ     50
int main()
{
    key_t chairsKey, barbersKey, cashierKey, specialClientsCounterKey, stopClientsKey, stopSpecialClientsKey,chairsQuantityKey,barbersQuantityKey;
    chairsKey = 5677;
    barbersKey = 5678;
    cashierKey = 5679;
    specialClientsCounterKey = 5680;
    stopClientsKey = 5681;
    stopSpecialClientsKey = 5682;
    chairsQuantityKey = 5683;
    barbersQuantityKey = 5684;

    int chairsSize = 25;
    int barbersSize = 25;
    int cashierSize = 25;
    int specialClientsCounterSize = 10;
    int stopClientsSize = 10;
    int stopSpecialClientsSize = 10;
    int chairsQuantitySize = 10;
    int barbersQuantitySize = 10;

    int keysSize[] = { chairsSize, barbersSize, cashierSize, specialClientsCounterSize, stopClientsSize, chairsQuantitySize, barbersQuantitySize};
    key_t keys[]= {chairsKey, barbersKey, cashierKey, specialClientsCounterKey, stopClientsKey, stopSpecialClientsKey,chairsQuantityKey,barbersQuantityKey};

    int shmid;
    int iKey;
    for(iKey = 0; iKey!= sizeof(keys)/4; iKey++){
        shmid = locateSegment(keys[iKey], keysSize[iKey]);
        destroySegment(shmid,keys[iKey]);
    }

    return 0;
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
        printf("Segmento %d no localizado\n",pKey);
        return -1;
    }
    return shmid;
}
