#include "lista.h"
#include <stddef.h>
#include <stdlib.h>

lista_t* lista_crear(){
    lista_t* lista = calloc(1, sizeof(lista_t)); 
    if(!lista){
        return NULL;
    }
    return lista;
}

lista_t* lista_insertar(lista_t* lista, void* elemento){
    if(!lista){
        return NULL;
    }
    nodo_t* nodo_nuevo = calloc(1, sizeof(nodo_t));
    if(!nodo_nuevo){
        return NULL;
    }
    nodo_nuevo->elemento = elemento;
    if(!lista->nodo_inicio){
        lista->nodo_inicio = nodo_nuevo;
    } else{
        lista->nodo_fin->siguiente = nodo_nuevo;
    }
    lista->nodo_fin = nodo_nuevo;
    lista->cantidad++;
    return lista;
}

/*Recibe un nodo y una posicion a encontrar.
* Devuelve el nodo correspondiente a la posicion buscada.
* De no existir la posicion, devuelve el ultimo nodo. 
*/
nodo_t* encontrar_nodo(nodo_t* nodo, size_t posicion_a_encontrar){
    if(!nodo->siguiente ||(posicion_a_encontrar == 0)){
        return nodo;
    }
    return encontrar_nodo(nodo->siguiente, posicion_a_encontrar-1);
}

lista_t* lista_insertar_en_posicion(lista_t* lista, void* elemento, size_t posicion){
    if(!lista){
        return NULL;
    }

    nodo_t* nodo_nuevo = calloc(1, sizeof(nodo_t));
    if(!nodo_nuevo){
        return NULL;
    }
    nodo_nuevo->elemento = elemento; 
    if(posicion >= lista->cantidad){
        free(nodo_nuevo);
        return lista_insertar(lista, elemento);
    } else if(posicion == (size_t)0){
        nodo_nuevo->siguiente = lista->nodo_inicio;
        lista->nodo_inicio = nodo_nuevo;
    } else{
        nodo_t* nodo_previo = encontrar_nodo(lista->nodo_inicio, posicion-1);
        nodo_nuevo->siguiente = nodo_previo->siguiente;
        nodo_previo->siguiente = nodo_nuevo;
    }

    lista->cantidad++;
    return lista;
}

void* lista_quitar(lista_t* lista){
    if(!lista || lista->cantidad == 0) return NULL;
    nodo_t* elemento_a_quitar = lista->nodo_fin->elemento;
    if(lista->cantidad == 1){
        free(lista->nodo_fin);
        lista->nodo_inicio = NULL;
        lista->nodo_fin = NULL;
    } else if(lista->cantidad == 2){
        free(lista->nodo_fin);
        lista->nodo_fin = lista->nodo_inicio;
    } else {
        nodo_t* anteultimo_nodo = encontrar_nodo(lista->nodo_inicio, lista->cantidad -2);
        free(lista->nodo_fin);
        lista->nodo_fin = anteultimo_nodo;
    }
    lista->cantidad--;
    return(elemento_a_quitar);
    
}

void* lista_quitar_de_posicion(lista_t* lista, size_t posicion){
    if(!lista || lista->cantidad == 0){
        return NULL;
    }
    if(posicion == 0){
        nodo_t* elemento_a_quitar = lista->nodo_inicio->elemento;
        nodo_t* nodo_a_quitar = lista->nodo_inicio;
        lista->nodo_inicio = nodo_a_quitar->siguiente;
        free(nodo_a_quitar);
        lista->cantidad--;
        return elemento_a_quitar;
    }else if(posicion >= lista->cantidad-1){
        return lista_quitar(lista);
    } 
    nodo_t* nodo_precesor = encontrar_nodo(lista->nodo_inicio, posicion-1);
    nodo_t* nodo_a_quitar = nodo_precesor->siguiente;
    nodo_t* elemento_a_quitar = nodo_a_quitar->elemento;
    nodo_precesor->siguiente = nodo_a_quitar->siguiente;
    free(nodo_a_quitar);
    lista->cantidad--;
    return elemento_a_quitar;
}

void* lista_elemento_en_posicion(lista_t* lista, size_t posicion){
    if(lista_vacia(lista) || (posicion >= lista->cantidad)){
        return NULL;
    }
    nodo_t* nodo_en_posicion = encontrar_nodo(lista->nodo_inicio, posicion);
    return nodo_en_posicion->elemento;
}

void* lista_primero(lista_t* lista){
    if(!lista || lista_vacia(lista)) return NULL;
    return lista->nodo_inicio->elemento;
}

void* lista_ultimo(lista_t* lista){
    if(!lista || lista_vacia(lista)) return NULL;
    return lista->nodo_fin->elemento;
}

bool lista_vacia(lista_t* lista){
    if(!lista) return true;
    return ((!lista->cantidad) ||(!lista->nodo_inicio) || (!lista->nodo_fin));
}

size_t lista_tamanio(lista_t* lista){
    if( !lista || lista_vacia(lista)) return 0;
    return lista->cantidad;
}

void lista_destruir(lista_t* lista){
    if(!lista) return;
    while(lista->cantidad > 0){
        lista_quitar_de_posicion(lista, 0);
    }
    free(lista);
}

lista_iterador_t* lista_iterador_crear(lista_t* lista){
    if(!lista) return NULL;
    lista_iterador_t* iterador = calloc(1, sizeof(lista_iterador_t));
    if(!iterador) return NULL;
    iterador->lista = lista;
    if(!lista_vacia(lista)){
        iterador->corriente = lista->nodo_inicio;
    }
    return iterador;
}

bool lista_iterador_tiene_siguiente(lista_iterador_t* iterador){
    if(!iterador){
        return false;
    } 
    return iterador->corriente;
}

bool lista_iterador_avanzar(lista_iterador_t* iterador){
    if(!iterador || !iterador->corriente){
        return false;
    }
    iterador->corriente = iterador->corriente->siguiente;
    return iterador->corriente;
}

void* lista_iterador_elemento_actual(lista_iterador_t* iterador){
    if(!iterador || !iterador->corriente) return NULL;
    return iterador->corriente->elemento;
}

void lista_iterador_destruir(lista_iterador_t* iterador){
      if(!iterador) return;
    free(iterador);
}

size_t lista_con_cada_elemento(lista_t* lista, bool (*funcion)(void*, void*), void *contexto){
    if(!lista||!funcion) return 0;
    size_t contador = 0;
    nodo_t* nodo_actual = lista->nodo_inicio;
    while(nodo_actual && (funcion(nodo_actual->elemento, contexto))){
        nodo_actual = nodo_actual->siguiente;
        contador++;
    }
    return contador;
}
