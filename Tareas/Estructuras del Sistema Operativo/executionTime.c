/*
*@file executionTime.c
*@brief Este programa calcula el tiempo que tarda en ejecutar una suma de dos números.
*@author César Arellano
*@date 23/01/2021
*/

#include <stdio.h>
#include <time.h> // Librería usada para utilizar clock_t

int add(int num1, int num2);

int main(void) {
  // Declaramos variables para determinar el tiempo de ejecución, clock_t.
	clock_t clkStart, clkEnd; 
	double executionTime = 0.0;

	// Se instancia el inicio del reloj
	clkStart = clock();
  printf("La suma es: %d\n",add(2,4));
	// Se instancia el fin del reloj	
	clkEnd = clock();
	// Se calcula el tiempo de ejecución.	
	executionTime = (double) (clkEnd-clkStart) / CLOCKS_PER_SEC;
  printf("Tiempo de ejecución: %lfs\n", executionTime);
	return 0;
}

int add(int num1, int num2) {
  return num1 + num2;
}