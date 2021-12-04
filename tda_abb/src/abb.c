#include "abb.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

abb_t* abb_crear(abb_comparador comparador){
  if(!comparador) return NULL;
  abb_t* abb = malloc(sizeof(abb_t));
  if(!abb) return NULL;
  abb->comparador = comparador;
  abb->tamanio = 0;
  abb->nodo_raiz = NULL;
  return abb;
}

/* Se crea un nodo reservando la memoria necesaria para este
* y se inicializan los valores de sus elementos. 
*/
nodo_abb_t* crear_nodo(void* elemento){
  nodo_abb_t* nodo = calloc(1, sizeof(nodo_abb_t));
  if(!nodo) return NULL;
  nodo->elemento = elemento;
  nodo->izquierda = NULL;
  nodo->derecha = NULL;
  return nodo;
}

/*inserta un nodo creado en el lugar que le corresponde, 
* para eso llama al comparador.
*/
nodo_abb_t* insertar_nodo(nodo_abb_t* raiz, void* elemento, abb_comparador comparador){
  if(!raiz){
    raiz = crear_nodo(elemento);
    return raiz;
  }
   int comparacion = comparador(elemento, raiz->elemento);
  if(comparacion <= 0){
    raiz->izquierda = insertar_nodo(raiz->izquierda, elemento, comparador);
  } else{
    raiz->derecha = insertar_nodo(raiz->derecha, elemento, comparador);
  }
  return raiz;
}

abb_t* abb_insertar(abb_t* arbol, void* elemento){
  if(!arbol) return NULL;
  arbol->nodo_raiz = insertar_nodo(arbol->nodo_raiz, elemento, arbol->comparador);
  arbol->tamanio++;
  return arbol;
}

/*recorre el arbol recursivamente en busqueda del nodo con el mismo elemento que el pasado por paramtro,
*utiliza el comprador del arbol para el recorrido.
*/
void* abb_buscar_nodo(nodo_abb_t* nodo_a_buscar, void* elemento, abb_comparador comparador){
  if(!nodo_a_buscar ||!elemento) return NULL;
  int comparacion = comparador(nodo_a_buscar->elemento, elemento);
  if(comparacion > 0){
    return abb_buscar_nodo(nodo_a_buscar->izquierda, elemento, comparador);
  } else if(comparacion < 0){
    return abb_buscar_nodo(nodo_a_buscar->derecha, elemento, comparador);
  }
  return nodo_a_buscar;
}

void* abb_buscar(abb_t* arbol, void* elemento){
  if(!arbol) return NULL;
  nodo_abb_t* nodo_a_buscar = abb_buscar_nodo(arbol->nodo_raiz, elemento, arbol->comparador);
  if(!nodo_a_buscar) return NULL;
  return nodo_a_buscar->elemento;
}

/*Busca el mayor del lado derecho del arbol (el predecesor inorden).
*
*/
nodo_abb_t* predecesor_inorden(nodo_abb_t* nodo){
  if(nodo->derecha){
    return predecesor_inorden(nodo->derecha);
  }
  return nodo;
}

/*Libera la memoria del nodo a quitar y arregla la referencia del nodo padre en caso de tener hijos (el nodo a quitar).
*
*/
nodo_abb_t* quitar_nodo(abb_t* arbol, nodo_abb_t* nodo, void* elemento){
  if(!nodo) return NULL;
  int comparacion = arbol->comparador(nodo->elemento, elemento);
  if(comparacion == 0){
    if(!nodo->izquierda){
      nodo_abb_t* aux = nodo->derecha;
      free(nodo);
      return aux;
    } else if(!nodo->derecha){
      nodo_abb_t* aux = nodo->izquierda;
      free(nodo);
      return aux;
    } else if(nodo->derecha && nodo->izquierda){
      nodo_abb_t* predecesor = predecesor_inorden(nodo->izquierda);
      nodo->elemento = predecesor->elemento;
      nodo->izquierda = quitar_nodo(arbol, nodo->izquierda, predecesor->elemento);
    } 
  } else if(comparacion > 0){
    nodo->izquierda = quitar_nodo(arbol, nodo->izquierda, elemento);
  } else if(comparacion < 0){
    nodo->derecha = quitar_nodo(arbol, nodo->derecha, elemento);
  }
  return nodo;
}

