#include "src/abb.h"
#include <stdio.h>
#include "pa2mm.h"
#include "string.h"

typedef struct cosa{
  int clave;
  char contenido[10];
}cosa_t;

cosa_t* crear_cosa(int clave){
  cosa_t* cosa = (cosa_t*)malloc(sizeof(cosa_t));
  if(cosa)
    cosa->clave = clave;
  return cosa;
}

int comparador(void* elemento_uno, void* elemento_dos){
  if(!elemento_uno || !elemento_dos) return 0;
  cosa_t* uno = elemento_uno;
  cosa_t* dos = elemento_dos;
  return uno->clave - dos->clave;
}

void destruir_cosa(cosa_t* cosa){
  free(cosa);
}

void destructor(void* c){
  cosa_t* cosa = c;
  if(cosa)
  destruir_cosa(c);
}

//PRUEBAS CON NULL
void pruebasArbolNULL(){
  pa2m_afirmar(abb_crear(NULL) == NULL, "Al intentar crear un abb con funcion comparador nula, retorna NULL.");
  void* elemento = (void*)1;
  pa2m_afirmar(abb_insertar(NULL, elemento) ==NULL, "Insertar un elemento en un abb nulo retorna NULL.")
  pa2m_afirmar(abb_quitar(NULL, elemento)==NULL, "Quitar un elemento de un abb nulo retorna NULL.");
  pa2m_afirmar(abb_buscar(NULL, elemento)==NULL, "Buscar un elemento de un abb nulo retorna NULL.");
  pa2m_afirmar(abb_vacio(NULL)== true, "Un arbol nulo esta vacio.");
  pa2m_afirmar(abb_tamanio(NULL) == 0,"El tamanio de un abb nulo es 0.");
  pa2m_afirmar(abb_con_cada_elemento(NULL, INORDEN, false, NULL)== 0, "Recorrer los elementos (iterador interno) INORDEN de un arbol nulo retorna 0.");
  pa2m_afirmar(abb_con_cada_elemento(NULL, POSTORDEN, false, NULL)== 0, "Recorrer los elementos (iterador interno) POSTORDEN de un arbol nulo retorna 0.");
  pa2m_afirmar(abb_con_cada_elemento(NULL, PREORDEN, false, NULL)== 0, "Recorrer los elementos (iterador interno) PREORDEN de un arbol nulo retorna 0.");
  pa2m_afirmar(abb_recorrer(NULL, INORDEN, NULL, 0)== 0,"Recorrer los elementos con recorrido INORDEN de un arbol nulo retorna 0 elementos recorridos.");
  pa2m_afirmar(abb_recorrer(NULL, PREORDEN, NULL, 0)== 0,"Recorrer los elementos con recorrido PREORDEN de un arbol nulo retorna 0 elementos recorridos.");
  pa2m_afirmar(abb_recorrer(NULL, POSTORDEN, NULL, 0)== 0,"Recorrer los elementos con recorrido POSTORDEN de un arbol nulo retorna 0 elementos recorridos.");
}

//creacion
void seCreaUnArbol_Exitosamente(){
  abb_t* arbol = abb_crear(comparador);
  pa2m_afirmar(arbol != NULL, "Se crea un arbol correctamente.");
  pa2m_afirmar(arbol->comparador == comparador, "El comparador del arbol es el correspondiente.");
  pa2m_afirmar(arbol->tamanio == 0 , "El tamanio del arbol es 0.")
  pa2m_afirmar(abb_vacio(arbol), "El arbol recien creado sin elementos esta vacio.");
  pa2m_afirmar(arbol->nodo_raiz == NULL, "El nodo raiz de un abb recien creado apunta a NULL.");
  abb_destruir(arbol);
}

//insercion 
void seInsertaUnElementoEnArbol_ObtengoArbolConUnElemento(){
  abb_t* arbol = abb_crear(comparador);
  cosa_t* cosa = crear_cosa(1);
  pa2m_afirmar(arbol != NULL, "Se crea un arbol correctamente.");
  pa2m_afirmar(abb_insertar(arbol, cosa), "Se inserta un elemento en el arbol correctamente.");
  pa2m_afirmar(!abb_vacio(arbol), "El arbol no se encuentra vacio.");
  pa2m_afirmar(abb_tamanio(arbol) == 1, "El tamanio del arbol es el correcto.");
  pa2m_afirmar(arbol->nodo_raiz->elemento == cosa, "El elemento insertado es el correcto y se encuentra en la posicion correspondiente.");
  abb_destruir_todo(arbol, destructor);
}

