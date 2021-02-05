#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>

int dato=0; /* dato compartido por los hilos */
sem_t s; //Semáforo

void* asigna(void *arg); //Hilos

#define NUM_HILOS 5

int main(int argc, char *argv[]) {
  pthread_t tid[NUM_HILOS]; /* the thread identifier */
  int datos[5];

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

  //Inicia Semaforo
  sem_init(&s, 0, 4);

  /* get the default attributes */
  pthread_attr_init(&attr);

  //Crea los hilos
  for (int i = 0; i < NUM_HILOS; i++)
  {
      datos[i] = i + 1;
      pthread_create(&tid[i], &attr, asigna, &datos[i]);
  }

  //Espera a los hilos
  for (int i = 0; i < NUM_HILOS; i++)
  {
      pthread_join(tid[i], NULL);
  }

  printf("dato = %d\n", dato);
}

void* asigna(void *arg) {

  int par = *(int *)arg;

  //Sección critica
  int t = dato;
  printf("Saludos desde el hilo %d\n", par);
  sleep (1);
  t = t * par;
  dato = t;
  printf("Adios desde el hilo %d\n", par);

  //Termina
  pthread_exit(0);

}