void* abb_quitar(abb_t* arbol, void* elemento){
  if(!arbol) return NULL;
  void* elemento_quitado = abb_buscar(arbol, elemento);
  arbol->nodo_raiz = quitar_nodo(arbol, arbol->nodo_raiz, elemento);
  arbol->tamanio--;
  return elemento_quitado;
}

bool abb_vacio(abb_t* arbol){
  if(!arbol|| !arbol->nodo_raiz|| arbol->tamanio ==0) return true;
  return false;
}

size_t abb_tamanio(abb_t *arbol){
  if(!arbol) return 0;
  return arbol->tamanio;
}

void abb_destruir(abb_t *arbol){
  if(!arbol) return;
  abb_destruir_todo(arbol, NULL);
}

/*PRE:Recibe un nodo, un destructor y un arbol validos. 
*POST: libera la memoria reservada para el nodo y llama al destructor en caso de ser valido.
*/
void abb_destruir_nodo( nodo_abb_t* nodo, void (*destructor)(void *)){
  if(!nodo) return;
  if(nodo->izquierda)
    abb_destruir_nodo(nodo->izquierda, destructor);
  if(nodo->derecha)
    abb_destruir_nodo(nodo->derecha, destructor);
  if(destructor)
    destructor(nodo->elemento);
  free(nodo);
}

void abb_destruir_todo(abb_t *arbol, void (*destructor)(void *)){
  if(!arbol) return;
  abb_destruir_nodo(arbol->nodo_raiz, destructor);
  free(arbol);    
}


/*Recorre primero el izquierdo, despues el nodo raiz y despues el izquierdo. 
* en caso de que la funcion retorna falso, se detiene. 
*/
void abb_recorrido_inorden_con_cada_elemento(nodo_abb_t* nodo, bool (*funcion)(void *, void *),  void *aux, size_t* contador, bool* detenido){
  if(!nodo || *detenido) return;
  //izq
  if(nodo->izquierda && !(*detenido)){
    abb_recorrido_inorden_con_cada_elemento(nodo->izquierda, funcion, aux, contador, detenido);
  }
  if(!(*detenido)){
    (*contador)++;
  }
  //centro
  if(!nodo || *detenido) return;
  if(funcion(nodo->elemento, aux) == false){
    *detenido = true;
    return;
  }
  //derecha
  if(nodo->derecha && !(*detenido)){
    abb_recorrido_inorden_con_cada_elemento(nodo->derecha, funcion, aux, contador, detenido);
  }
}

/*Recorre primero la raiz, despues el nodo izquiedo y por ultimo el derecho. 
* en caso de que la funcion retorna falso, se detiene. 
*/
void abb_recorrido_preorden_con_cada_elemento(nodo_abb_t* nodo, bool (*funcion)(void *, void *),  void *aux, size_t* contador, bool* detenido){
  if(!nodo || *detenido) return;
  //chequeo centro
  if(!(*detenido)){
    (*contador)++;
  }
  if(funcion(nodo->elemento, aux) == false){
    *detenido = true;
    return;
  }
  //lado izq
  if(nodo->izquierda && !(*detenido)){
    abb_recorrido_preorden_con_cada_elemento(nodo->izquierda, funcion, aux, contador, detenido);
  }
  // if(!nodo || *detenido) return;
  //chequeo derecho
  if(nodo->derecha && !(*detenido)){
    abb_recorrido_preorden_con_cada_elemento(nodo->derecha, funcion, aux, contador, detenido);
  }
}

/*Recorre primero el lado izquierdo, despues el derecho y por ultimo la raiz. 
* en caso de que la funcion retorna falso, se detiene. 
*/
void abb_recorrido_postorden_con_cada_elemento(nodo_abb_t* nodo, bool (*funcion)(void *, void *), void *aux, size_t* contador, bool* detenido){
  if(!nodo|| (*detenido)) return;
  //lado izq
  if(nodo->izquierda && !(*detenido)){
    abb_recorrido_postorden_con_cada_elemento(nodo->izquierda, funcion, aux, contador, detenido);
  }
  //chequeo derecho
  if(nodo->derecha && !(*detenido)){
    abb_recorrido_postorden_con_cada_elemento(nodo->derecha, funcion, aux, contador, detenido);
  }
  if(!nodo|| (*detenido)) return;
  //chequeo centro
  if(!(*detenido)){
    (*contador)++;
  }
  if(funcion(nodo->elemento, aux) == false){
    *detenido = true;
    return;
  }
}

