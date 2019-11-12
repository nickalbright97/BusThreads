//---------------------------------------------------------------------
// Assignment :   PA3 - IndyBus
// Date :         11/11/19
// Author :       Nick Albright & Miguel Padilla
// File Name :    shmBus.h
//----------------------------------------------------------------------

#ifndef SHMEM_BUS
#define SHMEM_BUS


typedef struct {
    int totalSeats; // Total seats on bus (regardless of occupancy)
    int totalTourists; // Total Tourists that day
    int totalTrips; // Total trips for each tourist(These three could be passed by argument to Indy thread function)
    int onBoard; // Number of tourists on board the bus
    int inTown; // Numer of tourists in town
    int tickets; // Total tickets for today = totalTourists * totalTrips
} shmBus ;

typedef struct {
    int totalSeats; // Total seats on bus (regardless of occupancy)
    int totalTourists; // Total Tourists that day
    int totalTrips; // Total trips for each tourist
} busArgs;

#define SHMEM_SIZE sizeof(shmBus)
#define BUS_ARGS_SIZE sizeof(busArgs)
#define PROJ_ID 13

#endif
