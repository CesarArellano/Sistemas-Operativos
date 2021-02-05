/*
  ============================================================================
  Name        : pthreads.c
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

// Variables compartidas por los hilos
int dato=0; /* dato compartido por los hilos */
int flag[2]; /* 0 es falso y 1 es verdadero */
int turn;
pthread_mutex_t lock; //Cerrojo

void* resta(); /* hilo 1 */
void* suma(); /* hilo 2 */
int main(int argc, char *argv[]) {
  pthread_t tid1,tid2; /* the thread identifier */
  pthread_attr_t attr; /* set of attributes for the thread */
  if (argc != 2) {
    fprintf(stderr, "usage: %s <integer value>\n",argv[0]);
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
	
  /* get the default attributes */
  pthread_attr_init(&attr);

  /* create the thread 1*/
  pthread_create(&tid1, &attr, resta, NULL);

  /* create the thread 2*/
  pthread_create(&tid2, &attr, suma, NULL);
	
  /* espera a los hilos */
  pthread_join(tid2, NULL);
  pthread_join(tid1, NULL);
  
  printf("dato = %d\n", dato);
}
/**
 * Este hilo resta, proceso 0
 */
void* resta() {

  // Cierra cerrojo
  pthread_mutex_lock(&lock);
  // Seccion crítica
  int t = dato;

  sleep(2);

  t = t - 200;

  dato = t;
  // Libera cerrojo
  pthread_mutex_unlock(&lock);
  // Termina
  pthread_exit(0);
}
/**
 * Este hilo suma, proceso 1
 */
void* suma() {

  pthread_mutex_lock(&lock);
  // Seccion crítica
  int t = dato;

  sleep(3);

  t = t + 200;

  dato = t;

  pthread_mutex_unlock(&lock);
  // Termina
  pthread_exit(0);
}