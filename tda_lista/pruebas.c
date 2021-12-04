#include "src/lista.h"
#include "src/pila.h"
#include "src/cola.h"
#include <stdio.h>
#include <stdlib.h>
#include "pa2mm.h"

/////Pruebas LISTA/////
// creacion lista
void CrearUnaLista_CreaListaVacia(){
  lista_t* lista = NULL;

  pa2m_afirmar((lista = lista_crear()), "Crear una lista devuelve un puntero a la lista.");
  pa2m_afirmar((lista->cantidad == 0), "La lista inicia con 0 nodos.");
  pa2m_afirmar((lista->nodo_inicio == NULL), "La lista inicia con un nodo inicial nulo.");
  pa2m_afirmar((lista->nodo_fin == NULL),  "La lista inicia con un nodo final nulo.");

  lista_destruir(lista);
}

// insertar elementos
void dadaUnaListaNula_InsertoElementosAlaLista_ObtengoNull(){
  pa2m_afirmar((lista_insertar(NULL,NULL) == NULL), "Insertar un elemento nulo a una lista nula retorna NULL.");

  void* elemento = (void*)1;
  pa2m_afirmar((lista_insertar(NULL, elemento) == NULL), "Insertar un elmento a una lista nula retorna NULL");
}

void dadaUnaListaVacia_InstertoElementosALaLista_ObtengoListaConElementosAumentados(){
  lista_t* lista = lista_crear();
  void* elemento = (void*)1;

  lista_insertar(lista, elemento);
  pa2m_afirmar((lista->cantidad == 1), "Inserto un elemento a la lista, lista con cantidad 1.");

  lista_insertar(lista, elemento);
  pa2m_afirmar((lista->cantidad == 2), "Inserto un segundo elemento a la lista, lista con cantidad 2.");

  lista_destruir(lista);
}

//insertar elmentos en posicion
void dadaUnaListaNulaElementoYPosicion_InsertoElementosALaLista_ObtengoNull(){
  void* elemento = (void*)1;
  pa2m_afirmar((lista_insertar_en_posicion(NULL, elemento, 0)== NULL), "No puedo insertar elmento en una lista nula.");
}

void dadaUnaListaVaciaUnElementoYPosicion_InsertoElementosEnPosicion_ObtengoListaConElementosAumentados(){
  lista_t* lista = lista_crear();
  void* elemento = (void*)1;
  lista_insertar_en_posicion(lista, elemento, 0);
  pa2m_afirmar((lista->cantidad == 1), "Cantidad de elementos es  1.");
  lista_insertar_en_posicion(lista, elemento, 1);
  pa2m_afirmar((lista->cantidad == 2), "Cantidad de elementos es  2.");

  lista_destruir(lista);
}

void dadaUnaListaVaciaUnElementoYPosicion0_InsertoElementosAlPrincipioDeLaLista(){
  lista_t* lista = lista_crear();
  void* elemento = (void*)1;
  lista_insertar_en_posicion(lista, elemento, 0);
  pa2m_afirmar(lista->nodo_inicio->elemento == elemento, "Se agrega un elemento al principio cuando la lista esta vacia.");
  lista_insertar_en_posicion(lista, elemento, 0);
  pa2m_afirmar(lista->nodo_inicio->elemento == elemento,"Se agrega un nuevo elemento al principio de la lista.");

  lista_destruir(lista);
}

void dadaUnaListaVaciaUnElementoYPosicionMayorA1_InsertoElementoEnLaUltimaPosicion(){
  lista_t* lista = lista_crear();
  void* elemento = (void*)1;
  lista_insertar_en_posicion(lista, elemento, 8);
  pa2m_afirmar(lista->nodo_fin->elemento == elemento, "Se inserto un elmento al final de la lista.");

  lista_destruir(lista);
}