void seInsertanElementosEnArbol_ObtengoArbolConElementos(){
  abb_t* arbol = abb_crear(comparador);
  cosa_t* cosa_uno = crear_cosa(5);
  cosa_t* cosa_dos = crear_cosa(2);
  cosa_t* cosa_tres = crear_cosa(7);
  cosa_t* cosa_cuatro = crear_cosa(1);
  cosa_t* auxiliar = crear_cosa(0);
  pa2m_afirmar(arbol!= NULL, "Se crea un arbol exitosamente.");
  pa2m_afirmar(arbol->nodo_raiz == NULL, "El nodo raiz de arbol recien creado apunta a null.");
  pa2m_afirmar(abb_vacio(arbol), "Un arbol recien creado se encuentra vacio.");
  pa2m_afirmar(abb_insertar(arbol, cosa_uno), "Se inserta un elemento al arbol exitosamente.");
  pa2m_afirmar(arbol->nodo_raiz->elemento == cosa_uno, "Al insertar un elemento en un arbol vacio, se inserta en el nodo raiz.");
  pa2m_afirmar(arbol->tamanio == 1, "El tamanio del arbol es el correspondiente.");
  pa2m_afirmar(abb_insertar(arbol, cosa_dos), "Se puede insertar un segundo elemento en el arbol.");
  pa2m_afirmar(arbol->nodo_raiz->izquierda->elemento == cosa_dos, "Se inserta un segundo elemento en el lugar correspondiente.");
  pa2m_afirmar(abb_tamanio(arbol) == 2, "El tamanio del arbol es el correspondiente.");
  pa2m_afirmar(abb_insertar(arbol, cosa_tres), "Se puede insertar un tercer elemento correctamente.");
  pa2m_afirmar(arbol->nodo_raiz->derecha->elemento == cosa_tres, "Se inserta un segundo elemento en el lugar correspondiente.");
  pa2m_afirmar(abb_tamanio(arbol) == 3, "El tamanio del arbol es el correspondiente.");
  pa2m_afirmar(!abb_vacio(arbol),"Un arbol con tres elmentos no se encuentra vacio.");
  pa2m_afirmar(abb_insertar(arbol, NULL), "Se puede insertar un elemento nulo al arbol.");
  pa2m_afirmar(arbol->nodo_raiz->izquierda->izquierda->elemento == NULL, "El elemento es el correspondiente.")
  auxiliar->clave = 0;
  pa2m_afirmar(abb_buscar(arbol, auxiliar) == NULL, "Al buscar el elemento nulo, retorna NULL.");
  pa2m_afirmar(abb_tamanio(arbol) == 4, "El tamanio del arbol es el correspondiente.");
  pa2m_afirmar(abb_insertar(arbol, cosa_cuatro) ,"Se puede insertar un cuarto elemento.");
  pa2m_afirmar(abb_tamanio(arbol) == 5, "El tamanio del arbol es el correspondiente." );
  pa2m_afirmar(arbol->nodo_raiz->izquierda->izquierda->izquierda->elemento == cosa_cuatro, "El elemento insertado es el correspondiente.");
  destruir_cosa(auxiliar);
  abb_destruir_todo(arbol, destructor);
}

