#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <time.h>
#include <pthread.h>

int dato = 0;
int *arr;

void imp(int arr[], int len);
void *bubbleSort(void *ar);
void mezcla(int arr[], int ini, int med, int fin);

struct args {
  int *arr;
  int l;
};

int main(int argc, char *argv[]) {
  pthread_t tid1, tid2;
  pthread_attr_t attr;
  
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
}