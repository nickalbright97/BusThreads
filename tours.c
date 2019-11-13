//---------------------------------------------------------------------
// Assignment :   PA3 - IndyBus
// Date :         11/11/19
// Author :       Nick Albright & Miguel Padilla
// File Name :    tours.c
//----------------------------------------------------------------------

#include <time.h>
#include "wrappers.h"

sem_t availSeats, arrived, print, shmAccess, finishSing, busMove, tourDone, tourFull, busEmpty;


void *indy( void *ptr) {
    int shmflg, shmid;
    key_t shmkey;

    shmBus *sBus;

    shmflg = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH | IPC_CREAT;
    shmkey = ftok("shmBus.h", PROJ_ID);
    shmid = Shmget(shmkey, SHMEM_SIZE, shmflg);
    sBus = Shmat(shmid, NULL, 0);



    busArgs *args = (busArgs *) ptr;
    
    printf("Indy     : Hey! I Just Started My Day Waiting for ALL tourists to arrive in town\n");
    

    srandom(time(NULL));
    int randDur;
    randDur = random() % 2501 + 1500;
    
    int numTours;
       
    while (numTours < args->totalTrips) {


        
    }
    

    return NULL;
}


void *tourist (void *ptr) {
    long tID;
    int curTour;    
    int shmflg, shmid;
    key_t shmkey;

    shmBus *sBus;


    shmflg = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH | IPC_CREAT;
    shmkey = ftok("shmBus.h", PROJ_ID);
    shmid = Shmget(shmkey, SHMEM_SIZE, shmflg);
    sBus = Shmat(shmid, NULL, 0);

    tID = (long) ptr;
    curTour = 0;
    printf("Tourist %ld:  Hey! I just arrived to Harrisonburg.\n", tID);
    //NEEDS TOURIST NUMBER

    while (curTour < sBus->totalTrips) {

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
    shmkey = ftok("shmBus.h", PROJ_ID);
    shmid = Shmget(shmkey, SHMEM_SIZE, shmflg);
    sBus = Shmat(shmid, NULL, 0);

    //initialize semaphores
    int semflg, semmode;
    semflg = O_CREAT;
    semmode = S_IRUSR | S_IWUSR;


    Sem_init(&availSeats, 0, 0);
    Sem_init(&arrived, 0, 0);
    Sem_init(&print, 0, 1);
    Sem_init(&shmAccess, 0, 1);
    Sem_init(&finishSing, 0, 0);
    Sem_init(&tourDone, 0, 0);
    Sem_init(&tourFull, 0, 0);
    Sem_init(&busEmpty, 0, 0);

    long unsigned int indyID;
    // Create indy process
    Pthread_create(&indyID, NULL, indy, (void *) argStruct);
    printf("main() created a peer thread with ID=%u\n", (unsigned) indyID); // debug line
    Pthread_join(indyID, NULL);
    printf("joined\n"); // debug line

    printf("*********************************************************************\n");
    printf("OPERATOR:    Bus has %d seats. We expect %2d tourists today. Each will make %d tours.\n", 
        argStruct->totalSeats, argStruct->totalTourists, argStruct->totalTrips);
    printf("*********************************************************************\n");

    pthread_t tourists[totalTourists];

    for (unsigned long i = 0; i < totalTourists; i++)
    {
        Pthread_create(&tourists[i], NULL, tourist, (void *) (i+1));
    }

    for (int i = 0; i < totalTourists; i++)
    {
        Pthread_join(tourists[i], NULL);    
    }
    
    
    

    Sem_destroy(&availSeats);
    Sem_destroy(&arrived);
    Sem_destroy(&print);
    Sem_destroy(&shmAccess);
    Sem_destroy(&finishSing);
    Sem_destroy(&tourDone);
    Sem_destroy(&tourFull);
    Sem_destroy(&busEmpty);


    free(argStruct);
    
    printf("OPERATOR Terminated");
}