//quitar & buscar
void seQuitanElementosDeUnArbolExitosamente(){
  abb_t* abb = abb_crear(comparador);
  cosa_t* cosa_uno = crear_cosa(10);
  cosa_t* cosa_dos = crear_cosa(5);
  cosa_t* cosa_tres = crear_cosa(20);
  cosa_t* cosa_cuatro = crear_cosa(3);
  cosa_t* cosa_cinco = crear_cosa(7);
  cosa_t* cosa_seis = crear_cosa(17);
  cosa_t* cosa_siete = crear_cosa(25);
  cosa_t* auxiliar = crear_cosa(0);
  abb_insertar(abb, cosa_uno);
  abb_insertar(abb, cosa_dos);
  abb_insertar(abb, cosa_tres);
  abb_insertar(abb, cosa_cuatro);
  abb_insertar(abb, cosa_cinco);
  abb_insertar(abb, cosa_seis);
  abb_insertar(abb, cosa_siete);

  pa2m_afirmar(abb!= NULL, "Se crea un nuevo arbol exitosamente.");
  pa2m_afirmar(abb_tamanio(abb) == 7, "El tamanio del arbol con 8 elementos es el correspondiente.");
  auxiliar->clave = 25;
  pa2m_afirmar(abb_buscar(abb, auxiliar) == cosa_siete, "Al buscar un elemento, retorna el correspondiente.");
  pa2m_afirmar(abb_quitar(abb, auxiliar) == cosa_siete, "Cuando se quita un nodo hoja el elemento quitado es el correspondiente.");
  pa2m_afirmar(abb_buscar(abb, auxiliar) == NULL, "Al buscar un quitado, retorna NULL.");
  auxiliar->clave = 5;
   pa2m_afirmar(abb_tamanio(abb) == 6, "El tamanio del arbol ahora es 6.");
  pa2m_afirmar(abb_quitar(abb, auxiliar) == cosa_dos, "Cuando se quita un nodo con dos hijos, el elemento quitado es el correspondiente.");
  pa2m_afirmar(abb_tamanio(abb) == 5, "El tamanio del arbol ahora es 5.");
  auxiliar->clave = 20;
  pa2m_afirmar(abb_buscar(abb, auxiliar) == cosa_tres, "Al buscar un elemento, retorna el correspondiente.");
  pa2m_afirmar(abb_quitar(abb, auxiliar) == cosa_tres, "Se puede quitar un elemento que tiene dos nodos hijos.");
  pa2m_afirmar(abb_tamanio(abb)== 4, "El tamanio luego de quitar un elemento es el correspondiente.");

  destruir_cosa(cosa_dos);
  destruir_cosa(cosa_siete);
  destruir_cosa(cosa_tres);
  destruir_cosa(auxiliar);
  abb_destruir_todo(abb, destructor);
}

void seBuscanElmentosDentroDeUnArbol_ObtengoElementosCorrespondientes(){
abb_t* abb = abb_crear(comparador);
  cosa_t* cosa_uno = crear_cosa(10);
  cosa_t* cosa_dos = crear_cosa(5);
  cosa_t* cosa_tres = crear_cosa(20);
  cosa_t* cosa_cuatro = crear_cosa(3);
  cosa_t* cosa_cinco = crear_cosa(7);
  cosa_t* cosa_seis = crear_cosa(17);
  cosa_t* cosa_siete = crear_cosa(25);
  cosa_t* cosa_inexistente = crear_cosa(23);
  abb_insertar(abb, cosa_uno);
  abb_insertar(abb, cosa_dos);
  abb_insertar(abb, cosa_tres);
  abb_insertar(abb, cosa_cuatro);
  abb_insertar(abb, cosa_cinco);
  abb_insertar(abb, cosa_seis);
  abb_insertar(abb, cosa_siete);

  pa2m_afirmar(abb!=NULL, "Se crea un arbol distinto correctamente.");
  pa2m_afirmar(abb_tamanio(abb) == 7, "El tamanio del arbol es el correspondiente.");
  pa2m_afirmar(abb_buscar(abb, cosa_seis) == cosa_seis, "Buscar el elemento 17, retorna 17.");
  pa2m_afirmar(abb_buscar(abb, abb->nodo_raiz->elemento) == cosa_uno, "Buscar el elemento de un nodo raiz devuelve el elemnto correspondiente.");
  pa2m_afirmar(abb_buscar(abb, cosa_inexistente) == NULL, "Al buscar un elemento que no existe dentro del arbol, no lo encuentra.");
  
  destruir_cosa(cosa_inexistente);
  abb_destruir_todo(abb, destructor);
}

//con_cada_elemento

//siempre devuelve true.
bool es_true(){
  return true;
}


