#include "cola.h"
#include "lista.h"

struct _cola_t{
    size_t elementos;
    destructor_t destructor;
};

// PRE & POST en cola.h // 
cola_t* cola_crear(destructor_t destructor){
    cola_t* cola = (cola_t*)lista_crear(destructor);
    if(!cola){
        return NULL;
    }
    cola->destructor= destructor;
    return cola;
}

// PRE & POST en cola.h // 
cola_t* cola_encolar(cola_t* cola, void* elemento){
    if(!cola) return NULL;
    return (cola_t*)lista_insertar((lista_t*)cola, elemento);
}

// PRE & POST en cola.h // 
void* cola_desencolar(cola_t* cola){
    return lista_quitar_de_posicion((lista_t*)cola,0);
}

// PRE & POST en cola.h // 
void* cola_frente(cola_t* cola){
    return lista_primero((lista_t*)cola);
}

// PRE & POST en cola.h // 
size_t cola_tamanio(cola_t* cola){
    return lista_tamanio((lista_t*)cola);
}

// PRE & POST en cola.h // 
bool cola_vacia(cola_t* cola){
    return lista_vacia((lista_t*)cola);
}

// PRE & POST en cola.h // 
void cola_destruir(cola_t* cola){
    lista_destruir((lista_t*)cola);
}