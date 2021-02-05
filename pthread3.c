/*
  ============================================================================
  Name        : prueba_simple.c
  Author      : Rene
  Version     : 0.1
  Copyright   : Your copyright notice
  Description : ejemplo con pthreads
  ============================================================================
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>

// Variables compartidas por hilos
int dato=0; /* dato compartido por los hilos */
sem_t s; // Semaforo

void* resta(); /* hilo 1 */
void* suma(); /* hilo 2 */


int main(int argc, char *argv[]) {
  pthread_t tid1,tid2; /* the thread identifier */
  pthread_attr_t attr; /* set of attributes for the thread */
  if (argc != 2) {
    fprintf(stderr, "usage: a.out <integer value>\n");
    /*exit(1);*/
    return -1;
  }
  if (atoi(argv[1]) < 0) {
    fprintf(stderr, "Argument %d must be non-negative\n", atoi(argv[1]));
    /*exit(1);*/
    return -1;
  }

  /* El parametro que recibo es el valor inicial de dato */
  dato = atoi(argv[1]);
  
  // Inicia semaforo
  sem_init(&s,0,1);

  /* get the default attributes */
  pthread_attr_init(&attr);

  /* create the thread 1*/
  pthread_create(&tid1, &attr, resta, NULL);
  /* create the thread 2*/
  pthread_create(&tid2, &attr, suma, NULL);
  
  /* espera a los hilos */
  pthread_join(tid1, NULL);
  
  pthread_join(tid2, NULL);
  
  printf("dato = %d\n", dato);
}
/* Este hilo resta */
void* resta() {

  // Espera el semaforo
  sem_wait(&s);
  
  int t = dato;

  sleep(2);

  t = t-200;

  dato = t;

  // da paso semaforo
  sem_post(&s);

  pthread_exit(0);
}
/* Este hilo suma */
void* suma() {
  // Espera el semaforo
  sem_wait(&s);

  int t = dato;

  sleep(3);

  t = t + 200;
  dato = t;

  // da paso semaforo
  sem_post(&s);

  pthread_exit(0);
}