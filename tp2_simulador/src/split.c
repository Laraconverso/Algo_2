#include "split.h"
#include <stdlib.h>
#include <string.h>

/*
* Cuenta la cantidad de veces que el separador se encuentra dentro del string.
*/
size_t contador_ocurrencias(const char* string, char separador){
    size_t contador = 0;
    size_t i = 0;
    while(string[i] != '\0'){
        if(string[i]== separador){
            contador++;
        }
        i++;
    }
    return contador;
}

/*
* Busca el separador pasado por parametro dentro de un string.
*/
size_t buscar_prox_separador(const char* string, char separador){
    size_t i = 0;
    while(string[i] != '\0' && string[i] != separador){
       i++;
    }
    return i;
}

/*
* Duplica el string y reservando memoria para este nuevo.
*/
char* duplicar_string(const char* string, size_t cantidad){
    char* nuevo_string = malloc((cantidad+1)*sizeof(char));
    if(!nuevo_string){
        return NULL;
    }
    for(size_t i= 0 ; i<cantidad; i++){
        nuevo_string[i]= string[i];
    }
    nuevo_string[cantidad] = 0;
    return nuevo_string;
}

/*
* Libera la memoria resevada para el vector
*/
void liberar_vector(char** vector){
    while(*vector){
        free(*vector);
        vector++;
    }
}

//PRE & POST en split.h//
char** split(const char* string, char separador){
    if(!string)
        return NULL;

    size_t ocurrencias = contador_ocurrencias(string, separador);
    size_t cantidad_substrings = ocurrencias+1;

    char** vector = calloc(cantidad_substrings+1, sizeof(void*));
    if(!vector)
        return NULL;


    for(size_t n = 0; n < cantidad_substrings; n++){
        size_t tamanio_substring = buscar_prox_separador(string, separador);
        char *substring = duplicar_string(string, tamanio_substring);

        if(!substring){
            liberar_vector(vector);
            return NULL;
        }

        vector[n] = substring;
        string += tamanio_substring+1;
    }

    return vector;
}