void dadaUnaListaUnElementoYPosicion_InsertoCuatroElementos_ObtengoListaDeNodosEnlazados(){
  lista_t* lista = lista_crear();
  void* elemento_uno = (void*)1;
  void* elemento_dos = (void*)1;
  void* elemento_tres = (void*)1;
  void* elemento_cuatro = (void*)1;

  lista_insertar_en_posicion(lista, elemento_uno, 0);
  lista_insertar_en_posicion(lista, elemento_tres, 2);
  lista_insertar_en_posicion(lista, elemento_cuatro, 3);
  lista_insertar_en_posicion(lista, elemento_dos, 1);

  pa2m_afirmar(lista->nodo_inicio->elemento == elemento_uno, "El elemento 1 es el correspondiente a la posicion asignada.");
  pa2m_afirmar(lista->nodo_inicio->siguiente->elemento == elemento_dos, "El elemento 2 es el correspondiente a la posicion asignada.");
  pa2m_afirmar(lista->nodo_inicio->siguiente->siguiente->elemento ==elemento_tres, "El elemento 3 es el correspondiente a la posicion asignada.");
  pa2m_afirmar(lista->nodo_fin->elemento == elemento_cuatro, "El elemento 4 es el correspondiente a la posicion asignada.");

  lista_destruir(lista);
}

// quitar elementos
void dadaUnaListaNula_QuitarUnElementoDevuelveNULL(){
  pa2m_afirmar(lista_quitar(NULL) == NULL, "Quitar elemento de una lista nula retorna NULL.");
}

void dadaUnaListaVacia_QuitarUnElementoDevuelveNULL(){
  lista_t* lista = lista_crear();
  pa2m_afirmar(lista_quitar(lista) == NULL, "Quitar un elmento de una lista vacia retorna NULL");

  lista_destruir(lista);
}

void dadaUnaListaConUnElemento_QuitarElElemento_DevuelveElementoYListaQuedaVacia(){
  lista_t* lista = lista_crear();
  void* elemento = (void*)1;
  lista_insertar(lista, elemento);
  lista_quitar(lista);
  pa2m_afirmar(lista->cantidad == 0, "La cantidad de elementos en la lista es 0.");
  pa2m_afirmar(lista->nodo_inicio == NULL, "El nodo inicio apunta a NULL");
  pa2m_afirmar(lista->nodo_fin ==NULL, "El nodo fin apunta a NULL");

  lista_destruir(lista);
}

void dadaUnaLista_QuitarUnElementoDevuelveElElementoElminado_CantidadNodosDisminuye(){
  lista_t* lista = lista_crear();
  void* elemento_uno = (void*)1;
  void* elemento_dos = (void*)2;
  void* elemento_tres = (void*)3;
  void* elemento_cuatro = (void*)4;
  lista_insertar(lista, elemento_uno);
  lista_insertar(lista, elemento_dos);
  lista_insertar(lista, elemento_tres);
  lista_insertar(lista, elemento_cuatro);

  pa2m_afirmar(lista->cantidad == 4, "La lista comienza con 4 elementos.");
  void* elemento_a_quitar_uno = elemento_cuatro;
  void* elemento_quitado_uno = lista_quitar(lista);
  pa2m_afirmar(elemento_quitado_uno == elemento_a_quitar_uno, "Se quito el elemento adecuado correctamente.");
  pa2m_afirmar((lista->cantidad == 3), "La lista ahora posee 3 elementos.");
  void* elemento_a_quitar_dos = elemento_tres;
  void* elemento_quitado_dos = lista_quitar(lista);
  pa2m_afirmar(elemento_quitado_dos == elemento_a_quitar_dos, "Se quito el elemento adecuado correctamente.");
  pa2m_afirmar((lista->cantidad == 2), "La lista ahora posee 2 elementos.");
  void* elemento_a_quitar_tres = elemento_dos;
  void* elemento_quitado_tres = lista_quitar(lista);
  pa2m_afirmar(elemento_quitado_tres == elemento_a_quitar_tres, "Se quito el elemento adecuado correctamente.");
  pa2m_afirmar((lista->cantidad == 1), "La lista ahora posee 1 elemento.");
  void* elemento_a_quitar_cuatro = elemento_uno;
  void* elemento_quitado_cuatro = lista_quitar(lista);
  pa2m_afirmar(elemento_quitado_cuatro == elemento_a_quitar_cuatro, "Se quito el elemento adecuado correctamente.");
  pa2m_afirmar((lista->cantidad == 0), "La lista ahora posee 0 elementos.");
  pa2m_afirmar(lista_quitar(lista) == NULL, "Quitar un elemento inexistente de la lista no es posible.");

  lista_destruir(lista);
}

