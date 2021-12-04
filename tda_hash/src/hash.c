#include "hash.h"
#include "lista.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define ERROR -1
#define EXITOSO 0
#define CAPACIDAD_MIN 3
#define CAPACIDAD_MAX_CARGA 0.75

struct hash{
    size_t cantidad;
    hash_destruir_dato_t destructor;
    size_t capacidad;
    lista_t** tabla;
};

typedef struct dato{
    char* clave;
    void* elemento;
} dato_t;


/* 
* Se encarga de calcular la posicion dada una clave y la capacidad de un hash. 
*/
size_t funcion_hash(const char* clave, size_t capacidad){
    size_t valor = 0;
    while(*clave){
        valor += *clave++;
    }
    return valor % capacidad;
}

/*
* Copia la clave en un string auxiliar
*/
char* copiar_clave(const char* clave){
    if(!clave) return NULL;
    char* copia = malloc(strlen(clave)+1);
    strcpy(copia, clave);
    return copia;
}

/*
* Reserva memoria para el nodo que contiene la clave y el elemento.
*/
dato_t* crear_dato(const char* clave, void* elemento){
    if(!clave) return NULL;
    dato_t* dato_creado = malloc(sizeof(dato_t));
    if(!dato_creado) return NULL;
    dato_creado->clave = copiar_clave(clave);
    dato_creado->elemento = elemento;
    return dato_creado;
}

/*
* Libera la memoria del dato almacenado, la clave y el elemento aplicando el destructor. 
*/
void destruir_dato(dato_t* dato, hash_destruir_dato_t destructor){
    if(!dato) return;
    if(destructor)
        destructor(dato->elemento);
    free(dato->clave);
    free(dato);
}

/*
* Calcula el factor de carga del hash.
*/
float calcular_carga(hash_t* hash){
    float factor = (float)(hash->cantidad/hash->capacidad);
    return factor;
}

/*
* Calcula la nueva capacidad del hash
*/
size_t nueva_capacidad_hash(size_t capacidad_anterior){
    size_t nueva_capacidad = capacidad_anterior*2;
    return nueva_capacidad;
}

hash_t* hash_crear(hash_destruir_dato_t destruir_elemento, size_t capacidad_inicial){
    hash_t* hash = malloc(sizeof(hash_t));
    if(!hash) return NULL;
    if(capacidad_inicial < CAPACIDAD_MIN)
        capacidad_inicial = CAPACIDAD_MIN;
    lista_t** lista = malloc(sizeof(lista_t)*capacidad_inicial);
    if(!lista){
        free(hash);
        return NULL;
    }
    hash->tabla = lista;
    for(size_t i=0; i< capacidad_inicial; i++){
        hash->tabla[i] = lista_crear();
    }
    hash->capacidad = capacidad_inicial;
    hash->cantidad = 0;
    hash->destructor = destruir_elemento;
    return hash;
}

/*
* Se encarga de trasladar los elementos de la tabla de hash anterior 
* a una nueva con la capacidad renovada.
*/
int funcion_rehash(hash_t* hash){
    size_t nueva_capacidad = nueva_capacidad_hash(hash->capacidad);
    size_t cantidad = hash->cantidad;
    lista_t** nueva_tabla = malloc(sizeof(lista_t*)*nueva_capacidad);
    if(!nueva_tabla) return ERROR;
    for(size_t i= 0; i<nueva_capacidad; i++){
        nueva_tabla[i] = lista_crear();
    }
    for(size_t j = 0; j <hash->capacidad; j++){
        if(hash->tabla[j]){
            while (!lista_vacia(hash->tabla[j])){
                dato_t* dato_auxiliar = (dato_t*)lista_elemento_en_posicion(hash->tabla[j], 0);
                lista_quitar_de_posicion(hash->tabla[j],0);
                size_t posicion = funcion_hash(dato_auxiliar->clave, nueva_capacidad);
                lista_insertar(nueva_tabla[posicion], dato_auxiliar);
            }
            lista_destruir(hash->tabla[j]);
        }
    }
    free(hash->tabla);
    hash->tabla = nueva_tabla;
    hash->capacidad = nueva_capacidad;
    hash->cantidad = cantidad;
    return EXITOSO;
}

int hash_insertar(hash_t* hash, const char* clave, void* elemento){
    if(!hash ||!clave) return ERROR;
    int estado_insercion = ERROR;
    bool se_agrego_dato = false;
    float carga_hash= calcular_carga(hash);
    if(CAPACIDAD_MAX_CARGA <= carga_hash){
        estado_insercion = funcion_rehash(hash);
        if(estado_insercion == ERROR){
            return estado_insercion;
        }
    }
    dato_t* dato = crear_dato(clave, elemento);
    if(!dato) estado_insercion = ERROR;
    size_t posicion = funcion_hash(clave, hash->capacidad);
    size_t cantidad_elementos = lista_tamanio(hash->tabla[posicion]);
    for(size_t i = 0; i< cantidad_elementos; i++){
        dato_t* dato_en_tabla = (dato_t*)lista_elemento_en_posicion(hash->tabla[posicion], i);
        if(dato_en_tabla){
            if(strcmp(dato->clave, dato_en_tabla->clave)==0){
                destruir_dato(dato_en_tabla, hash->destructor);
                lista_quitar_de_posicion(hash->tabla[posicion], i);
                lista_insertar_en_posicion(hash->tabla[posicion], dato, i);
                se_agrego_dato = true;
            }
        }
    } 
    if(!se_agrego_dato){
        lista_insertar(hash->tabla[posicion], dato);
        hash->cantidad++;
    }
    estado_insercion = EXITOSO;
    return estado_insercion;
}

