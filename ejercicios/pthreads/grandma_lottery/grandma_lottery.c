#include <pthread.h>
#include <stdlib.h>
#include <sys/random.h>
#include <stdio.h>

void* number(void* data);


int main(void) {


    
    size_t numero_ran1 = 0;
    size_t numero_ran2 = 0;

    // void* temp;
    pthread_t thread_ran1;
    pthread_create(&thread_ran1, NULL, number, NULL);

    pthread_t thread_ran2;
    pthread_create(&thread_ran2, NULL, number, NULL);


    pthread_join(thread_ran1, (void*)&numero_ran1);
    pthread_join(thread_ran2, (void*)numero_ran2);

    //numero_ran1 = (size_t) temp;
    printf("Numero:%zu\n", numero_ran1);
    

}


void* number(void* data) {
  
  (void) data;
  size_t numeroran =  99; 
  void* ptr = (void*)numeroran;
  return ptr;
}