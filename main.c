#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <time.h>
#include <sys/types.h>
#include <sys/time.h>

sem_t controlSeats;
sem_t BarberReady;
sem_t CustReady;
pthread_mutex_t lock;

int seats = 0;


void* Barber(){
    while(1){
     if(seats > 0){

        sem_wait(&controlSeats);
        sem_wait(&CustReady);
        usleep(10000);
        seats--;
        sem_post(&controlSeats);

        pthread_mutex_lock(&lock);

        printf("Barber function : Removed Customer Barber Works...\n");

        pthread_mutex_unlock(&lock);

        sem_post(&BarberReady);
     }else{


        printf("Barber function :Barber sleeps...\n");
        usleep(10000);
        sem_post(&BarberReady);

     }

    }

}

void* Customer(){
    while(1){


        if(seats < 3){

        sem_wait(&controlSeats);
        seats++;
        //usleep(100000);
        printf("Customer Function :Added Customer Filling seat %d \n",seats);
        sem_post(&controlSeats);
        sem_post(&CustReady);

        sem_wait(&BarberReady);


        }else{


        printf("Customer Function :Customer tried to enter but there were no seats :( \n");
        usleep(10000);
        sem_post(&BarberReady);
        }

    }
}


int main()
{
    pthread_t BarberCuts;
    pthread_t addCustomer;

    sem_init(&controlSeats,0,1);
    sem_init(&BarberReady,0,0);
    sem_init(&CustReady,0,0);

    pthread_mutex_init(&lock, NULL);

    pthread_create(&addCustomer,NULL,&Customer,NULL);
    pthread_create(&BarberCuts,NULL,&Barber,NULL);

    pthread_join(BarberCuts,NULL);
    pthread_join(addCustomer,NULL);


    sem_destroy(&controlSeats);
    pthread_mutex_destroy(&lock);
    return 0;
}