//quitar elementos en poscion
void dadaUnaListaNula_QuitarUnElementoEnPosicionX_DevuelveNULL(){
  pa2m_afirmar(lista_quitar_de_posicion(NULL, 10) == NULL, "Quitar un elemento de posicion 10 de una lista nula retorna NULL.");
}

void dadaUnaListaVacia_QuitarUnElementoEnPosicionX_DevuelveNULL(){
  lista_t* lista = lista_crear();
  pa2m_afirmar(lista_quitar_de_posicion(lista, 5) == NULL, "Quitar un elemento de una lista vacia retorna NULL.");

  lista_destruir(lista);
}

void dadaUnaLista_QuitarElemenetosSegunPosicion_DevuelveElElementoQuitado(){
  lista_t* lista = lista_crear();
  void* elemento_uno = (void*)1;
  void* elemento_dos = (void*)2;
  void* elemento_tres = (void*)3;
  lista_insertar(lista, elemento_uno);
  lista_insertar(lista, elemento_dos);
  lista_insertar(lista, elemento_tres);

  pa2m_afirmar((lista->cantidad == 3), "La lista posee 3 elementos.");
  void* elemento_quitado_uno = lista_quitar_de_posicion(lista,1);
  pa2m_afirmar(elemento_quitado_uno == elemento_dos, "Se quito el elemento adecuado correctamente.");
  pa2m_afirmar((lista->cantidad == 2), "La lista ahora posee 2 elementos.");
  void* elemento_quitado_dos = lista_quitar_de_posicion(lista,0);
  pa2m_afirmar(elemento_quitado_dos == elemento_uno, "Se quito el elemento adecuado correctamente.");
  pa2m_afirmar((lista->cantidad == 1), "La lista ahora posee 1 elemento.");
  void* elemento_quitado_tres = lista_quitar_de_posicion(lista,0);
  pa2m_afirmar(elemento_quitado_tres == elemento_tres, "Se quito el elemento adecuado correctamente.");
  pa2m_afirmar((lista->cantidad == 0), "La lista ahora posee 0 elementos.");

  lista_destruir(lista);
}

void dadaUnaLista_QuitarElementosSegunPosicionMayor_QuitaElUltimoElemento(){
  lista_t* lista = lista_crear();
  void* elemento_uno = (void*)1;
  void* elemento_dos = (void*)2;
  void* elemento_tres = (void*)3;
  lista_insertar(lista, elemento_uno);
  lista_insertar(lista, elemento_dos);
  lista_insertar(lista, elemento_tres);

  pa2m_afirmar((lista->cantidad == 3), "La lista posee 3 elementos.");
  void* elemento_a_quitar_uno = lista_quitar_de_posicion(lista, 8);
  pa2m_afirmar(elemento_a_quitar_uno == elemento_tres, "Con una posicion mayor a la cantidad de elementos, se quita el ultimo elmento");
  pa2m_afirmar((lista->cantidad == 2), "La lista posee 2 elementos.");
  void* elemento_a_quitar_dos = lista_quitar_de_posicion(lista, 15);
  pa2m_afirmar(elemento_a_quitar_dos == elemento_dos, "Con una posicion mayor a la cantidad de elementos, se quita el ultimo elmento");
  pa2m_afirmar((lista->cantidad == 1), "La lista posee 1 elemento.");
  
  lista_destruir(lista);
}

//lista elementos posicion
void dadaUnaListaNulaYPosicion_ObtengoElementoNULL(){
  pa2m_afirmar(lista_elemento_en_posicion(NULL, (size_t)NULL) == NULL, "Buscar el elemento de una posicion nula en una lista nula devuelve NULL.");
  pa2m_afirmar(lista_elemento_en_posicion(NULL, 3) == NULL, "Buscar un elemento en lista nula en una posicion retorna NULL.");
}

