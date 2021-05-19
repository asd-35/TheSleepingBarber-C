#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <time.h>
#include <sys/types.h>
#include <sys/time.h>

sem_t controlSeats;
pthread_mutex_t lock;

int seats = 0;


void* Barber(){

     if(seats > 0){
        pthread_mutex_lock(&lock);
        printf("Barber function :Barber Works...\n");
        usleep(100000);
        seats--;
        pthread_mutex_unlock(&lock);
     }else{
        pthread_mutex_lock(&lock);
        printf("Barber function :Barber sleeps...\n");
        pthread_mutex_unlock(&lock);
     }



}

void* Customer(){
    while(1){


        sem_wait(&controlSeats);
        Barber();
        seats++;
        printf("Customer Function :Filling seat %d \n",seats);
        usleep(100000);
        sem_post(&controlSeats);


    }
}


int main()
{
    pthread_t BarberCuts;
    pthread_t addCustomer;

    sem_init(&controlSeats,0,1);
    pthread_mutex_init(&lock, NULL);

    pthread_create(&addCustomer,NULL,&Customer,NULL);
    pthread_create(&BarberCuts,NULL,&Barber,NULL);

    pthread_join(addCustomer,NULL);
    pthread_join(BarberCuts,NULL);

    sem_destroy(&controlSeats);
    pthread_mutex_destroy(&lock);
    return 0;
}