void pruebasRecorridoConCadaElementoFuncionTrue_RetornanLosTresRecorridosIgualCantidad(){
  abb_t* abb = abb_crear(comparador);
  cosa_t* cosa_uno = crear_cosa(8);
  cosa_t* cosa_dos = crear_cosa(4);
  cosa_t* cosa_tres = crear_cosa(2);
  cosa_t* cosa_cuatro = crear_cosa(15);
  cosa_t* cosa_cinco = crear_cosa(14);
  abb_insertar(abb, cosa_uno);
  abb_insertar(abb, cosa_dos);
  abb_insertar(abb, cosa_tres);
  abb_insertar(abb, cosa_cuatro);
  abb_insertar(abb, cosa_cinco);
  size_t cantidad_postorden = abb_con_cada_elemento(abb, POSTORDEN, es_true, NULL);
  size_t cantidad_preorden = abb_con_cada_elemento(abb, PREORDEN, es_true, NULL);
  size_t cantidad_inorden = abb_con_cada_elemento(abb, INORDEN, es_true, NULL);
  pa2m_afirmar(abb_tamanio(abb) == 5, "El tamanio del arbol es el correspondiente.");
  pa2m_afirmar(cantidad_inorden == 5, "La cantidad recorrida con el recorrido inorden con funcion true es correcta.");
  pa2m_afirmar(cantidad_preorden == 5, "La cantidad recorrida con el recorrido preorden con funcion true es correcta.");
  pa2m_afirmar(cantidad_postorden == 5, "La cantidad recorrida con el recorrido postorden con funcion true es correcta.");

  abb_destruir_todo(abb, destructor);
}

//Devuelve false si el elemento es igual a 4.
bool hasta_cuatro(void* elemento, void* aux){
  aux = aux;
  if(elemento){
    if(((cosa_t*)elemento)->clave == 4){
      return false;
    }
  }
  return true;
}

//Devuelve false si el elemento es igual a 10.
bool hasta_diez(void* elemento, void* aux){
  aux = aux;
  if(elemento){
    if(((cosa_t*)elemento)->clave == 10)
      return false;
  }
  return true;
}

void pruebasRecorridoConCadaElemento(){
  abb_t* abb = abb_crear(comparador);
  cosa_t* cosa_uno = crear_cosa(8);
  cosa_t* cosa_dos = crear_cosa(4);
  cosa_t* cosa_tres = crear_cosa(2);
  cosa_t* cosa_cuatro = crear_cosa(15);
  cosa_t* cosa_cinco = crear_cosa(10);
  cosa_t* cosa_seis = crear_cosa(6);
  abb_insertar(abb, cosa_uno);
  abb_insertar(abb, cosa_dos);
  abb_insertar(abb, cosa_tres);
  abb_insertar(abb, cosa_cuatro);
  abb_insertar(abb, cosa_cinco);
  abb_insertar(abb, cosa_seis);


  pa2m_afirmar(abb_con_cada_elemento(abb, INORDEN, hasta_cuatro, NULL) == 2, "La funcion dejo de recorrer correctamente para el recorrido inorden.");
  pa2m_afirmar(abb_con_cada_elemento(abb, PREORDEN, hasta_cuatro, NULL) == 2, "La funcion dejo de recorrer correctamente para el recorrido preorden.");
  pa2m_afirmar(abb_con_cada_elemento(abb, POSTORDEN, hasta_cuatro, NULL) == 3, "La funcion dejo de recorrer correctamente para el recorrido postorden.");
  pa2m_afirmar(abb_con_cada_elemento(abb, INORDEN, hasta_diez, NULL) == 5, "La funcion dejo de recorrer correctamente para el recorrido inorden.");
  pa2m_afirmar(abb_con_cada_elemento(abb, PREORDEN, hasta_diez, NULL) == 6, "La funcion dejo de recorrer correctamente para el recorrido preorden.");
  pa2m_afirmar(abb_con_cada_elemento(abb, POSTORDEN, hasta_diez, NULL) == 4, "La funcion dejo de recorrer correctamente para el recorrido postorden.");

  abb_destruir_todo(abb, destructor);
} 


