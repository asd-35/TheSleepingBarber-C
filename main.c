#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <time.h>
#include <sys/types.h>
#include <sys/time.h>

sem_t controlSeats; //to control the seats
sem_t BarberReady;  // to control the barber
sem_t CustReady;   // to control the customer flow
pthread_mutex_t lock; // lock to cut hair

int seats = 0; // to count the seats available (max 4)


void* Barber(){ //Barber function as a pointer to assign it as a thread
    while(1){ // infinite loop to examine the flow
     if(seats > 0){ // if some or all the seats are full

        sem_wait(&controlSeats); // locks the seats
        sem_wait(&CustReady); // locks the customers to go in
        usleep(10000); // sleeps for customer walking in
        seats--; // makes 1" seat available
        sem_post(&controlSeats); // unlocks the seats

        pthread_mutex_lock(&lock); // makes a mutex lock to cut hair

        printf("Barber function : Removed Customer Barber Works...\n");

        pthread_mutex_unlock(&lock); // unlocks the lock

        sem_post(&BarberReady); // makes barber available
     }else{

        //the barber sleeps if there is no one in waiting state
        printf("Barber function :Barber sleeps...\n");
        usleep(10000); // barber sleeps
        sem_post(&BarberReady); // barber slept he/she is ready to work again

     }

    }

}

void* Customer(){ //Customer function as a pointer to assign it as a thread
    while(1){ // infinite loop to examine the flow


        if(seats < 3){ // if there is seats available

        sem_wait(&controlSeats); // locks the seats someone is coming in
        seats++; // someone came to the shop

        printf("Customer Function :Added Customer Filling seat %d \n",seats);
        sem_post(&controlSeats); // unlocks the seats
        sem_post(&CustReady); // makes a customer ready to go

        sem_wait(&BarberReady); // puts barber in waiting state checks if he is available by waiting the sem_post() for barberReady in barber function


        }else{


        printf("Customer Function :Customer tried to enter but there were no seats :( \n");
        usleep(10000);
        sem_post(&BarberReady); // every 10000 ms someone comes to the shop if he cant enter makes sure the barber works by invoking the barberReady
        }

    }
}


int main()
{
    // thread objects for barber and customer function
    pthread_t BarberCuts;
    pthread_t addCustomer;
    //semaphore creation for described locks in functions
    sem_init(&controlSeats,0,1);
    sem_init(&BarberReady,0,0);
    sem_init(&CustReady,0,0);
    //mutex creation for described locks in functions
    pthread_mutex_init(&lock, NULL);
    //threads created
    pthread_create(&addCustomer,NULL,&Customer,NULL);
    pthread_create(&BarberCuts,NULL,&Barber,NULL);
    // threads started running simutaneously
    pthread_join(BarberCuts,NULL);
    pthread_join(addCustomer,NULL);

    //semaphores and mutexs destroyed
    sem_destroy(&controlSeats);
    sem_destroy(&BarberReady);
    sem_destroy(&CustReady);
    pthread_mutex_destroy(&lock);
    return 0;
}
