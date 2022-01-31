#include "hospital.h"
#include "split.h"
#include "hospital_structs.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//Compara los pokemones recibidos por parametro segun su nivel.
int comparador_pokemones_nivel(void *pkm1, void *pkm2){
    if (!pkm1 || !pkm2) return 0;
    pokemon_t *uno = (pokemon_t *)pkm1;
    pokemon_t *dos = (pokemon_t *)pkm2;
    return (int)(uno->nivel - dos->nivel);
}

//Compara los pokemones recibidos por parametro segun su nivel.
int comparador_pokemones_nombre(void *pkm1, void *pkm2){
    if (!pkm1 || !pkm2) return 0;
    char *uno = ((pokemon_t *)pkm1)->nombre;
    char *dos = ((pokemon_t *)pkm2)->nombre;
    return strcmp(uno, dos);
}

/*PRE: Recibe un puntero a un pokemon
* POST: Libera la memoria reservada del pokemon
*/
void pokemones_destruir(void *p_pokemon){
    if (!p_pokemon) return;
    pokemon_t *pokemon = (pokemon_t *)p_pokemon;
    free(pokemon->nombre);
    free(pokemon);
}

/*PRE:Recibe un puntero a un entrenador valido
*POST:libera la memoria del entrenados y la informacion de este.
*/
void entrenadores_destruir(void *p_entrenador){
    if (!p_entrenador) return;
    entrenador_t *entrenador = (entrenador_t *)p_entrenador;
    lista_destruir(entrenador->pokemones);
    free(entrenador->nombre);
    free(entrenador);
}

//PRE & POST: Definidas en hospital.h//
hospital_t *hospital_crear(){
    hospital_t *hospital = calloc(1, sizeof(hospital_t));
    if (!hospital) return NULL;
    hospital->cola_entrenadores_sala_espera = cola_crear(entrenadores_destruir);
    if (!hospital->cola_entrenadores_sala_espera){
        free(hospital);
        return NULL;
    }
    hospital->lista_entrenadores_atendidos = lista_crear(entrenadores_destruir);
    if (!hospital->lista_entrenadores_atendidos){
        cola_destruir(hospital->cola_entrenadores_sala_espera);
        free(hospital);
        return NULL;
    }
    hospital->heap_pokeones_sala_espera = heap_crear(comparador_pokemones_nivel, pokemones_destruir);
    if (!hospital->heap_pokeones_sala_espera){
        cola_destruir(hospital->cola_entrenadores_sala_espera);
        lista_destruir(hospital->lista_entrenadores_atendidos);
        free(hospital);
        return NULL;
    }
    hospital->lista_pokemones_atendidos = lista_crear(pokemones_destruir);
    if (!hospital->lista_pokemones_atendidos){
        cola_destruir(hospital->cola_entrenadores_sala_espera);
        lista_destruir(hospital->lista_entrenadores_atendidos);
        heap_destruir(hospital->heap_pokeones_sala_espera);
        free(hospital);
        return NULL;
    }
    hospital->pokemon_atendiendose = NULL;
    return hospital;
}

/*PRE:Recibe un hospital no nulo.
*POST:Libera la memoria reservada de los elementos dentro del hospital
*/
void hospital_liberar(hospital_t *hospital){
    if (!hospital) return;
    cola_destruir(hospital->cola_entrenadores_sala_espera);
    lista_destruir(hospital->lista_entrenadores_atendidos);
    heap_destruir(hospital->heap_pokeones_sala_espera);
    lista_destruir(hospital->lista_pokemones_atendidos);
    pokemones_destruir(hospital->pokemon_atendiendose);
}