void dadaUnaListaVaciaYposicion_ObtengoElementoNULL(){
  lista_t* lista  = lista_crear();
  pa2m_afirmar(lista_elemento_en_posicion(lista, 2) == NULL, "Buscar el elemento de una posicion en una lista vacia retorna NULL.");

  lista_destruir(lista);
}

void dadaUnaListaConElementosYPosicion_ObtengoElElementoBuscado(){
  lista_t* lista = lista_crear();
  void* elemento_uno = (void*)1;
  void* elemento_dos = (void*)2;
  void* elemento_tres = (void*)3;
  lista_insertar(lista, elemento_uno);
  lista_insertar(lista, elemento_dos);
  lista_insertar(lista, elemento_tres);

  pa2m_afirmar(lista_elemento_en_posicion(lista, 0) == elemento_uno, "El elemento de la primera posicion es el correspondiente.");
  pa2m_afirmar(lista_elemento_en_posicion(lista, 1) == elemento_dos, "El elemento de la segunda posicion es el correspondiente.");
  pa2m_afirmar(lista_elemento_en_posicion(lista, 2) == elemento_tres, "El elemento de la tercera posicion es el correspondiente.");

  lista_destruir(lista);
}

void dadaUnaListaConElementosYPosicionMayorACantidadDeElementos_ObtengoNULL(){
  lista_t* lista = lista_crear();
  void* elemento_uno = (void*)1;
  void* elemento_dos = (void*)2;
  lista_insertar(lista, elemento_uno);
  lista_insertar(lista, elemento_dos);

  pa2m_afirmar(lista_elemento_en_posicion(lista, 10) == NULL, "El elemento de la primera posicion es el correspondiente.");
  
  lista_destruir(lista);
}

//lista primero
void dadaUnaListaNula_ElPrimerElementoEsNULL(){
  pa2m_afirmar(lista_primero(NULL) == NULL, "El primer elemento de una lista nula es NULL.");
}

void dadaUnaListaVacia_ElPrimerElementoEsNULL(){
  lista_t* lista = lista_crear();
  pa2m_afirmar(lista_primero(lista) == NULL, "El primer elemento de una lista vacia es NULL.");

  lista_destruir(lista);
}

void dadaUnaLista_ElPrimerElementoEsElCorrespondiente(){
  lista_t* lista = lista_crear();
  void* elemento_uno = (void*)1;
  void* elemento_dos = (void*)2;
  void* elemento_tres = (void*)3;
  lista_insertar(lista, elemento_uno);
  lista_insertar(lista, elemento_dos);
  lista_insertar(lista, elemento_tres);

  pa2m_afirmar(lista_primero(lista) == elemento_uno, "El primer elemento de la lista es el correspondiente.");

  lista_destruir(lista);
}

//lista ultimo
void dadaUnaListaNula_ElUltimoElementoEsNULL(){
  pa2m_afirmar(lista_ultimo(NULL) == NULL, "El ultimo elemento de una lista nula es NULL.");
}

void dadaUnaListaVacia_ElUltimoElementoEsNULL(){
  lista_t* lista = lista_crear();
  pa2m_afirmar(lista_ultimo(lista) == NULL, "El ultimo elemento de una lista vacia es NULL.");

  lista_destruir(lista);
}

void dadaUnaLista_ElUltimorElementoEsElCorrespondiente(){
  lista_t* lista = lista_crear();
  void* elemento_uno = (void*)1;
  void* elemento_dos = (void*)2;
  void* elemento_tres = (void*)3;
  lista_insertar(lista, elemento_uno);
  lista_insertar(lista, elemento_dos);
  lista_insertar(lista, elemento_tres);

  pa2m_afirmar(lista_ultimo(lista) == elemento_tres, "El ultimo elemento de la lista es el correspondiente.");

  lista_destruir(lista);
}

//lista vacia
void dadaUnaListaNula_DevuelveTrue_LaListaSeEncuentraVacia(){
  pa2m_afirmar(lista_vacia(NULL) == true, "Una lista nula, es una lista vacia.");
}