size_t abb_con_cada_elemento(abb_t *arbol, abb_recorrido recorrido, bool (*funcion)(void *, void *), void *aux){
  if(!arbol || funcion == NULL) return 0;
  bool detenido = false;
  size_t contador = 0;
  switch(recorrido){
    case INORDEN:
      abb_recorrido_inorden_con_cada_elemento(arbol->nodo_raiz, funcion, aux, &contador, &detenido);
      break;
    case PREORDEN:
      abb_recorrido_preorden_con_cada_elemento(arbol->nodo_raiz, funcion, aux, &contador, &detenido);
      break;
    case POSTORDEN:
      abb_recorrido_postorden_con_cada_elemento(arbol->nodo_raiz, funcion, aux, &contador, &detenido);
      break;
  }
  return contador;
}


/*Recorre primero el izquierdo, despues el nodo raiz y despues el izquierdo. 
* Va copiando los elementos en el array pasado por parametro hasta agotar su capacidad.
*/
void recorrido_inorden(nodo_abb_t* nodo, void** array, size_t tamanio_array, size_t* cantidad){
  if(!nodo) return;
  if(nodo->izquierda)
    recorrido_inorden(nodo->izquierda, array, tamanio_array, cantidad);
  if((*cantidad) >= tamanio_array) return; 
  array[*cantidad] = nodo->elemento;
  (*cantidad)++;
  if(nodo->derecha)
    recorrido_inorden(nodo->derecha, array, tamanio_array, cantidad);
  return;
}

/*Recorre primero la raiz, despues el nodo izquiedo y por ultimo el derecho. 
* Va copiando los elementos en el array pasado por parametro hasta agotar su capacidad.
*/
void recorrido_preorden(nodo_abb_t* nodo, void** array, size_t tamanio_array, size_t* cantidad){
  if(!nodo|| (*cantidad) >= tamanio_array) return;
  array[*cantidad] = nodo->elemento;
  (*cantidad)++;
  if(nodo->izquierda) 
    recorrido_preorden(nodo->izquierda, array, tamanio_array,  cantidad);
  if(nodo->derecha)
    recorrido_preorden(nodo->derecha, array, tamanio_array,  cantidad);
  return;
}

/*Recorre primero el lado izquierdo, despues el derecho y por ultimo la raiz. 
* Va copiando los elementos en el array pasado por parametro hasta agotar su capacidad.
*/
void recorrido_postorden(nodo_abb_t* nodo, void** array, size_t tamanio_array, size_t* cantidad){
  if(!nodo) return;
  if(nodo->izquierda)
    recorrido_postorden(nodo->izquierda, array, tamanio_array, cantidad);
  if(nodo->derecha)
    recorrido_postorden(nodo->derecha, array, tamanio_array,  cantidad);
  if((*cantidad)>= tamanio_array) return; 
  array[*cantidad] = nodo->elemento;
  (*cantidad)++;
  return;
}

size_t abb_recorrer(abb_t* arbol, abb_recorrido recorrido, void** array, size_t tamanio_array){
  if(!arbol || !tamanio_array) return 0;
  size_t cantidad_elemntos_almacenados = 0;
  switch(recorrido){
    case INORDEN:
      recorrido_inorden(arbol->nodo_raiz, array, tamanio_array, &cantidad_elemntos_almacenados);
      break;
    case PREORDEN:
      recorrido_preorden(arbol->nodo_raiz, array, tamanio_array, &cantidad_elemntos_almacenados);
      break;
    case POSTORDEN:
      recorrido_postorden(arbol->nodo_raiz, array, tamanio_array, &cantidad_elemntos_almacenados);
    break;
  }
  return cantidad_elemntos_almacenados;
}
