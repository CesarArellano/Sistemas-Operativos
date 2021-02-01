#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <time.h>

void imp(int arr[], int len);
void bubbleSort(int m[], int elementos);
void mezcla(int arr[], int ini, int med, int fin);

#define tm 1000
int main()
{
  //int *t = malloc(sizeof(int));
  int *arr;
  arr = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  for(int i = 0; i < tm; i++) {
    arr[i] = rand()%tm; // Entre 0 y 99;
  }
  long ini = clock();

  int pid = fork();
  if (pid > 0) {
    printf("hello\nfather here \n Ordene: ");
    bubbleSort(arr,tm/2);
    wait(&pid);
    mezcla(arr,0,tm/2,tm-1);
    long fin = clock();
    double t = fin-ini;
    t = t/((double)CLOCKS_PER_SEC);
    printf("Se tardo %f segundos \n", t);
  } else {
    printf("hello\nchild here \n Ordene: ");
    bubbleSort(&arr[50],50);
  }
  return 0;
}

void bubbleSort(int m[], int elementos)
{
  int aux, i, s=1;

  while(s && (--elementos>0))
  {
    s=0;
    for(i=1; i<=elementos; i++)
      if(m[i-1]>m[i])
      {
        aux=m[i-1];
        m[i-1]=m[i];
        m[i]=aux;
        s=1;
      }
  }
}

void imp(int arr[], int len) {
  int i;
  for(i=0; i<len; i++) {
    printf("%d\n", arr[i]);
  }
}

void mezcla(int arr[], int ini, int med, int fin) {

  int *aux = malloc(sizeof(int) * (fin - ini + 1));
  int finIni = med - 1;
  int s = ini;
  int tam = fin-ini+1;
  int t = 0;

  while ((ini <= finIni) && (med <= fin)) {
    if (arr[ini] <= arr[med]) {
      aux[t] = arr[ini];
      ini++;
      t++;
    } else {
      aux[t] = arr[med];
      med++;
      t++;
    }
  }

  while (ini <= finIni) {
    aux[t++] = arr[ini++];
  }

  while (med <= fin) {
    aux[t++] = arr[med++];
  }

  for (int i = 0; i <= (tam); i++) {
      arr[s+i] = aux[i];
  }
}