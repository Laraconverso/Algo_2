#include "heap.h"
#include <stdio.h>
#include <stdlib.h>

//HEAP MINIMAL (el mas chico se encuentra en la raiz).
struct _heap_t{
    void** elementos;
    heap_comparador comparador;
    heap_destructor destructor;
    size_t cant_elementos;
};

//Dada una posicion actual valida, se obtiene la posicion de su padre.
int pos_padre(int pos_actual){
    return (pos_actual-1)/2;
}

//Dada una posicion actual valida, se obtiene la posicion de su hijo izquierdo.
int posicion_hijo_izquierdo(int pos_actual){
   return pos_actual*2+1;
}

//Dada una posicion actual valida, se obtiene la posicion de su hijo derecho.
int posicion_hijo_derecho(int pos_actual){
    return pos_actual*2+2;
}

//PRE & POST: Definidas en heap.h//
heap_t* heap_crear(heap_comparador comparador, heap_destructor destructor){
    if(!comparador) return NULL;
    heap_t* heap_nuevo = calloc(1,sizeof(heap_t));
    if(!heap_nuevo) return NULL;
    heap_nuevo->comparador = comparador;
    heap_nuevo->destructor = destructor;
    return heap_nuevo;
}

//PRE & POST: Definidas en heap.h// 
void swap(heap_t* heap, size_t pos_1, size_t pos_2){
    void* auxiliar = heap->elementos[pos_1];
    heap->elementos[pos_1] = heap->elementos[pos_2];
    heap->elementos[pos_2] = auxiliar;
}

//PRE & POST: Definidas en heap.h// 
void sift_up(heap_t* heap, int posicion_actual){
    if(posicion_actual == 0) return;
    int posicion_padre = pos_padre(posicion_actual);
    if(posicion_padre < 0) return;
    void* elemento_actual = heap->elementos[posicion_actual];
    void* padre = heap->elementos[posicion_padre];
    if(heap->comparador(elemento_actual, padre) < 0 ){
        swap(heap, (size_t)posicion_actual, (size_t)posicion_padre);
        sift_up(heap, posicion_padre);
    }
}

//PRE & POST: Definidas en heap.h// 
heap_t* heap_insertar(heap_t* heap, void* elemento){
    if(!heap)return NULL;
    void** auxiliar = realloc(heap->elementos, (heap->cant_elementos+1)*sizeof(void*));
    if(!auxiliar) return NULL;
    heap->elementos = auxiliar;
    heap->elementos[heap->cant_elementos] = elemento;
    heap->cant_elementos++;
    sift_up(heap, (int)heap->cant_elementos-1);
    return heap;
}

//PRE & POST: Definidas en heap.h// 
void sift_down(heap_t* heap, int posicion_actual){
    if(!heap->elementos) return;
    int pos_hijo_izquierdo = posicion_hijo_izquierdo(posicion_actual);
    int pos_hijo_derecho = posicion_hijo_derecho(posicion_actual);
    if(pos_hijo_izquierdo >= heap_tamanio(heap)) return;
    int posicion_intecambio = pos_hijo_izquierdo;
    if(pos_hijo_derecho < heap_tamanio(heap)){
        void* hijo_izquierdo = heap->elementos[pos_hijo_izquierdo];
        void* hijo_derecho = heap->elementos[pos_hijo_derecho];
        if(pos_hijo_derecho< heap_tamanio(heap)){
            if(heap->comparador(hijo_izquierdo, hijo_derecho)>0){
                posicion_intecambio = pos_hijo_derecho;
            }
        }
    } 
    void* elemento_actual = heap->elementos[posicion_actual];
    void* elemento_intercambio = heap->elementos[posicion_intecambio];
    if(heap->comparador(elemento_actual, elemento_intercambio) >0){
        swap(heap, (size_t)posicion_actual, (size_t)posicion_intecambio);
        sift_down(heap, posicion_intecambio);
    }
}

//PRE & POST: Definidas en heap.h//
size_t heap_tamanio(heap_t* heap){
    if(!heap) return 0;
    return heap->cant_elementos;
}

//PRE & POST: Definidas en heap.h//
void* heap_raiz(heap_t* heap){
    if(!heap || heap_tamanio(heap) == 0) return NULL;
    return heap->elementos[0];
}

//PRE & POST: Definidas en heap.h// 
void* heap_quitar_raiz(heap_t* heap){
    if(!heap || (heap_tamanio(heap)==0))return NULL;
    void* elemento_a_quitar = heap_raiz(heap);
    swap(heap, 0, heap->cant_elementos-1);
    void** auxiliar= realloc(heap->elementos, (heap->cant_elementos-1)*sizeof(void*));
    if(!auxiliar && heap->cant_elementos >1) return NULL;
    heap->elementos = auxiliar;
    heap->cant_elementos--;
    if(heap_tamanio(heap)>0){
        sift_down(heap, 0);
    }
    else{
        heap->elementos = NULL;
    }
    return elemento_a_quitar;
}

//PRE & POST: Definidas en heap.h//
void heap_destruir(heap_t* heap){
    if(!heap) return;
    if(heap->elementos){
        for(size_t i = 0; i< heap_tamanio(heap); i++){
            if(heap->destructor)
                heap->destructor(heap->elementos[i]);
        }
        free(heap->elementos);
    }
    free(heap);
}