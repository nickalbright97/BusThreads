//---------------------------------------------------------------------
// Assignment :   PA3 - IndyBus
// Date :         11/11/19
// Author :       Nick Albright & Miguel Padilla
// File Name :    tours.c
//----------------------------------------------------------------------

#include <time.h>
#include "wrappers.h"

sem_t textTex, busEmpty, canStart, beltsFastened, startSing, fastenAlert, songsSung, availSeats, tourDone, shmAccess, arrived;

int totalSeats, totalTourists, totalTrips, onBoard, inTown, tickets;

void reset_semaphores()
{
    Sem_init(&availSeats, 0, 0);
    Sem_init(&shmAccess, 0, 1);
    Sem_init(&tourDone, 0, 0);
    Sem_init(&canStart, 0, 0);
    Sem_init(&busEmpty, 0, 0);
    Sem_init(&beltsFastened, 0, 0);
    Sem_init(&startSing, 0, 0);
    Sem_init(&fastenAlert, 0, 0);
    Sem_init(&songsSung, 0, 0);
    Sem_init(&textTex, 0, 1);
    Sem_init(&arrived, 0, 0);
}

void *indy( void *ptr) {
    
    printf("\nIndy     : Hey! I Just Started My Day Waiting for ALL tourists to arrive in town\n\n");
    
    srandom(time(NULL));
    int randDur;
    int numTours = 0;
    int beltNumber = 0;
    int songNumber = 0;
       
    
    for (int i = 0; i < totalTourists; i++) 
        Sem_wait(&arrived);

    while (1 == 1) {
        if (tickets < 1) {
            break; // break out of while loop
        }

        randDur = random() % 2501 + 1500;
        Sem_wait(&textTex);
        printf("\nIndy     : New Tour. Declaring %d vacant seats\n", totalSeats); 
        Sem_post(&textTex);
        Sem_wait(&textTex);
        printf("Indy     : taking a nap till tourists get on board.\n"); 
        Sem_post(&textTex);
        onBoard = 0; // Clear Bus Seats

        for (int i = 0; i < totalSeats; i++) {
            Sem_post(&availSeats); // Seats are available
        }
        
        Sem_wait(&canStart);

        Sem_wait(&textTex);
        printf("Indy     : Welcome On Board Dear %d Passenger(s)! Please, fasten your seatbelts\n", onBoard); 
        Sem_post(&textTex);
        for (int i = 0; i < totalSeats; i++) {
            Sem_post(&fastenAlert); // tell passengers to fasten their seatbelts
        }

        while (beltNumber < onBoard) { 
            Sem_getvalue(&beltsFastened, &beltNumber);
        }; // wait til all belts are fastened

        Sem_wait(&textTex);
        printf("Indy     : Thank you all for fastening your seatbelts.\n");
        printf("Indy     : Tour will last for %d milliseconds\n", randDur);
        Sem_post(&textTex);
        
        Sem_wait(&textTex);
        printf("Indy     : Bus will now move. We All must Sing!\n");
        printf("Indy     : Bus! Bus! On the street! Who is the fastest driver to beat?\n");
        Sem_post(&textTex);
        for (int i = 0; i < totalSeats; i++) {
            Sem_post(&startSing);
        }        

        usleep(randDur); // Sleep for duration of tour
        while (songNumber < onBoard) { // wait til all passengers have sung
            Sem_getvalue(&songsSung, &songNumber);
        } 
        for (int i = 0; i < totalSeats; i++) {
            Sem_post(&tourDone); // Tour has ended
        }
        printf("Indy     : Tour is over. Thanks for your business\n");
        Sem_wait(&busEmpty); // All passengers have gotten off
        numTours++;

        reset_semaphores();
        beltNumber = 0;  //clear seat belts
    }
    
    printf("\nIndy     : Business is now closed. I did %2d tours today\n", numTours);

    return NULL;
}


