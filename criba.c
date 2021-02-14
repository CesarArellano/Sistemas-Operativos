#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>

// Definiendo estructuras
typedef struct Arguments{
  int* array;
  int length;
}Arguments;

void sortThreads(int arrayNumbers[], int length);
void cribaThreads(int arrayNumbers[], int length);
void* bubbleSort(void *args);
void mix(int array[], int first, int middle, int last);
void* criba(void *arguments);
void print(int arrayNumbers[], int length);

// Variables globales
sem_t s;

int main() {
  clock_t clockStart, clockStop;
  double finalTime;
  int *arr;
  int option, numElements, numThreads; // Número hasta el que se quiere buscar.
  
  do {
    system("clear");
    printf("Menú:\n");
    printf("1.- Ordenamiento.\n");
    printf("2.- Criba.\n");
    printf("3.- Salir del programa.\n\n");
    printf("Seleccione una opción: ");
    scanf(" %d", &option);
    system("clear");

    switch(option) {
      case 1:
        printf("Ordenamiento.\n");        
        printf("¿Cuántos elementos quieres ocupar?\n");
        scanf(" %d", &numElements);

        arr = (int*)malloc(sizeof(int) * numElements);
        for(int i = 0; i < numElements; i++) {
            arr[i] = rand() % numElements;
        }
        clockStart = clock();
        sortThreads(arr, numElements);
        print(arr, numElements);
        clockStop = clock();
        finalTime = (double)(clockStop - clockStart) / CLOCKS_PER_SEC;
        printf("La tarea con %d tomó: %f segundos\n", numElements, finalTime);  
      break;

      case 2:
        printf("Criba.\n");
        printf("¿Cuántos elementos quieres buscar?\n");
        scanf(" %d", &numElements);
        arr = (int*)malloc(sizeof(int) * numElements);
        clockStart = clock();
        cribaThreads(arr,numElements);
        clockStop = clock();
        finalTime = (double)(clockStop - clockStart) / CLOCKS_PER_SEC;
        printf("La tarea con %d tomó: %f segundos\n", numElements, finalTime);
      break;

      case 3:
        printf("Saliendo del programa...\n");
        free(arr);
        return 0;
      break;
      
      default:
        printf("Error, opción inválida.\n");
      break;
    }

    printf("Presione Enter para regresar...\n");
    __fpurge(stdin);
    getchar();
  } while (option != 3);
  return 0;
}

void sortThreads(int arrayNumbers[], int length) {
  pthread_t tid1, tid2; // Identificadores de hilos
  pthread_attr_t attr;
  sem_init(&s, 1, 0); // Se inicializa el semáforo
  int halfLength = length / 2; // Mitad 1 hilo, mitad el otro.

  // Argumentos para el hilo 1
  Arguments threadOneArgs;
  threadOneArgs.array = arrayNumbers;
  threadOneArgs.length = halfLength;

  // Argumentos  para el hilo 2
  Arguments threadTwoArgs;
  threadTwoArgs.array = &arrayNumbers[halfLength]; // Ahora el primer elemento del arreglo será el elemento que se encuentra a la mitad del arreglo original
  threadTwoArgs.length = halfLength; // Por lo tanto, el tamaño del arreglo será la mitad

   // Inicializa los atributos del hilo al que se referencia attr
  pthread_attr_init(&attr);
  // Creo el primer hilo
  pthread_create(&tid1, &attr, bubbleSort, &threadOneArgs);
  pthread_create(&tid2, &attr, bubbleSort, &threadTwoArgs);
  // Espero a que los hilos se terminen de ejecutar
  pthread_join(tid1, NULL);  
  pthread_join(tid2, NULL);
}

// Ordenar arrays mediante hilos.
void* bubbleSort(void *arguments) {
  Arguments* argumentsArray = (Arguments*)arguments;
  int *array, length;
  int temp;

  array = argumentsArray->array;
  length = argumentsArray->length;

  for (int i = 0; i < length; i++) {
    for (int j = 0; j < length - i - 1; j++) {
      if (array[j + 1] < array[j]) {
        temp = array[j];
        array[j] = array[j + 1];
        array[j + 1] = temp;
      }
    }
  }
}

// Mezcla los números ya ordenados por los hilos asignados.