//recorrer 
//inorden
void seRecorreUnArbolVacio_ObtengoCeroElementos(){
  abb_t* abb = abb_crear(comparador);
  size_t tamanio_array = 0;
  void* array[tamanio_array];
  size_t cantidad_recorrida_inorden = abb_recorrer(abb, INORDEN, array, tamanio_array);
  size_t cantidad_recorrida_preorden = abb_recorrer(abb, PREORDEN, array, tamanio_array);
  size_t cantidad_recorrida_postorden = abb_recorrer(abb, POSTORDEN, array, tamanio_array);
  pa2m_afirmar(cantidad_recorrida_inorden == 0, "Al recorrer un arbol vacio con recorrido INORDEN obtendo 0 elementos recorridos.");
  pa2m_afirmar(cantidad_recorrida_preorden == 0, "Al recorrer un arbol vacio con recorrido PREORDEN obtendo 0 elementos recorridos.");
  pa2m_afirmar(cantidad_recorrida_postorden == 0, "Al recorrer un arbol vacio con recorrido POSTORDEN obtendo 0 elementos recorridos.");

  abb_destruir(abb);
}

void seRecorreUnArbolInordenConElementos_ObtengoLaCantidadDeElementosCorrespondientes(){
  abb_t* abb = abb_crear(comparador);
  size_t tamanio_array = 6;
  void* array[tamanio_array];
  cosa_t* diez = crear_cosa(10);
  cosa_t* siete = crear_cosa(7);
  cosa_t* cuatro = crear_cosa(4);
  cosa_t* nueve = crear_cosa(9);
  cosa_t* diecisiete = crear_cosa(17);
  cosa_t* quince = crear_cosa(15);
  abb_insertar(abb, diez);
  abb_insertar(abb, siete);
  abb_insertar(abb, cuatro);
  abb_insertar(abb, nueve);
  abb_insertar(abb, diecisiete);
  abb_insertar(abb, quince);
  
  size_t recorridos_inorden = abb_recorrer(abb, INORDEN, array, tamanio_array);
  pa2m_afirmar(recorridos_inorden == tamanio_array, "Recorrer el arbol INORDEN retorna la cantidad de elementos recorridos correspondiente.");
  pa2m_afirmar((cosa_t*)(array[0])== cuatro, "El primer elemento del array es el correspondiente.");
  pa2m_afirmar((cosa_t*)(array[1])== siete, "El segundo elemento del array es el correspondiente.");
  pa2m_afirmar((cosa_t*)(array[2])== nueve, "El tercero elemento del array es el correspondiente.");
  pa2m_afirmar((cosa_t*)(array[3])== diez, "El cuarto elemento del array es el correspondiente.");
  pa2m_afirmar((cosa_t*)(array[4])== quince, "El quinto elemento del array es el correspondiente.");
  pa2m_afirmar((cosa_t*)(array[5])== diecisiete, "El sexto elemento del array es el correspondiente.");
  abb_destruir_todo(abb, destructor);

}

void seRecorreUnArbolPreordenConElementos_ObtengoLaCantidadDeElementosCorrespondientes(){
  abb_t* abb = abb_crear(comparador);
  size_t tamanio_array = 6;
  void* array[tamanio_array];
  cosa_t* diez = crear_cosa(10);
  cosa_t* siete = crear_cosa(7);
  cosa_t* cuatro = crear_cosa(4);
  cosa_t* nueve = crear_cosa(9);
  cosa_t* diecisiete = crear_cosa(17);
  cosa_t* quince = crear_cosa(15);
  abb_insertar(abb, diez);
  abb_insertar(abb, siete);
  abb_insertar(abb, cuatro);
  abb_insertar(abb, nueve);
  abb_insertar(abb, diecisiete);
  abb_insertar(abb, quince);
  
  size_t recorridos_inorden = abb_recorrer(abb, PREORDEN, array, tamanio_array);
  pa2m_afirmar(recorridos_inorden == tamanio_array, "Recorrer el arbol PREORDEN retorna la cantidad de elementos recorridos correspondiente.");
  pa2m_afirmar((cosa_t*)(array[0])== diez, "El primer elemento del array es el correspondiente.");
  pa2m_afirmar((cosa_t*)(array[1])== siete, "El segundo elemento del array es el correspondiente.");
  pa2m_afirmar((cosa_t*)(array[2])== cuatro, "El tercero elemento del array es el correspondiente.");
  pa2m_afirmar((cosa_t*)(array[3])== nueve, "El cuarto elemento del array es el correspondiente.");
  pa2m_afirmar((cosa_t*)(array[4])== diecisiete, "El quinto elemento del array es el correspondiente.");
  pa2m_afirmar((cosa_t*)(array[5])== quince, "El sexto elemento del array es el correspondiente.");
  abb_destruir_todo(abb, destructor);

}