void *tourist (void *ptr) {
    long tID;
    int curTour;    
    int isFastened = 0;

    srandom(time(NULL));
            
    tID = (long) ptr;
    curTour = 0;

    Sem_wait(&textTex);
    printf("Tourist %ld: Hey! I just arrived to Harrisonburg.\n", tID);
    
    
    Sem_post(&arrived);

    while (curTour < totalTrips) {

        
        long randDur;
        randDur = random() % 2001 + 500;
        

        printf("Tourist %ld: Tour # %d.  Going to shop for %ld milliseconds.\n", tID, curTour + 1, randDur);
        inTown++;
        Sem_post(&textTex);

        usleep(randDur);

        printf("Tourist %ld: Back from shopping, waiting for a seat on the bus\n", tID);
        while (1 == 1) {
            int availSeatsInt = 0;
            Sem_getvalue(&availSeats, &availSeatsInt);
            if (availSeatsInt > 0) {
                Sem_wait(&availSeats); // Wait for available seats
                break;
            }
        }

        onBoard++;
        inTown--;
        tickets--;

        printf("Tourist %ld: I got a seat on the bus.. CLICK\n", tID);
        if (onBoard >= totalSeats || inTown == 0) {
            Sem_post(&canStart);
        }
        
        Sem_wait(&fastenAlert);
        isFastened = 1;
        Sem_post(&beltsFastened); // fasten belt
        Sem_wait(&startSing); // wait for bus to move/driver to tell us to sing
        printf("Tourist %ld: The Wheels on the Bus go Round and Round!\n", tID);
        Sem_post(&songsSung); // tell driver i've sang
        Sem_wait(&tourDone); // wait til tour is done
        Sem_wait(&shmAccess); // Take turns getting off bus
        printf("Tourist %ld: Got off the bus\n", tID);
        onBoard--;
        if (onBoard == 0) {
            printf("Tourist %ld: Last to get off. Alerting Driver Bus is now empty \n", tID);
            Sem_post(&busEmpty);
        }
        Sem_post(&shmAccess);
        curTour ++;    
    }
    Sem_post(&textTex);

    srandom(time(NULL));
    int randDur;
    randDur = random() % 61 + 30;
    printf("Tourist %ld: Packing my luggage\n", tID);
    usleep(randDur); // Pack luggage
    printf("Tourist %ld: just left Town\n", tID);
}

int main(int argc, char **argv)
{

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

    onBoard = 0;
    inTown = 0;
    tickets = totalTrips * totalTourists;

    Sem_init(&availSeats, 0, 0);
    Sem_init(&shmAccess, 0, 1);
    Sem_init(&tourDone, 0, 0);
    Sem_init(&canStart, 0, 0);
    Sem_init(&busEmpty, 0, 0);
    Sem_init(&beltsFastened, 0, 0);
    Sem_init(&startSing, 0, 0);
    Sem_init(&fastenAlert, 0, 0);
    Sem_init(&songsSung, 0, 0);
    Sem_init(&textTex, 0, 1);
    Sem_init(&arrived, 0, 0);

    printf("*********************************************************************\n");
    printf("Operator:    Bus has %d seats. We expect %2d tourists today. Each will make %d tours.\n", 
        totalSeats, totalTourists, totalTrips);
    printf("*********************************************************************\n");

    long unsigned int indyID;
    // Create indy process
    Pthread_create(&indyID, NULL, indy, NULL);

    pthread_t tourists[totalTourists];

    for (unsigned long i = 0; i < totalTourists; i++)
    {
        Pthread_create(&tourists[i], NULL, tourist, (void *) (i+1)); // Create tourist thread
    }

    for (int i = 0; i < totalTourists; i++)
    {
        Pthread_join(tourists[i], NULL);    // Join tourist thread
    }

    Pthread_join(indyID, NULL); // join Indy thread

    Sem_destroy(&availSeats);
    Sem_destroy(&shmAccess);
    Sem_destroy(&tourDone);
    Sem_destroy(&busEmpty);
    Sem_destroy(&canStart);
    Sem_destroy(&beltsFastened);
    Sem_destroy(&startSing);
    Sem_destroy(&fastenAlert);
    Sem_destroy(&songsSung);
    Sem_destroy(&textTex);
    Sem_destroy(&arrived);
    
    printf("\nOPERATOR Terminated\n");
}



