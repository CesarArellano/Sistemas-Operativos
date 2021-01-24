/*
*@file directoryReader2.c
*@brief Este programa lee la estructura avanzada del directorio actual.
*@author César Arellano
*@date 23/01/2021
*/

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

int main(void) {
	// Declaración de las variables para obtener los datos del directorio
	DIR *currentDirectory;
	struct dirent *directory;
	
	// Cadena para ejecutar el comando stat "nombreFichero", el cual despliega información avanzada del fichero.
	char command[50];

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
      strcpy(command, "stat ");
      strcat(command, directory->d_name); // resultado de la cadena: "stat nombreFichero"
      system(command); // Ejecuta comando en terminal.
      printf("\n");
		}
		closedir(currentDirectory); // Cerramos el directorio.
	}
	return 0;
}