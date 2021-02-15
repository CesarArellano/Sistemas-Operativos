  
/*
*@file practicaUno.c
*@brief Este programa pretende explicar el funcionamiento de los hilos y los semáforos para optimizar tiempos de ejecución de tareas específicas (ordenamiento y criba).
*@author César Arellano
*@date 14/02/2021
*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>

// Definiendo estructura de Ordenamiento
typedef struct SortArguments{
  int* arraySort;
  int lengthSort;
}SortArguments;

// Definiendo estructura de Criba
typedef struct CribatArguments{
    int initialNumber;
    int finalNumber;
    int* resultNumbers;
    int* x;
    int* nums;
    int* totalNumbers;
}CribaArguments;

// Funciones prototipo para ordenamiento
void sortThreads(int arrayNumbers[], int length);
void* bubbleSort(void *args);
void mix(int array[], int first, int middle, int last);

// Funciones prototipo para la criba
void cribaThreads(int finalNumber, int numberThreads);
CribaArguments *getArguments(int finalNumber, int numberThreads);
void* criba(void *sortArguments);

// Función para imprimir resultado del arreglo.
void print(int arrayNumbers[], int length);

// Variable global, para el semáforo de la criba
sem_t mutex;

int main() {
  clock_t clockStart, clockStop;
  double finalTime;
  int *arraySort;
  int option, numElements, numThreads; // Número hasta el que se quiere buscar.
  int validThreads; // con esta variable validaremos el número de hilos que puede ingresar el usuario.
  
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

        arraySort = (int*)malloc(sizeof(int) * numElements);
        for(int i = 0; i < numElements; i++) {
            arraySort[i] = rand() % numElements;
        }
        clockStart = clock();
        sortThreads(arraySort, numElements);
        print(arraySort, numElements);
        clockStop = clock();
        finalTime = (double)(clockStop - clockStart) / CLOCKS_PER_SEC;
        printf("La tarea con %d tomó: %f segundos\n", numElements, finalTime);        
        free(arraySort); // Limpiamos memoria del arreglo de ordenamiento.
      break;

      case 2:
        printf("Criba.\n");
        printf("¿Cuántos elementos quieres buscar?\n");
        scanf(" %d", &numElements);
        printf("¿Cuántos hilos quieres usar? (Menor al número de elementos)\n");
        scanf(" %d", &numThreads);
        validThreads = (numElements / 2) - 1;
        if((numThreads > numElements) || (validThreads < numThreads)) {
          printf("Error, el número de hilos supera los elementos a buscar\n");
        } else {
          clockStart = clock();
          cribaThreads(numElements, numThreads);
          clockStop = clock();
          finalTime = (double)(clockStop - clockStart) / CLOCKS_PER_SEC;
          printf("La tarea con %d tomó: %f segundos\n", numElements, finalTime);
        } 
      break;

      case 3:
        printf("Saliendo del programa...\n");
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
  int halfLength = length / 2; // Mitad 1 hilo, mitad el otro.

  // Argumentos para el hilo 1
  SortArguments threadOneArgs;
  threadOneArgs.arraySort = arrayNumbers;
  threadOneArgs.lengthSort = halfLength;

  // Argumentos  para el hilo 2
  SortArguments threadTwoArgs;
  threadTwoArgs.arraySort = &arrayNumbers[halfLength]; // Ahora el primer elemento del arreglo será el elemento que se encuentra a la mitad del arreglo original
  threadTwoArgs.lengthSort = halfLength; // Por lo tanto, el tamaño del arreglo será la mitad

   // Inicializa los atributos del hilo al que se referencia attr
  pthread_attr_init(&attr);
  // Creo el primer hilo
  pthread_create(&tid1, &attr, bubbleSort, &threadOneArgs);
  pthread_create(&tid2, &attr, bubbleSort, &threadTwoArgs);
  // Espero a que los hilos se terminen de ejecutar
  pthread_join(tid1, NULL);  
  pthread_join(tid2, NULL);
  mix(arrayNumbers,0, halfLength, length-1); // Hacemos la mezcla para que divida el trabajo para ambos hilos.
}

// Ordenar arrays mediante hilos.
void* bubbleSort(void *sortArguments) {
  SortArguments* sortArgumentsArray = (SortArguments*) sortArguments;
  int *array, length;
  int temp;

  array = sortArgumentsArray->arraySort;
  length = sortArgumentsArray->lengthSort;

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

// Generamos el número de hilos para el proceso de la criba.
void cribaThreads(int final_number, int numberOfThreads){
  pthread_t* threads = (pthread_t*)malloc(sizeof(pthread_t) * numberOfThreads);
  pthread_attr_t attr;
  CribaArguments* args = getArguments(final_number, numberOfThreads);
  int* prime_numbers = (int*)malloc(sizeof(int) * final_number);
  int* nums = (int*)malloc(sizeof(int) * final_number);
  int x = 0;

  sem_init(&mutex, 0, 1);

  pthread_attr_init(&attr);

  for(int i = 0; i < numberOfThreads; i++){
    args[i].resultNumbers = prime_numbers;
    args[i].x = &x;
    args[i].nums = nums;
    args[i].totalNumbers = &final_number;
  }

  for(int i = 0; i < numberOfThreads; i++){
    pthread_create(&threads[i], &attr, criba, &args[i]);    
    pthread_join(threads[i], NULL);
  }
  sem_destroy(&mutex);
  printf("Hay %d números primos \n", x);

  // Limpiando memoria de variables, creadas dinámicamente con la función malloc.
  free(threads);
  free(prime_numbers);
  free(nums);
  free(args);
}

// Obtenemos el número de argumentos necesarios para realizar las operaciones.
CribaArguments *getArguments(int finalNumber, int numberThreads) {
  CribaArguments* args = (CribaArguments*)malloc(sizeof(CribaArguments) * numberThreads);
  int init = 2; // Iniciamos en dos porque es nuestro primer número primo.
  int next_step = (finalNumber / numberThreads);

  for(int i = 0; i < numberThreads; i++){
    if(i == 0){
      args[i].initialNumber = 2;
      args[i].finalNumber = next_step;
    }else{
      args[i].initialNumber = args[i-1].finalNumber;
      args[i].finalNumber = args[i].initialNumber + next_step;
    }
      
  }
  return args;
}

// Proceso de la criba, con este obtenemos la impresión de los números primos
void* criba(void* args){
  CribaArguments* arguments = (CribaArguments*)args;
  int n = arguments->finalNumber;
  int i = arguments->initialNumber;
  int p;

  sem_wait(&mutex);
  for (i; i <= n; i++){
    if (arguments->nums[i] != 1 || i == 2){
      arguments->resultNumbers[ *arguments->x] = i;
      printf("El número %d es primo.\n", i);
      for (p = 2; (p * i) <= *arguments->totalNumbers; p++){
          arguments->nums[(p * i)] = 1;
      }
      *arguments->x = *arguments->x + 1;

    }
  }
  sem_post(&mutex);
  pthread_exit(0);
}

// Imprimimos el resultado del array.
void print(int arrayNumbers[], int length) {
  for(int i = 0; i < length; i++) {
        printf("%d ", arrayNumbers[i]);
  }
  printf("\n");
}
