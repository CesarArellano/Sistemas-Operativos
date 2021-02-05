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
#include <time.h>
#include <semaphore.h>

int dato = 0; /* dato compartido por los hilos */
int size = 0;
int *array;
sem_t s;
void *resta(); /* hilo 1 */
void *suma(); /* hilo 2 */
void print(int array[], int length);
void *bubbleSort(void *ar);
void mix(int array[], int first, int middle, int last);

struct args {
  int *array;
  int l;
};

int main(int argc, char *argv[]) {
  pthread_t tid1, tid2; /* the thread identifier */
  pthread_attr_t attr; /* set of attributes for the thread */
  
  if (argc != 2) {
    fprintf(stderr, "usage: %s <integer value>\n", argv[0]);
    /*exit(1);*/
    return -1;
  }
  
  if (atoi(argv[1]) < 0) {
    fprintf(stderr, "Argument %d must be non-negative\n", atoi(argv[1]));
    /*exit(1);*/
    return -1;
  }

  /* El parametro que recibo es el valor inicial de dato */
  size = atoi(argv[1]);

  sem_init(&s,1,0);

  array = malloc(size * sizeof(int));

  for(int i = 0; i < size; i++) {
    array[i] = rand() % size;
  }

  long start = clock();
  
  /* get the default attributes */
  pthread_attr_init(&attr);

  struct args a1;
  a1.array = array;
  a1.l = size / 2;

  /* create the thread 1*/
  // pthread_create(&tid1, &attr, resta, NULL);
  pthread_create(&tid1, &attr, bubbleSort, (void *)&a1);

  struct args a2;
  a2.array = &array[size / 2];
  a2.l = size / 2;

  /* create the thread 2*/
  // pthread_create(&tid2, &attr, suma, NULL);
  pthread_create(&tid2, &attr, bubbleSort, (void *)&a2);
  
  /* espera a los hilos */
  pthread_join(tid1, NULL);
  pthread_join(tid2, NULL);
  
  mix(array, 0, size/2, size - 1);

  long stop = clock();
  double time = stop - start;
  time = time / ((double)CLOCKS_PER_SEC);
  
  print(array, 100);
  printf("Se tardo %f segundos\n", time);

}

void *bubbleSort(void *ar) {
    int *a, length;
    int i, j, temp;

    a = ((struct args *)ar)->array;
    length = ((struct args *)ar)->l;

    for (i = 0; i < length; i++) {
        for (j = 0; j < length - i - 1; j++) {
            if (a[j + 1] < a[j]) {
                temp = a[j];
                a[j] = a[j + 1];
                a[j + 1] = temp;
            }
        }
    }
}

void mix(int array[], int first, int middle, int last) {

    int t = 0, s = first;
    int half = middle - 1;
    int size = last - first + 1;
    int *aux = malloc(sizeof(int) * (size));

    while((first <= half) && (middle <= last)) {
        if (array[first] <= array[middle]) {
            aux[t] = array[first];
            first++;
            t++;
        } else {
            aux[t] = array[middle];
            middle++;
            t++;
        }
    }

    while(first <= half) {
        aux[t++] = array[first++];
    }

    while(middle <= last) {
        aux[t++] = array[middle++];
    }

    for(int i = 0; i <= size; i++) {
        array[s+i] = aux[i];
    }
    
}

void print(int array[], int length) {
    for(int i = 0; i < length; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");
}

/* Este hilo resta */
void *resta() {
  // Espera del semaforo
  sem_wait(&s);
  int t = 0;
  for (int i = 0; i < 100; i++) { // Haz cosas
    t--;
  }
  dato = t;
  // da paso el semaforo;
  sem_post(&s);
  pthread_exit(0);
}
 /* Este hilo suma */
void *suma() {
  int t = 0;
  for (int i = 0; i < 50; i++) { // Haz cosas
    t++;
  }
  dato = t;
  pthread_exit(0);
}