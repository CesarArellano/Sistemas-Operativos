#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <math.h>

void print(int array[], int length);
void bubbleSort(int array[], int length);
void mix(int array[], int first, int middle, int last);

int main(void) {
    int *array, sizeElements = 2500, pid;
    clock_t clockStart, clockStop;
    double finalTime;

    for(int k = 0; k < 3; k++) {

        // Mapas en memoria virtual que se situa en el proceso actual.
        array = mmap(NULL, // void *direcció
                    sizeElements * sizeof(int), // size_t tamaño
                    PROT_READ | PROT_WRITE, // int protección
                    MAP_SHARED | MAP_ANONYMOUS, // int banderas
                    -1, 0 // int filedescriptor, off_t desfasamiento
                    );

        for(int i = 0; i < sizeElements; i++) {
            array[i] = rand() % sizeElements;
        }

        clockStart = clock();
        pid = fork();

        if(pid > 0) { // Proceso Hijo
            printf("Proceso Hijo: ");
            bubbleSort(array, sizeElements / 2);
            wait(&pid);
            mix(array, 0, sizeElements/2, sizeElements-1);
        } else { // Proceso Padre
            printf("Proceso Padre: ");
            bubbleSort(&array[sizeElements / 2], sizeElements / 2);
        }

        clockStop = clock();
        finalTime = (double)(clockStop - clockStart) / CLOCKS_PER_SEC;
        finalTime = fabs(finalTime);
        printf("La tarea con %d tomó: %f segundos\n", sizeElements, finalTime);
        sizeElements *=  5;
    }
    return 0;
}

void bubbleSort(int array[], int length) {
    int i, j, temp;
    for (i = 0; i < length; i++) {
        for (j = 0; j < length - i - 1; j++) {
            if (array[j + 1] < array[j]) {
                temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;
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