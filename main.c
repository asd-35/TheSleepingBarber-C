#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <time.h>
#include <sys/types.h>
#include <sys/time.h>

sem_t CustReady;
sem_t BarberReady;
sem_t NumberOfSeats;

int seats = 4;

void* Barber(){
    while(1){

    }

}

void* Customer(){
    while(1){

    }

}


int main()
{

    return 0;
}
