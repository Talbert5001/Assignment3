/*
Jiaqi Zhou/Talbert Hernton
CST-315
2/27/2022
Assignment 3
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>

int flag = 0; //indicator of status
//process one
void *process1();
//process two
void *process2();
//initialize resources
pthread_t t1, t2;
//first mutex
pthread_mutex_t first_mutex;
//second mutex
pthread_mutex_t second_mutex;
//function to follow after triggering alarm
void interrupt(int sig){
    //0 = not busy
    if(flag != 0){
    printf("\nTime to execute expired.\n");
    //unlock second mutex to allow process1 to grab t2
    pthread_mutex_unlock(&second_mutex);
    printf("The resources are releasing \n");
    }
}

int main(){
    //begin alarm for 4 seconds to check status of process1
    alarm(4);
    //tell the alarm what to trigger
    signal(SIGALRM, interrupt);
    //initialize first mutex
    pthread_mutex_init(&first_mutex, NULL);
    //initialize second mutex
    pthread_mutex_init(&second_mutex, NULL);
    //begin process1
    pthread_create(&t1, NULL, process1, NULL);
    //begin process2
    pthread_create(&t2, NULL, process2, NULL);
    //upon completion join threads
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    printf("Thread has joined\n");
}

void *process1(){
    //process1 grabs resource 1
    pthread_mutex_lock(&first_mutex);
    //change status
    flag++;
    printf("First mutex acquired by thread 1.\n");
    //sleep so that process2 can create deadlock
    sleep(1);
    //attempts to get resource 2
    pthread_mutex_lock(&second_mutex);
    printf("Second mutex acquired by thread 1.\n");
    //release resource 2
    pthread_mutex_unlock(&second_mutex);
    //change status
    flag--;
    printf("Second mutex released by thread 1.\n");
    //release resource 1
    pthread_mutex_unlock(&first_mutex);
    printf("First mutex released by thread 1.\n");
    return 0;
}
void *process2(){
    //process2 acquires resource 2 first
    pthread_mutex_lock(&second_mutex);
    flag++;
    printf("Second mutex acquired by thread 2.\n");
    //sleep so that process1 gets resource 1
    sleep(1);
    //attempts to get resource 1
    pthread_mutex_lock(&first_mutex);
    printf("First mutex acquired by thread 2.\n");
    //release resource 1
    pthread_mutex_unlock(&first_mutex);
    flag--;
    printf("First mutex released by thread 2.\n");
    //release resource 2
    pthread_mutex_unlock(&second_mutex);
    printf("Second mutex released by thread 2.\n");
    return 0;
}
