/*
*@file directoryReader.c
*@brief Este programa lee la estructura básica del directorio actual.
*@author César Arellano
*@date 23/01/2021
*/

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

int main(void) {
	// Declaración de las variables para obtener los datos del directorio
	DIR *currentDirectory;
	struct dirent *directory;
	
	// Variable para abrir / leer archivos del directorio.
	FILE *currentFile;

	// Abrimos el directorio actual, para su posterior lectura.
	currentDirectory = opendir(".");

  // Validamos que exista el directorio.
  if (currentDirectory == NULL) {
    printf("No se pudo abrir el directorio especificado.\n");
    exit(0);
  }

	printf("Estructura del directorio actual:\n");
	if (currentDirectory) {
		// Leemos el directorio actual para imprimir su contenido.
		while ((directory = readdir(currentDirectory)) != NULL) {
			printf("Nombre: %s\n", directory->d_name);
			// Determinamos el tipo de fichero (archivo).
			if (directory->d_type == 8) {
				//Abrimos archivo para calcular su tamaño en kilobytes.
				currentFile = fopen(directory->d_name,"r");
				if (currentFile != NULL) {
					fseek(currentFile, 0L, SEEK_END);
					printf("%s ocupa %ld kB\n", directory->d_name, ftell(currentFile)/1000);
					fclose(currentFile);
				}
				printf("Tipo: Archivo\n");
				printf("Número serial del archivo: 8");
			}
			else if (directory->d_type == 4) { // Determinamos el tipo de fichero (carpeta).
				printf("Tipo: Carpeta\n");
				printf("Número serial del archivo: 4");
			}
			else { // Determinamos el tipo de fichero (otro).
				printf("Número serial del archivo: %u", directory->d_type);
			}
			printf("\n\n");
		}
		closedir(currentDirectory); // Cerramos el directorio actual.
	}
	return 0;
}