void dadaUnaListaVacia_SeEncuentraVacia(){
  lista_t* lista = lista_crear();
   pa2m_afirmar(lista_vacia(lista) == true, "Una lista vacia, es esfectivamente una lista vacia.");
  lista_destruir(lista);
}

void dadaUnaListaConElementos_LaListaNoSeEncuentraVacia(){
  lista_t* lista = lista_crear();
  void* elemento_uno = (void*)1;
  void* elemento_dos = (void*)2;
  void* elemento_tres = (void*)3;
  lista_insertar(lista, elemento_uno);
  lista_insertar(lista, elemento_dos);
  lista_insertar(lista, elemento_tres);

  pa2m_afirmar(lista_vacia(lista) == false, "Una lista con elementos no es una lista vacia.");

  lista_destruir(lista);
}

//lista tamanio

void dadaUnaListaNula_elTamanioEscero(){
  pa2m_afirmar(lista_tamanio(NULL) == 0, "Una lista nula tiene tamanio 0.");
}

void dadaUnaListaVacia_elTamanioEscero(){
  lista_t* lista = lista_crear();
  pa2m_afirmar(lista_tamanio(lista) == 0, "Una lista vacia tiene tamanio 0.");

  lista_destruir(lista);
}

void dadaUnaListaConElementos_ElTamanioEsLaCantidadDeElementos(){
  lista_t* lista = lista_crear();
  void* elemento_uno = (void*)1;
  void* elemento_dos = (void*)2;
  void* elemento_tres = (void*)3;
  lista_insertar(lista, elemento_uno);
  lista_insertar(lista, elemento_dos);
  lista_insertar(lista, elemento_tres);

  pa2m_afirmar(lista_tamanio(lista) == 3, "Una lista con tres elementos tiene un tamanio 3.");

  lista_destruir(lista);
}

//iterador externo
void dadaUnaListaNula_cuandoIntentoCrearUnIterador_DevuelveNULL(){
  pa2m_afirmar(lista_iterador_crear(NULL) == NULL, "Cuando intento crear un iterador de una lista nula devuelve NULL.");
}

void dadaUnaListaVacia_CreoUnIterador(){
  lista_t* lista = lista_crear();
  lista_iterador_t* iterador_lista_vacia = lista_iterador_crear(lista);

  pa2m_afirmar(iterador_lista_vacia != NULL, "Puedo crear un iterador de una lista vacia.");
  pa2m_afirmar(iterador_lista_vacia->corriente == NULL,"El nodo corriente del iterador de una lista vacia es NULL." );
  pa2m_afirmar(lista_iterador_avanzar(iterador_lista_vacia) == false, "El iterador de una lista vacia no puede avanazar.");
  pa2m_afirmar(lista_iterador_tiene_siguiente(iterador_lista_vacia) == false, "El iterador de una lista vacia no tiene elemento siguiente.");
  pa2m_afirmar(lista_iterador_elemento_actual(iterador_lista_vacia) == NULL, "El elemento actual de una lista vacia es NULL.");

  lista_iterador_destruir(iterador_lista_vacia);
  lista_destruir(lista);
}