void mix(int array[], int first, int middle, int last) {
  int t = 0, s = first;
  int half = middle - 1;
  int size = last - first + 1;
  int *aux = malloc(sizeof(int) * (size));

  while ((first <= half) && (middle <= last)){
    if (array[first] <= array[middle]){
      aux[t] = array[first];
      first++;
      t++;
    }else{
      aux[t] = array[middle];
      middle++;
      t++;
    }
  }

  while (first <= half){
    aux[t++] = array[first++];
  }

  while (middle <= last){
    aux[t++] = array[middle++];
  }

  for (int i = 0; i <= size; i++){
    array[s + i] = aux[i];
  }
}

void cribaThreads(int arrayNumbers[], int length) {
  pthread_t tid1, tid2; // Identificadores de hilos
  pthread_attr_t attr;
  sem_init(&s, 1, 0); // Se inicializa el semáforo
  int halfLength = length-1; // Mitad 1 hilo, mitad el otro.

  // Argumentos para el hilo 1
  Arguments threadOneArgs;
  threadOneArgs.array = arrayNumbers;
  threadOneArgs.length = halfLength;

   // Inicializa los atributos del hilo al que se referencia attr
  pthread_attr_init(&attr);
  // Creo el primer hilo
  pthread_create(&tid1, &attr, criba, &threadOneArgs);
  // Espero a que los hilos se terminen de ejecutar
  pthread_join(tid1, NULL);
  
  // Argumentos  para el hilo 2
  Arguments threadTwoArgs;
  threadTwoArgs.array = &arrayNumbers[halfLength]; // Ahora el primer elemento del arreglo será el elemento que se encuentra a la mitad del arreglo original
  threadTwoArgs.length = halfLength; // Por lo tanto, el tamaño del arreglo será la mitad
  pthread_create(&tid2, &attr, criba, &threadTwoArgs);
  pthread_join(tid2, NULL);
}

void* criba(void *arguments) {
  Arguments* argumentsArray = (Arguments*)arguments;
  int *primos, length;
  int i, x = 0, p;
  primos = argumentsArray->array;
  length = argumentsArray->length;
  int nums[length];

  for (i = 2; i <= length; i++) {
    if (nums[i] != 1 || i == 2) {
      primos[x] = i;
      printf("El número %d es primo\n", i);
      for (p = 2; (p*i) <= length; p++) {
        nums[(p*i)] = 1; // Múltiplos no son primos, se asignan en 1;
      }
      x++;
    }
  }
  printf("Hay %d números primos\n",x);
}

void print(int arrayNumbers[], int length) {
  for(int i = 0; i < length; i++) {
        printf("%d ", arrayNumbers[i]);
  }
  printf("\n");
}

/*
void multi_process_criba(int final_number, int numberOfThreads){
    pthread_t* threads = (pthread_t*)malloc(sizeof(pthread_t) * numberOfThreads);
    pthread_attr_t attr;
    cribaArgs* args = getArguments(final_number, numberOfThreads);
    int* prime_numbers = (int*)malloc(sizeof(int) * final_number);
    int x = 0;

    pthread_attr_init(&attr);

    for(int i = 0; i < numberOfThreads; i++){
        args[i].prime_numbers = prime_numbers;
        args[i].x = &x;
    }

    for(int i = 0; i < numberOfThreads; i++){
        pthread_create(&threads[i], &attr, criba, &args[i]);
    }

    for(int i = 0; i < numberOfThreads; i++){
        pthread_join(threads[i], NULL);
    }

    printf("Hay %d números primos \n", x);
}

cribaArgs* getArguments(int final_number, int numberOfThreads){
    cribaArgs* args = (cribaArgs*)malloc(sizeof(cribaArgs) * numberOfThreads);
    int init = 2;
    int next_step = final_number / numberOfThreads;

    for(int i = 0; i < numberOfThreads; i++){
        args[i].initial_number = init;
        args[i].final_number = init + next_step;

        init = next_step + 1;
    }
    return args;
}


void* criba(void* args){
    cribaArgs* arguments= (cribaArgs*)args;
    int n = arguments->final_number;
    int i = arguments->initial_number;
    int p, nums[n];


    for (i; i <= n; i++){
        if (nums[i] != 1 || i == 2){
            arguments->prime_numbers[ *arguments->x] = i;
            printf("El número %d es primo.\n", i);
            for (p = 2; (p * i) <= n; p++){
                nums[(p * i)] = 1;
            }
            *arguments->x = *arguments->x + 1;
        }
    }
    
}

void print(int arrayNumbers[], int length) {
  for(int i = 0; i < length; i++) {
        printf("%d ", arrayNumbers[i]);
  }
  printf("\n");
}

void* criba(void* args);
void multi_process_criba(int final_number, int numberOfThreads);
cribaArgs *getArguments(int final_number, int numberOfThreads);
*/