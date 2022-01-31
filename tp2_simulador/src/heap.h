#ifndef HEAP_H_
#define HEAP_H_
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//En este archivo se encuentran las primitivas del TDA Heap 

typedef int (*heap_comparador)(void*, void*);
typedef void (*heap_destructor)(void*);
typedef struct _heap_t heap_t;

/*
* Crea el heap, reservando memoria para este e inicializando sus atributos. 
*/
heap_t* heap_crear(heap_comparador comparador, heap_destructor destructor);

/*
*Recibe un heap valido y dos posiciones a intercambiar. Lo que se encuentre dentro de la posicion 1 ira a la posicion dos y viceversa, esto lo hace con ayuda de una variable auxiliar. 
*/
void swap(heap_t* heap, size_t pos_1, size_t pos_2);

/*
* Reubica los elementos en la posicion correspondiente subiendolos, de manera que resulta un heap minimal.
*/
void sift_up(heap_t* heap, int posicion_actual);

/*
* Dado un elemento, este se inserta en el heap, realizando las comparaciones correspondientes con los elementos que ya se encuentran dentro del mismo y se reordena realizando la operacion sift up en caso de que el elemento insertado sea menor a los que ya se encuentran en el mismo. 
*/
heap_t* heap_insertar(heap_t* heap, void* elemento);

/*
* Reubica los elementos en la posicion correspondiente bajandolos, de manera que resulta un heap minimal.
*/
void sift_down(heap_t* heap, int posicion_actual);

/*
*Retorna la cantidad de elementos guardados en el heap. 
*/
size_t heap_tamanio(heap_t* heap);

/*
* Retorna el elemento que se encuentra en la raiz de heap, al ser un heap minimal debe retornar el elemento menor. 
*/
void* heap_raiz(heap_t* heap);

/* 
* Quita la raiz actual del heap, al ser minimal, esta resulta ser el elemento mas chico dentro del mismo.
*/
void* heap_quitar_raiz(heap_t* heap);

/*
* Libera la memoriareservada para el heap, incluyendo los elementos dentro de este. 
*/
void heap_destruir(heap_t* heap);

#endif// HEAP_H_