void dadaUnaListaConElementos_CreoUnIteradorExterno(){
  lista_t* lista = lista_crear();
  void* elemento_uno = (void*)1;
  void* elemento_dos = (void*)2;
  void* elemento_tres = (void*)3;
  lista_insertar(lista, elemento_uno);
  lista_insertar(lista, elemento_dos);
  lista_insertar(lista, elemento_tres);

  lista_iterador_t* iterador_lista = lista_iterador_crear(lista);
  pa2m_afirmar(iterador_lista != NULL, "Puedo crear un iterador de una lista con elementos.");
  pa2m_afirmar(lista_iterador_elemento_actual(iterador_lista) == lista->nodo_inicio->elemento,"El nodo corriente del iterador de una lista devuelve el primer elemento de una lista.");
  pa2m_afirmar(lista_iterador_avanzar(iterador_lista), "El iterador puede avanzar sobre una lista con elementos.");
  pa2m_afirmar(lista_iterador_elemento_actual(iterador_lista) == lista->nodo_inicio->siguiente->elemento,"El iterador avanza una posicion y el elemento actual es el elemento correspondiente.");
  pa2m_afirmar(lista_iterador_tiene_siguiente(iterador_lista), "En una Lista de tres elementos, el iterador puede seguir avanzando.");
  pa2m_afirmar(lista_iterador_avanzar(iterador_lista),"El iterador avanzo una posicion mas en la lista.");
  pa2m_afirmar(lista_iterador_elemento_actual(iterador_lista) == lista->nodo_inicio->siguiente->siguiente->elemento,"El iterador avanza una posicion y el elemento actual es el elemento correspondiente.");
  lista_iterador_avanzar(iterador_lista);
  pa2m_afirmar(lista_iterador_elemento_actual(iterador_lista) == NULL,"Se itero hasta la ultima posicion y al llamar la funcion de iterador_avanzar el resultado es NULL.");
  pa2m_afirmar(lista_iterador_avanzar(iterador_lista) == false, "Al ser el ultimo elemento el iterador retorna false si intenta avanzar.");

  lista_iterador_destruir(iterador_lista);
  lista_destruir(lista);
}

//iterador interno
bool si_es_cinco_es_false(void* numero, void*contexto){
  if(*(int*)numero == 5){
    return false;
  }
  (*(int*)contexto)++;
  pa2m_afirmar((*(int*)numero == *(int*)contexto), "Los elementos coinciden.");
  return true;
}

bool funcion_iterador_interno_sin_corte(void* numero, void* contexto){
  (*(int*)contexto)++;
  pa2m_afirmar((*(int*)numero == *(int*)contexto), "Los son los correspondientes.");
  return true;
}

void dadaUnaListaConElemento_CreoUnIteradorInterno(){
  lista_t* lista = lista_crear();
  int elementos[] = {1,2,3,4,5,6,7};

  for(int i = 0; i < 7; i++){
    lista_insertar(lista, &elementos[i]);
  }
  lista_iterador_t* it = lista_iterador_crear(lista);
  int contexto = 0;
  pa2m_afirmar(!lista_con_cada_elemento(lista, NULL,(void*)&contexto), "El iterador interno devuelve 0 si no hay funcion.");
  lista_con_cada_elemento(lista, si_es_cinco_es_false, (void*)&contexto);
  pa2m_afirmar(contexto == 4,"La cantidad de iteraciones que realiza el iterador interno es 5.");

  contexto = 0;
  lista_con_cada_elemento(lista, funcion_iterador_interno_sin_corte, (void*)&contexto);
  pa2m_afirmar(contexto == lista->cantidad, "La cantidad de iteraciones que realiza el iterador interno corresponde al largo de la lista.")

  lista_iterador_destruir(it);
  lista_destruir(lista);
}

/////Pruebas PILA//////
void crearUnaPila_RetornaUnaPilaVacia(){
  pila_t* pila = NULL;

  pa2m_afirmar((pila = pila_crear()), "Crear una pila devuelve un puntero a una pila.");
  pa2m_afirmar(pila_vacia(pila), "Una pila recien creada se encuentra vacia.");

  pila_destruir(pila);
}

void funcionesPilaNULL(){
  void* elemento = (void*)1;
  pa2m_afirmar(pila_apilar(NULL, NULL)==NULL, "Apilar en una pila nula un elemento inexistente, devuelve NULL.");
  pa2m_afirmar(pila_apilar(NULL, elemento) == NULL, "Apilar en una pila nula un elemento, retorna NULL.");
  pa2m_afirmar(pila_desapilar(NULL) == NULL,"Despilar en una pila nula un elemento, retorna NULL.");
  pa2m_afirmar(pila_tope(NULL) == NULL, "El tope de una pila inexistente es NULL.");
  pa2m_afirmar(pila_tamanio(NULL) == 0, "El tamanio de una pila inexistente es 0.");
  
}