void seRecorreUnArbolPostordenConElementos_ObtengoLaCantidadDeElementosCorrespondientes(){
  abb_t* abb = abb_crear(comparador);
  size_t tamanio_array = 6;
  void* array[tamanio_array];
  cosa_t* diez = crear_cosa(10);
  cosa_t* siete = crear_cosa(7);
  cosa_t* cuatro = crear_cosa(4);
  cosa_t* nueve = crear_cosa(9);
  cosa_t* diecisiete = crear_cosa(17);
  cosa_t* quince = crear_cosa(15);
  abb_insertar(abb, diez);
  abb_insertar(abb, siete);
  abb_insertar(abb, cuatro);
  abb_insertar(abb, nueve);
  abb_insertar(abb, diecisiete);
  abb_insertar(abb, quince);
  
  size_t recorridos_inorden = abb_recorrer(abb, POSTORDEN, array, tamanio_array);
  pa2m_afirmar(recorridos_inorden == tamanio_array, "Recorrer el arbol PREORDEN retorna la cantidad de elementos recorridos correspondiente.");
  pa2m_afirmar((cosa_t*)(array[0])== cuatro, "El primer elemento del array es el correspondiente.");
  pa2m_afirmar((cosa_t*)(array[1])== nueve, "El segundo elemento del array es el correspondiente.");
  pa2m_afirmar((cosa_t*)(array[2])== siete, "El tercero elemento del array es el correspondiente.");
  pa2m_afirmar((cosa_t*)(array[3])== quince, "El cuarto elemento del array es el correspondiente.");
  pa2m_afirmar((cosa_t*)(array[4])== diecisiete, "El quinto elemento del array es el correspondiente.");
  pa2m_afirmar((cosa_t*)(array[5])== diez, "El sexto elemento del array es el correspondiente.");
  abb_destruir_todo(abb, destructor);

}

int main(){
  pa2m_nuevo_grupo("= PRUEBAS ABB =");
  pa2m_nuevo_grupo("Pruebas NULL");
  pruebasArbolNULL();

  pa2m_nuevo_grupo("Pruebas creacion");
  seCreaUnArbol_Exitosamente();

  pa2m_nuevo_grupo("Pruebas de insercion de un elmento");
  seInsertaUnElementoEnArbol_ObtengoArbolConUnElemento();
  pa2m_nuevo_grupo("Pruebas de insercion de tres elmentos");
  seInsertanElementosEnArbol_ObtengoArbolConElementos();

  pa2m_nuevo_grupo("Pruebas de quitar elementos");
  seQuitanElementosDeUnArbolExitosamente();

  pa2m_nuevo_grupo("Pruebas de busqueda de elementos");
  seBuscanElmentosDentroDeUnArbol_ObtengoElementosCorrespondientes();

  pa2m_nuevo_grupo("Pruebas de abb con cada elemento");
  pruebasRecorridoConCadaElementoFuncionTrue_RetornanLosTresRecorridosIgualCantidad();
  pruebasRecorridoConCadaElemento();

  pa2m_nuevo_grupo("Pruebas de abb recorrer");
  seRecorreUnArbolVacio_ObtengoCeroElementos();
  seRecorreUnArbolInordenConElementos_ObtengoLaCantidadDeElementosCorrespondientes();
  seRecorreUnArbolPreordenConElementos_ObtengoLaCantidadDeElementosCorrespondientes();
  seRecorreUnArbolPostordenConElementos_ObtengoLaCantidadDeElementosCorrespondientes();

  return pa2m_mostrar_reporte();
}
