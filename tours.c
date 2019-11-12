//---------------------------------------------------------------------
// Assignment :   PA3 - IndyBus
// Date :         11/11/19
// Author :       Nick Albright & Miguel Padilla
// File Name :    tours.c
//----------------------------------------------------------------------

#include <time.h>
#include "wrappers.h"

void *indy( void *ptr) {

    shmBus *sBus;

    shmflg = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH | IPC_CREAT;
    shmkey = Ftok("shmBus.h", PROJ_ID);
    shmid = Shmget(shmkey, SHMEM_SIZE, shmflg);
    sBus = Shmat(shmid, NULL, 0);


    busArgs *args = (busArgs *) ptr;
    
    printf("Indy     : Hey! I Just Started My Day Waiting for ALL tourists to arrive in town\n");
    

    srandom(time(NULL));
    int randDur;
    randDur = random() % 2501 + 1500;
    
    
    

    return NULL;
}


void *tourist (void *ptr) {
    int curTour;    

    shmBus *sBus;

    shmflg = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH | IPC_CREAT;
    shmkey = Ftok("shmBus.h", PROJ_ID);
    shmid = Shmget(shmkey, SHMEM_SIZE, shmflg);
    sBus = Shmat(shmid, NULL, 0);

    curTour = 0;
    //printf("Tourist %d:  Hey! I just arrived to Harrisonburg.\n", );
    //NEEDS TOURIST NUMBER

    while (curTour < ptr->totalTrips) {

        srandom(time(NULL));
        int randDur;
        randDur = random() % 2001 + 500;
        printf("Tourist :  Tour # 1.  Going to shop for %d milliseconds.\n", randDur);
    
     
        printf("Tourist :  Packing my luggage.\n");
        curTour ++;    
    }

}

int main(int argc, char **argv)
{
    int totalSeats, totalTourists, totalTrips;
    int shmflg, shmid;
    key_t shmkey;
    shmBus *sBus;
    busArgs bArgs;
    sem_t availSeats;
    sem_t arrived;


    int factory_lines, order_size;
    if (argc != 4) { 
        perror("Must be three arguments: seats on the bus, number of tourists, and trips each tourist takes");     
        exit(EXIT_FAILURE);
    }
    
    // Parse arguments from command line
    if (sscanf (argv[1], "%i", &totalSeats) != 1) {
        perror("error - not an integer");
    }
    if (sscanf (argv[2], "%i", &totalTourists) != 1) {
        perror("error - not an integer");
    }
    if (sscanf (argv[3], "%i", &totalTrips) != 1) {
        perror("error - not an integer");
    }

    
    busArgs *argStruct= malloc(BUS_ARGS_SIZE); // in shmBus.h, used to pass command line arguments by struct
    argStruct->totalSeats = totalSeats;
    argStruct->totalTourists = totalTourists;
    argStruct->totalTrips = totalTrips;

    shmflg = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH | IPC_CREAT;
    shmkey = Ftok("shmBus.h", PROJ_ID);
    shmid = Shmget(shmkey, SHMEM_SIZE, shmflg);
    sBus = Shmat(shmid, NULL, 0);


    Sem_init(availSeats, 0, 0);
    Sem_init(arrived, 0, 0);

    long unsigned int indyID;
    // Create indy process
    Pthread_create(&indyID, NULL, indy, (void *) argStruct);
    printf("main() created a peer thread with ID=%u\n", (unsigned) indyID); // debug line
    Pthread_join(indyID, NULL);
    printf("joined\n"); // debug line

    printf("*********************************************************************\n");
    printf("OPERATOR:    Bus has %d seats. We expect %2d tourists today. Each will make %d tours.\n", 
        args->totalSeats, args->totalTourists, args->totalTrips);
    printf("*********************************************************************\n");

    pthread_t tourists[totalTourists];

    for (int i = 0; i < totalTourists; i++)
    {
        Pthread_create(&tourists[i], NULL, tourist, (void* ) i+1);
    }

    for (int i = 0; i < totalTourists; i++)
    {
        Pthread_join(tourists[i], NULL);    
    }
    
    
    Sem_close(availSeats);
    Sem_close(arrived);
    
    free(argStruct);
    
    printf("OPERATOR Terminated");
}