void funcionesPila(){
  pila_t* pila = pila_crear();
  void* elemento_uno = (void*)1;
  void* elemento_dos = (void*)2;
  void* elemento_tres = (void*)3;

  pila_apilar(pila, elemento_uno);
  pila_apilar(pila, elemento_dos);
  pa2m_afirmar(pila_apilar(pila, elemento_tres) == pila, "Apilar elementos en una pila con tres elementos devuelve la pila.");
  void* elemento_desapilado = pila_desapilar(pila);
  pa2m_afirmar(elemento_desapilado == elemento_tres, "Desapilar elementos de una pila, devuelve el elemento desapildo.");
  pa2m_afirmar(pila_vacia(pila) == false, "Una pila con dos elementos no es una pila vacia efectivamente.");
  pa2m_afirmar(pila_tamanio(pila) == 2, "El tamanio de una pila de dos elementos es 2." );
  pa2m_afirmar(pila_tope(pila) == elemento_dos, "El tope de la pila tiene el elemento correspondiente.");
  pa2m_afirmar(pila_desapilar(pila) ==elemento_dos, "Desapilar un segundo elemento desapila el elemento correspondiente.");
   pa2m_afirmar(pila_desapilar(pila) ==elemento_uno, "Desapilar un tercer elemento desapila el elemento correspondiente.")
  pa2m_afirmar((pila_vacia(pila) && pila_tamanio(pila) == 0 && pila_tope(pila) ==NULL), "Despues de desapilar todos los elementos de una pila se encuentra vacia, su tamanio es 0 y su tope NULL.");

  pila_destruir(pila);
}

/////Pruebas COLA/////
void crearUnaCola_RetornaUnaColaVacia(){
  cola_t* cola = NULL;

  pa2m_afirmar((cola = cola_crear()), "Crear una cola devuelve un puntero a una cola.");
  pa2m_afirmar(cola_vacia(cola), "Una cola recien creada se encuentra vacia");

  cola_destruir(cola);
}

void funcionesColaNULL(){
  void* elemento = (void*)1;
  pa2m_afirmar(cola_encolar(NULL, NULL)==NULL, "Encolar en una cola nula un elemento inexistente, devuelve NULL.");
  pa2m_afirmar(cola_encolar(NULL, elemento) == NULL, "Encolar en una cola nula un elemento, retorna NULL.");
  pa2m_afirmar(cola_desencolar(NULL) == NULL,"Desencolar en una cola nula un elemento, retorna NULL.");
  pa2m_afirmar(cola_frente(NULL) == NULL, "El frente de una cola inexistente es NULL.");
  pa2m_afirmar(cola_tamanio(NULL) == 0, "El tamanio de una cola inexistente es 0.");

}

void funcionesCola(){
  cola_t* cola = cola_crear();
  void* elemento_uno = (void*)1;
  void* elemento_dos = (void*)2;
  void* elemento_tres = (void*)3;
  cola_encolar(cola, elemento_uno);
  cola_encolar(cola, elemento_dos);

  pa2m_afirmar(cola_encolar(cola, elemento_tres) == cola, "Encolar elementos en una cola devuelve la cola.");
  pa2m_afirmar(cola_desencolar(cola) == elemento_uno, "Desencolar elementos de una cola devuelve el elemento desencolado.");
  pa2m_afirmar(cola_vacia(cola) == false, "Una cola con dos elementos no es una cola vacia efectivamente.");
  pa2m_afirmar(cola_tamanio(cola) == 2, "El tamanio de una cola de dos elementos es 2." );
  pa2m_afirmar(cola_frente(cola) == elemento_dos, "El frente de la cola tiene el elemento correspondiente.");
  cola_desencolar(cola);
  cola_desencolar(cola);
  pa2m_afirmar((cola_vacia(cola) && cola_tamanio(cola) == 0 && cola_frente(cola) ==NULL), "Despues de desencolar todos los elementos de una cola se encuentra vacia, su tamanio es 0 y su frente NULL.")

  cola_destruir(cola);
}