int hash_quitar(hash_t* hash, const char* clave){
    if(!hash || !clave) return ERROR;
    int estado_quitar = ERROR;
    dato_t dato_auxiliar;
    dato_auxiliar.clave = "";
    dato_auxiliar.elemento = NULL;
    dato_auxiliar.clave = copiar_clave(clave);
    size_t posicion = funcion_hash(clave, hash->capacidad);
    size_t cantidad_elementos = lista_tamanio(hash->tabla[posicion]);
    for(size_t i =0; i< cantidad_elementos; i++){
        dato_t* dato_en_tabla = (dato_t*)lista_elemento_en_posicion(hash->tabla[posicion], i);
        if(dato_en_tabla){
            if(strcmp(dato_en_tabla->clave, dato_auxiliar.clave)==0){
                lista_quitar_de_posicion(hash->tabla[posicion], i);
                destruir_dato(dato_en_tabla, hash->destructor);
                hash->cantidad--;
                estado_quitar = EXITOSO;
            }
        }
    }
    free(dato_auxiliar.clave);
    return estado_quitar;
}

void* hash_obtener(hash_t* hash, const char* clave){
    if(!hash || !clave) return NULL;
    dato_t dato_auxiliar;
    dato_auxiliar.clave = "";
    dato_auxiliar.elemento = NULL;
    size_t posicion = funcion_hash(clave, hash->capacidad);
    size_t cantidad_elementos = lista_tamanio(hash->tabla[posicion]);
    dato_auxiliar.clave = copiar_clave(clave);
    for(size_t i = 0; i< cantidad_elementos; i++){
        dato_t* dato_en_tabla = (dato_t*)lista_elemento_en_posicion(hash->tabla[posicion], i);
        if(strcmp(dato_en_tabla->clave, dato_auxiliar.clave)==0){
            free(dato_auxiliar.clave);
            return dato_en_tabla->elemento;
        }
    }
    free(dato_auxiliar.clave);
    return NULL;
}

bool hash_contiene(hash_t* hash, const char* clave){
    if(!hash || !clave) return false;
    dato_t dato_auxiliar;
    dato_auxiliar.clave = "";
    dato_auxiliar.elemento = NULL;
    size_t posicion = funcion_hash(clave, hash->capacidad);
    size_t cantidad_elementos = lista_tamanio(hash->tabla[posicion]);
    dato_auxiliar.clave = copiar_clave(clave);
    for(size_t i = 0; i< cantidad_elementos; i++){
        dato_t* dato_en_tabla = (dato_t*)lista_elemento_en_posicion(hash->tabla[posicion], i);
        if(strcmp(dato_en_tabla->clave, dato_auxiliar.clave)==0){
            free(dato_auxiliar.clave);
            return true;
        }
    }
    free(dato_auxiliar.clave);
    return false;
}

size_t hash_cantidad(hash_t* hash){
    if(!hash) return 0;
    return hash->cantidad;
}

void hash_destruir(hash_t* hash){
    if(!hash) return;
    dato_t* auxiliar = NULL;
    size_t capacidad = hash->capacidad;
    for(size_t i = 0; i< capacidad; i++){
        if(hash->tabla[i]){
            while (!lista_vacia(hash->tabla[i])){
                auxiliar = (dato_t*)lista_elemento_en_posicion(hash->tabla[i], 0);
                lista_quitar_de_posicion(hash->tabla[i], 0);
                destruir_dato(auxiliar, hash->destructor);
            }
            lista_destruir(hash->tabla[i]);
        }
    }
    free(hash->tabla);    
    free(hash);
}

size_t hash_con_cada_clave(hash_t* hash, bool (*funcion)(hash_t* hash, const char* clave, void* aux), void* aux){
    if(!hash ||!funcion) return 0;
    size_t contador = 0;
    bool resultado_funcion = false;
    for(size_t i = 0; i< hash->capacidad; i++){
        if(hash->tabla[i]){
            size_t cantidad_elementos = lista_tamanio(hash->tabla[i]);
            for(size_t j = 0; j < cantidad_elementos; j++){
                dato_t* dato_en_tabla = (dato_t*)lista_elemento_en_posicion(hash->tabla[i], j);
                resultado_funcion = funcion(hash, dato_en_tabla->clave, aux);
                contador++;
                if(resultado_funcion == true) return contador;
            }
        }
        if(resultado_funcion == true) return contador;
    }
    return contador;
}