/*PRE:El archivo debe estar abierto.
*POST: Retorna el contenido de una linea de un archivo utilizando fgets e inicializa la memoria para dicha lina. 
*/
char *leer_linea(FILE *nombre_archivo){
    if (!nombre_archivo)return NULL;
    size_t bytes = 0;
    size_t tamanio_buffer = 1024;
    char *buffer = malloc(sizeof(char) * tamanio_buffer);
    if (!buffer)return NULL;
    while (fgets(buffer + bytes, (int)(tamanio_buffer - bytes), nombre_archivo)){
        size_t leido = strlen(buffer + bytes);
        if (leido > 0 && *(buffer + bytes + leido - 1) == '\n'){
            *(buffer + bytes + leido - 1) = 0;
            return buffer;
        }
        else{
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
    if (bytes == 0){
        free(buffer);
        return NULL;
    }
    return buffer;
}

/*PRE:Recibe un hospital no nulo, una lista de pokemones, un puntero a la linea leida y un entrenador (se debe chequear antes).
*POST:Inicializa el pokemon con su nombre y entrenador correspondiente a partir de la linea leida.
*/
bool cargo_info_pokemon(hospital_t *hospital, lista_t *pokemones, char **linea, entrenador_t *entreador){
    pokemon_t *pokemon_a_cargar = calloc(1, sizeof(pokemon_t));
    if (!pokemon_a_cargar) return false;
    pokemon_a_cargar->nombre = malloc((strlen(linea[0]) + 1) * sizeof(char));
    if (!pokemon_a_cargar->nombre){
        free(pokemon_a_cargar);
        return false;
    }
    strcpy(pokemon_a_cargar->nombre, linea[0]);
    pokemon_a_cargar->nivel = (size_t)atoi(linea[1]);
    pokemon_a_cargar->entrenador = entreador;
    pokemon_a_cargar->atendido = false;

    lista_t *auxiliar = lista_insertar(pokemones, pokemon_a_cargar);
    if (!auxiliar){
        free(pokemon_a_cargar->nombre);
        free(pokemon_a_cargar);
        return false;
    }
    return true;
}

/*PRE:Recibe un hospital no nulo( se debe chequear antes), una cola de entrenadores y un puntero a la linea leida (se debe chequear antes).
*POST: Inicializa el entrenador con su pokemones y nombre correspondientes a partir de la linea leida.
*/
bool cargo_info_entrenador(hospital_t *hospital, cola_t *entreadores, char **linea){
    entrenador_t *entrenador_a_cargar = calloc(1, sizeof(entrenador_t));
    if (!entrenador_a_cargar)
        return false;
    entrenador_a_cargar->id = atoi(linea[0]);
    entrenador_a_cargar->nombre = calloc((strlen(linea[1]) + 1), sizeof(char));
    if (!entrenador_a_cargar->nombre){
        free(entrenador_a_cargar);
        return false;
    }
    strcpy(entrenador_a_cargar->nombre, linea[1]);
    entrenador_a_cargar->pokemones = lista_crear(pokemones_destruir);
    if (!entrenador_a_cargar->pokemones){
        free(entrenador_a_cargar->nombre);
        free(entrenador_a_cargar);
        return false;
    }
    int contador = 2;
    while (linea[contador]){
        bool cargar_pokemones = cargo_info_pokemon(hospital, entrenador_a_cargar->pokemones, linea+contador, entrenador_a_cargar);
        if (cargar_pokemones == false){
            entrenadores_destruir(entrenador_a_cargar);
            return false;
        }
        hospital->cantidad_pokemon++;
        contador += 2;
    }
    cola_t *cola_auxiliar = cola_encolar(entreadores, entrenador_a_cargar);
    if (!cola_auxiliar){
        entrenadores_destruir(entrenador_a_cargar);
    }
    return true;
}

/*PRE: El vector que recibe no debe ser nulo.
*POST: Libera cada elmento que se encuentre dentro del vector pasado por parametro.
*/
void liberar_vectores(char **vtr){
    while (*vtr){
        free(*vtr);
        vtr++;
    }
}

//PRE & POST: Definidas en hospital.h//
bool hospital_leer_archivo(hospital_t *hospital, const char *nombre_archivo){
    if ((!hospital) || (!nombre_archivo)) return false;
    FILE *archivo = fopen(nombre_archivo, "r");
    if (!archivo){
        perror("No se pudo abrir el archivo");
        return false;
    }
    char *linea = leer_linea(archivo);
    char **aux = split(linea, ';');
    free(linea);
    if (!aux){
        fclose(archivo);
        return true;
    }
    while (aux){
        bool se_cargaron_entrenadores = cargo_info_entrenador(hospital, hospital->cola_entrenadores_sala_espera, aux);
        liberar_vectores(aux);
        free(aux);
        if (!se_cargaron_entrenadores){
            fclose(archivo);
            free(aux);
            hospital_liberar(hospital);
            return false;
        }
        hospital->cantidad_entrenadores++;
        linea = leer_linea(archivo);
        aux = split(linea, ';');
        free(linea);
    }
    fclose(archivo);
    return true;
}

//PRE & POST: Definidas en hospital.h//
size_t hospital_cantidad_entrenadores(hospital_t *hospital){
    if (hospital == NULL) return 0;
    return hospital->cantidad_entrenadores;
}

//PRE & POST: Definidas en hospital.h//
size_t hospital_cantidad_pokemon(hospital_t *hospital){
    if (hospital == NULL) return 0;
    return hospital->cantidad_pokemon;
}

/*PRE: La lista que recibe no debe ser nula.
*POST: Ordena los pokemones alfabeticamente y los inserta en la lista pasada por paramentro.
*/
void insertar_lista_pokemones_ordenada(lista_t *lista_pokemones, pokemon_t *pokemon){
    size_t minimo = 0;
    bool encontrado = false;
    for (size_t i = 0; i < lista_tamanio(lista_pokemones) && !encontrado; i++){
        pokemon_t* poke_aux = lista_elemento_en_posicion(lista_pokemones, i);
        if (comparador_pokemones_nombre(poke_aux, pokemon) > 0){
            encontrado = true;
        }
        else{
            minimo++;
        }
    }
    lista_insertar_en_posicion(lista_pokemones, pokemon, minimo);
}

/*
*Inserta los pokemones de la lista al heap minimal para que se ubique el que tiene menor nivel al principio.
*/
void pokemones_a_heap(lista_t *lista_pokemones, heap_t *heap_pokmones){
    if (heap_tamanio(heap_pokmones) == 0) return;
    pokemon_t *pokemon = heap_quitar_raiz(heap_pokmones);
    pokemones_a_heap(lista_pokemones, heap_pokmones);
    insertar_lista_pokemones_ordenada(lista_pokemones, pokemon);
    heap_insertar(heap_pokmones, pokemon);
}

/*
* Inserta los pokemones en una lista.
*/
void pokemones_a_lista(lista_t *lista, lista_t *lista_pokemones){
    for (size_t i = 0; i < lista_tamanio(lista_pokemones); i++){
        pokemon_t* poke_aux = lista_elemento_en_posicion(lista_pokemones, i);
        insertar_lista_pokemones_ordenada(lista, poke_aux);
    }
}

/*
* Ordena la informacion recibida del hospital como corresponde, los pokemones organizados segun su nivel.
*/
void ordenar_hospital(hospital_t *hospital, lista_t *lista_aux){
    for (size_t i = 0; i < cola_tamanio(hospital->cola_entrenadores_sala_espera); i++){
        entrenador_t* entrenador = cola_desencolar(hospital->cola_entrenadores_sala_espera);
        if (entrenador)
            pokemones_a_lista(lista_aux, entrenador->pokemones);
        cola_encolar(hospital->cola_entrenadores_sala_espera, entrenador);
    }
    pokemones_a_lista(lista_aux, hospital->lista_pokemones_atendidos);
    pokemones_a_heap(lista_aux, hospital->heap_pokeones_sala_espera);
    if (hospital->pokemon_atendiendose){
        insertar_lista_pokemones_ordenada(lista_aux, hospital->pokemon_atendiendose);
    }
}

//PRE & POST: Definidas en hospital.h//
size_t hospital_a_cada_pokemon(hospital_t *hospital, bool (*funcion)(pokemon_t *p)){
    if (hospital == NULL || funcion == NULL) return 0;
    size_t contador_pokemones_funcion = 0;
    lista_t *lista_auxiliar = lista_crear(NULL);
    if (!lista_auxiliar) return 0;
    ordenar_hospital(hospital, lista_auxiliar);
    bool funcion_aplicada = true;
    for (size_t i = 0; i < lista_tamanio(lista_auxiliar) && funcion_aplicada; i++, contador_pokemones_funcion++){
        pokemon_t *pokemon_actual = lista_elemento_en_posicion(lista_auxiliar, i);
        funcion_aplicada = funcion(pokemon_actual);
    }
    lista_destruir(lista_auxiliar);
    return contador_pokemones_funcion;
}

//PRE & POST: Definidas en hospital.h//
void hospital_destruir(hospital_t *hospital){
    if (!hospital) return;
    hospital_liberar(hospital);
    free(hospital);
}

//PRE & POST: Definidas en hospital.h//
size_t pokemon_nivel(pokemon_t *pokemon){
    if (pokemon == NULL) return 0;
    return pokemon->nivel;
}

//PRE & POST: Definidas en hospital.h//
const char *pokemon_nombre(pokemon_t *pokemon){
    if (pokemon == NULL) return NULL;
    return pokemon->nombre;
}

//PRE & POST: Definidas en hospital.h//
entrenador_t *pokemon_entrenador(pokemon_t *pokemon){
    if (pokemon == NULL) return NULL;
    return pokemon->entrenador;
}