int main() {
  //LISTA
  pa2m_nuevo_grupo("Pruebas Con Lista NULL");
  dadaUnaListaNula_InsertoElementosAlaLista_ObtengoNull();
  dadaUnaListaNulaElementoYPosicion_InsertoElementosALaLista_ObtengoNull();
  dadaUnaListaNula_QuitarUnElementoDevuelveNULL();
  dadaUnaListaNula_QuitarUnElementoEnPosicionX_DevuelveNULL();
  dadaUnaListaNulaYPosicion_ObtengoElementoNULL();
  dadaUnaListaNula_ElPrimerElementoEsNULL();
  dadaUnaListaNula_ElUltimoElementoEsNULL();
  dadaUnaListaNula_DevuelveTrue_LaListaSeEncuentraVacia();
  dadaUnaListaNula_elTamanioEscero();
  dadaUnaListaNula_cuandoIntentoCrearUnIterador_DevuelveNULL();

  pa2m_nuevo_grupo("Crear Lista");
  CrearUnaLista_CreaListaVacia();

  pa2m_nuevo_grupo("Insertar Elementos a Lista");
  dadaUnaListaVacia_InstertoElementosALaLista_ObtengoListaConElementosAumentados();

  pa2m_nuevo_grupo("Insertar Elementos en una Posicion a la Lista");
  dadaUnaListaVaciaUnElementoYPosicion_InsertoElementosEnPosicion_ObtengoListaConElementosAumentados();
  dadaUnaListaVaciaUnElementoYPosicion0_InsertoElementosAlPrincipioDeLaLista();
  dadaUnaListaVaciaUnElementoYPosicionMayorA1_InsertoElementoEnLaUltimaPosicion();
  dadaUnaListaUnElementoYPosicion_InsertoCuatroElementos_ObtengoListaDeNodosEnlazados();

  pa2m_nuevo_grupo("Quitar elementos de la lista");
  dadaUnaListaVacia_QuitarUnElementoDevuelveNULL();
  dadaUnaListaConUnElemento_QuitarElElemento_DevuelveElementoYListaQuedaVacia();
  dadaUnaLista_QuitarUnElementoDevuelveElElementoElminado_CantidadNodosDisminuye();

  pa2m_nuevo_grupo("Quitar elementos de una posicion de la lista");
  dadaUnaListaVacia_QuitarUnElementoEnPosicionX_DevuelveNULL();
  dadaUnaLista_QuitarElemenetosSegunPosicion_DevuelveElElementoQuitado();
  dadaUnaLista_QuitarElementosSegunPosicionMayor_QuitaElUltimoElemento();

  pa2m_nuevo_grupo("Consultar elementos en posicion");
  dadaUnaListaVaciaYposicion_ObtengoElementoNULL();
  dadaUnaListaConElementosYPosicion_ObtengoElElementoBuscado();
  dadaUnaListaConElementosYPosicionMayorACantidadDeElementos_ObtengoNULL();

  pa2m_nuevo_grupo("Primer elemento de la lista");
  dadaUnaListaVacia_ElPrimerElementoEsNULL();
  dadaUnaLista_ElPrimerElementoEsElCorrespondiente();

  pa2m_nuevo_grupo("Ultimo elemento de la lista");
  dadaUnaListaVacia_ElUltimoElementoEsNULL();
  dadaUnaLista_ElUltimorElementoEsElCorrespondiente();

  pa2m_nuevo_grupo("Lista vacia");
  dadaUnaListaVacia_SeEncuentraVacia();
  dadaUnaListaConElementos_LaListaNoSeEncuentraVacia();

  pa2m_nuevo_grupo("Lista tamanio");
  dadaUnaListaVacia_elTamanioEscero();
  dadaUnaListaConElementos_ElTamanioEsLaCantidadDeElementos();

  pa2m_nuevo_grupo("Iterador Lista");
  dadaUnaListaVacia_CreoUnIterador();
  dadaUnaListaConElementos_CreoUnIteradorExterno();
  dadaUnaListaConElemento_CreoUnIteradorInterno();

  //PILA
  pa2m_nuevo_grupo("Crear Pila");
  crearUnaPila_RetornaUnaPilaVacia();
  funcionesPilaNULL();
  funcionesPila();

  //COLA
  pa2m_nuevo_grupo("Crear Cola");
  crearUnaCola_RetornaUnaColaVacia();
  funcionesColaNULL();
  funcionesCola();

  return pa2m_mostrar_reporte();
}
