#include "split.h"
#include <stdlib.h>
#include <string.h>

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

size_t buscar_prox_separador(const char* string, char separador){
    size_t i = 0;
    while(string[i] != '\0' && string[i] != separador){
       i++;
    }
    return i;
}

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

void liberar_todo(char** vector){
    while(*vector){
        free(*vector);
        vector++;
    }
}

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
            liberar_todo(vector);
            return NULL;
        }

        vector[n] = substring;
        string += tamanio_substring+1;
    }
    return vector;
}