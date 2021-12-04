#include "hospital.h"
#include "split.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct _hospital_pkm_t{
    size_t cantidad_pokemon;
    pokemon_t *vector_pokemones;
    size_t cantidad_entrenadores;
};

struct _pkm_t{
    char *nombre;
    size_t nivel;
};

//PRE & POST: Definidas en hospital.h
hospital_t *hospital_crear(){
    hospital_t *hospital = calloc(1, sizeof(hospital_t));
    if (!hospital){
        return NULL;
    }
    hospital->cantidad_entrenadores = 0;
    hospital->cantidad_pokemon = 0;
    hospital->vector_pokemones = NULL;
    return hospital;
}

/*PRE:El archivo debe estar abierto.
*POST: Retorna el contenido de una linea de un archivo utilizando fgets e inicializa la memoria para dicha lina. 
*/
char *leer_linea(FILE *nombre_archivo){
    if(!nombre_archivo){
        return NULL;
    }
    size_t bytes = 0;
    size_t tamanio_buffer = 1024;
    char *buffer = malloc(sizeof(char) * tamanio_buffer);
    if (!buffer){
        return NULL;
    }
    while (fgets(buffer + bytes, (int)(tamanio_buffer - bytes), nombre_archivo)){
        size_t leido = strlen(buffer + bytes);
        if (leido > 0 && *(buffer + bytes + leido - 1) == '\n'){
            *(buffer + bytes + leido - 1) = 0;
            return buffer;
        } else{
            char *aux = realloc(buffer, tamanio_buffer * 2);
            if (!aux){
                free(buffer);
                return NULL;
            }
            buffer = aux;
            tamanio_buffer *= 2;
        }
        bytes += leido;
    }   
    if(bytes == 0){
        free(buffer);
        return NULL;
    }
return buffer;
}

/*PRE: El vector que recibe no debe ser nulo.
*POST: Retorna el largo de un vector. 
*/
size_t largo_vector(void *vector){
    void **puntero_aux = (void **)vector;
    size_t posicion = 0;
    while (puntero_aux[posicion] != NULL){
        posicion++;
    }
    return posicion;
}

/*PRE: El vector que recibe no debe ser nulo.
*POST: Ordena los pokemones alfabeticamente.
*/
void ordenar_pokemones(pokemon_t* vector_pokemones, size_t cantidad_pokemones){
    pokemon_t poke_aux;
    size_t minimo;
    for(size_t i= 0; i < (cantidad_pokemones); i++){
        minimo = i;
        for(size_t j = i+1; j < cantidad_pokemones; j++){
            if(strcmp(vector_pokemones[minimo].nombre, vector_pokemones[j].nombre)> 0){
                minimo = j;
            }
        }
        poke_aux = vector_pokemones[i];
        vector_pokemones[i] = vector_pokemones[minimo];
        vector_pokemones[minimo] = poke_aux;
    }
}

/*PRE: El vector que recibe no debe ser nulo.
*POST: Libera cada elmento que se encuentre dentro del vector pasado por parametro.
*/
void liberar_vectores(void *vtr){
    void **vtr_aux = (void **)vtr;
    for (size_t i = 0; i < 2; i++){
        free(vtr_aux[i]);
    }
    free(vtr);
}

//PRE & POST: Definidas en hospital.h
bool hospital_leer_archivo(hospital_t *hospital, const char *nombre_archivo){
    if ((!hospital) || (!nombre_archivo)){
        return false;
    }
    FILE *archivo = fopen(nombre_archivo, "r");
    if (!archivo){
        perror("No se pudo abrir el archivo");
        return false;
    }

    char *linea = leer_linea(archivo);
    size_t contador_pokemones = hospital->cantidad_pokemon;
    while(linea){
        if(!linea){
            free(linea);
            fclose(archivo);
            return false;
        }
        char **vector_aux = split(linea, ';');
        if(!vector_aux){
            free(linea);
            liberar_vectores(vector_aux);
            fclose(archivo);
            return false;
        }
        size_t largo_vector_aux = largo_vector(vector_aux);
        size_t cantidad_pokemones = ((largo_vector_aux / 2) - 1) + contador_pokemones;
        pokemon_t *vector_pokemones_aux = realloc(hospital->vector_pokemones, (cantidad_pokemones + contador_pokemones) * sizeof(pokemon_t));
        if (!vector_pokemones_aux){
            free(linea);
            liberar_vectores(vector_aux);
            free(vector_pokemones_aux);
            fclose(archivo);
            return false;
        }
        if (largo_vector_aux <= 1){
            free(linea);
            free(vector_aux[0]);
            free(vector_aux[1]);
            free(vector_aux);
            free(vector_pokemones_aux);
            fclose(archivo);
            return false;
        } else if (largo_vector_aux % 2 == 0){
            for (size_t i = 2; i < largo_vector_aux; i += 2){
                pokemon_t pokemon;
                pokemon.nombre = (char*)vector_aux[i];
                pokemon.nivel = (size_t)vector_aux[i + 1];
                vector_pokemones_aux[contador_pokemones] = pokemon;
                contador_pokemones++;
            }
        }
        hospital->vector_pokemones = vector_pokemones_aux;
        free(linea);
        liberar_vectores(vector_aux);
        hospital->cantidad_entrenadores++;
        linea = leer_linea(archivo);
    }
    hospital->cantidad_pokemon = contador_pokemones;
    fclose(archivo);
    ordenar_pokemones(hospital->vector_pokemones, hospital->cantidad_pokemon);
    return true;
}

//PRE & POST: Definidas en hospital.h
size_t hospital_cantidad_pokemon(hospital_t *hospital)
{
    if (hospital == NULL)
    {
        return 0;
    }
    return hospital->cantidad_pokemon;
}

//PRE & POST: Definidas en hospital.h
size_t hospital_cantidad_entrenadores(hospital_t *hospital)
{
    if (hospital == NULL)
    {
        return 0;
    }
    return hospital->cantidad_entrenadores;
}

//PRE & POST: Definidas en hospital.h
size_t hospital_a_cada_pokemon(hospital_t *hospital, bool (*funcion)(pokemon_t *p)){
    if (hospital == NULL || funcion == NULL || hospital->cantidad_pokemon == 0){
        return 0;
    }
    pokemon_t *pokemones = hospital->vector_pokemones;
    size_t contador_pokemones_funcion = 0;
    for (size_t i = 0; i < hospital->cantidad_pokemon; i++){
        if (funcion(&pokemones[i]) == true){
            contador_pokemones_funcion++;
        }
        else{
            contador_pokemones_funcion++;
            return contador_pokemones_funcion;
        }
    }
    return contador_pokemones_funcion;
}

//PRE & POST: Definidas en hospital.h
void hospital_destruir(hospital_t *hospital){
    if (hospital == NULL){
        return;
    }
    if (hospital->cantidad_pokemon > 0){
        for (size_t i = 0; i < hospital->cantidad_pokemon; i++){
            free(hospital->vector_pokemones[i].nombre);
            free((char *)hospital->vector_pokemones[i].nivel);
        }
        free(hospital->vector_pokemones);
        free(hospital);
    }
    else{
        free(hospital->vector_pokemones);
        free(hospital);
    }
}

//PRE & POST: Definidas en hospital.h
size_t pokemon_nivel(pokemon_t *pokemon){
    if (pokemon == NULL){
        return 0;
    }
    return pokemon->nivel;
}

//PRE & POST: Definidas en hospital.h
const char *pokemon_nombre(pokemon_t *pokemon){
    if (pokemon == NULL){
        return NULL;
    }
    return pokemon->nombre;
}
