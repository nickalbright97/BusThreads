//---------------------------------------------------------------------
// Assignment :   PA3 - IndyBus
// Date :         11/11/19
// Author :       Nick Albright & Miguel Padilla
// File Name :    tours.c
//----------------------------------------------------------------------


#include "wrappers.h"

void * indy( void * ptr) {

    busArgs *args = (busArgs *) ptr;

    printf("*********************************************************************\n");
    printf("Indy:    Bus has %d seats. We expect %2d tourists today. Each will make %d tours.\n", 
        args->totalSeats, args->totalTourists, args->totalTrips);
    printf("*********************************************************************\n");

    return NULL;
}

int main(int argc, char **argv)
{
    int totalSeats, totalTourists, totalTrips;

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

    long unsigned int indyID;
    // Create indy process
    Pthread_create(&indyID, NULL, indy, (void *) argStruct);
    printf("main() created a peer thread with ID=%u\n", (unsigned) indyID); // debug line
    Pthread_join(indyID, NULL);
    printf("joined\n"); // debug line

    free(argStruct);

}
