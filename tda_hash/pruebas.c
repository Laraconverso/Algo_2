#include "pa2mm.h"
#include "src/hash.h"

#define EXITOSO 0
#define ERROR -1

//NULL
void dadoHashNull_prueboLasPrimitivas_obtengoResultadoEsperado(){
    hash_t* hash_nulo = NULL;
    pa2m_afirmar(hash_insertar(hash_nulo, "ABC", (void*)34)== ERROR, "No se pueden insertar elementos en hash nulo.");
    pa2m_afirmar(hash_quitar(hash_nulo, "ABC") == ERROR, "No se pueden quitar elementos de un hash nulo.");
    pa2m_afirmar(hash_obtener(hash_nulo, "ABC") == NULL, "No se pueden obtener los elementos dado un hash nulo.");
    pa2m_afirmar(hash_cantidad(hash_nulo) == 0, "La cantidad de elementos almacencados de un hash nulo es 0.");
    pa2m_afirmar(hash_con_cada_clave(hash_nulo, false,NULL)== 0, "La canitdad de elementos recorridos con el iterador interno de un hash nulo es 0.");
}

//CREACION
void dadaCapacidadInicialValidaYDestructorNulo_seCreaUnHashCorrectamente(){
    hash_t* hash_uno = hash_crear(NULL, 1);
    pa2m_afirmar(hash_uno, "Crear un hash con capacidad menor a la minima y destructor nulo crea un hash correctamente.");
    hash_t* hash_quince = hash_crear(NULL, 15);
    pa2m_afirmar(hash_quince, "Crear un hash con capacidad valida y destructor nulo crea un hash correctamente.");
    hash_destruir(hash_uno);
    hash_destruir(hash_quince);
}

//INSERTAR
void dadoValoresValidos_seInsertanCorrectamenteLosElementos(){
    hash_t* hash = hash_crear(NULL, 7);
    pa2m_afirmar(hash != NULL, "Se crea un hash correctamente");
    pa2m_afirmar(hash_cantidad(hash) == 0,"Un  hash recien creado tiene 0 elementos.");
    pa2m_afirmar(hash_insertar(hash, "ABCD", (void*)3)== EXITOSO, "Se inserta correctamente un elemento.");
    pa2m_afirmar(hash_cantidad(hash)== 1, "La cantidad de elementos del hash es 1.");
    pa2m_afirmar(hash_insertar(hash, "EFGH", (void*)1)== EXITOSO, "Se inserta correctamente un elemento.");
    pa2m_afirmar(hash_cantidad(hash)== 2, "La cantidad de elementos del hash es 2.");
    pa2m_afirmar(hash_insertar(hash, "IJKL", (void*)5)== EXITOSO, "Se inserta correctamente un elemento.");
    pa2m_afirmar(hash_cantidad(hash)== 3, "La cantidad de elementos del hash es 3.");

    printf("\n");
    hash_destruir(hash);
}

void dadoValoresValidos_seInsertanElementosEnHashConMenorCapacidad(){
    hash_t* hash = hash_crear(NULL, 3);
    pa2m_afirmar(hash!= NULL, "Se crea un hash nuevo correctamente.");
    pa2m_afirmar(hash_insertar(hash, "HOLA", (void*)3)== EXITOSO, "Se inserta correctamente un elemento[1].");
    pa2m_afirmar(hash_insertar(hash, "ESTO", (void*)2)== EXITOSO, "Se inserta correctamente un elemento[2].");
    pa2m_afirmar(hash_insertar(hash, "ES", (void*)6)== EXITOSO, "Se inserta correctamente un elemento[3].");
    pa2m_afirmar(hash_insertar(hash, "UNA", (void*)7)== EXITOSO, "Se inserta correctamente un elemento[4].");
    pa2m_afirmar(hash_insertar(hash, "PRUEBA", (void*)3)== EXITOSO, "Se inserta correctamente un elemento[5].");
    pa2m_afirmar(hash_insertar(hash, "DEL", (void*)1)== EXITOSO, "Se inserta correctamente un elemento[6].");
    pa2m_afirmar(hash_insertar(hash, "HASH", (void*)4)== EXITOSO, "Se inserta correctamente un elemento[7].");
    pa2m_afirmar(hash_insertar(hash, "INSERTAR", (void*)8)== EXITOSO, "Se inserta correctamente un elemento[8].");
    pa2m_afirmar(hash_cantidad(hash)== 8, "La cantidad de elementos en el hash es 8.");

    hash_destruir(hash);
}

//QUITAR
void dadoUnHash_seQuitanElementos_obtengoResultadosEsperados(){
    hash_t* hash = hash_crear(NULL, 10);
    pa2m_afirmar(hash != NULL, "Se crea correctamente un hash");
    pa2m_afirmar(hash_insertar(hash, "Prueba1", (void*)3)== EXITOSO, "Se inserta correctamente un elemento[1].");
    pa2m_afirmar(hash_insertar(hash, "Prueba2", (void*)2)== EXITOSO, "Se inserta correctamente un elemento[2].");
    pa2m_afirmar(hash_insertar(hash, "Prueba3", (void*)6)== EXITOSO, "Se inserta correctamente un elemento[3].");
    pa2m_afirmar(hash_insertar(hash, "Prueba4", (void*)7)== EXITOSO, "Se inserta correctamente un elemento[4].");
    pa2m_afirmar(hash_insertar(hash, "Prueba5", (void*)10)== EXITOSO, "Se inserta correctamente un elemento[5].");
    pa2m_afirmar(hash_insertar(hash, "Prueba6", (void*)1)== EXITOSO, "Se inserta correctamente un elemento[6].");
    pa2m_afirmar(hash_insertar(hash, "Prueba7", (void*)4)== EXITOSO, "Se inserta correctamente un elemento[7].");
    pa2m_afirmar(hash_insertar(hash, "Prueba8", (void*)8)== EXITOSO, "Se inserta correctamente un elemento[8].");
    pa2m_afirmar(hash_cantidad(hash)== 8, "La cantidad de elementos en el hash es 8.");
    pa2m_afirmar(hash_quitar(hash, "Prueba1") == EXITOSO, "Se quita correctamente un elemento del hash.");
    pa2m_afirmar(hash_cantidad(hash)== 7, "La cantidad de elementos en el hash es 7.");
    pa2m_afirmar(hash_quitar(hash, "Prueba2") == EXITOSO, "Se quita correctamente un elemento del hash.");
    pa2m_afirmar(hash_cantidad(hash)== 6, "La cantidad de elementos en el hash es 6.");
    pa2m_afirmar(hash_quitar(hash, "Prueba5") == EXITOSO, "Se quita correctamente un elemento del hash.");
    pa2m_afirmar(hash_cantidad(hash)== 5, "La cantidad de elementos en el hash es 5.");
    pa2m_afirmar(hash_quitar(hash, "Prueba8") == EXITOSO, "Se quita correctamente un elemento del hash.");
    pa2m_afirmar(hash_cantidad(hash)== 4, "La cantidad de elementos en el hash es 4.");
    pa2m_afirmar(hash_quitar(hash, "Prueba3") == EXITOSO, "Se quita correctamente un elemento del hash.");
    pa2m_afirmar(hash_cantidad(hash)== 3, "La cantidad de elementos en el hash es 3.");
    pa2m_afirmar(hash_quitar(hash, "Prueba7") == EXITOSO, "Se quita correctamente un elemento del hash.");
    pa2m_afirmar(hash_cantidad(hash)== 2, "La cantidad de elementos en el hash es 2.");
    pa2m_afirmar(hash_quitar(hash, "Prueba4") == EXITOSO, "Se quita correctamente un elemento del hash.");
    pa2m_afirmar(hash_cantidad(hash)== 1, "La cantidad de elementos en el hash es 1.");
    pa2m_afirmar(hash_quitar(hash, "Prueba6") == EXITOSO, "Se quita correctamente un elemento del hash.");
    pa2m_afirmar(hash_cantidad(hash)== 0, "La cantidad de elementos en el hash es 0.");
    pa2m_afirmar(hash_quitar(hash, "Prueba1") == ERROR, "Se intenta quitar un elemento del hash que ya fue quitado, resultado ERROR.");
    hash_destruir(hash);
}

//OBTENER
void dadoUnHash_seObtienenLosElementosEsperados(){
    hash_t* hash = hash_crear(NULL, 6);
    pa2m_afirmar(hash != NULL, "Se crea correctamente un hash");
    pa2m_afirmar(hash_insertar(hash, "PR-UNO", (void*)21)== EXITOSO, "Se inserta correctamente un elemento[1].");
    pa2m_afirmar(hash_insertar(hash, "PR-DOS", (void*)56)== EXITOSO, "Se inserta correctamente un elemento[2].");
    pa2m_afirmar(hash_insertar(hash, "PR-TRES", (void*)23)== EXITOSO, "Se inserta correctamente un elemento[3].");
    pa2m_afirmar(hash_insertar(hash, "PR-CUATRO", (void*)100)== EXITOSO, "Se inserta correctamente un elemento[4].");
    pa2m_afirmar(hash_cantidad(hash)== 4, "La cantidad de elementos dentro del hash es 4.");
    pa2m_afirmar(hash_obtener(hash, "PR-DOS")==(void*)56, "Se obtiene el elemento correspondiente con la clave PR-DOS.");
    pa2m_afirmar(hash_obtener(hash, "PR-CUATRO")==(void*)100, "Se obtiene el elemento correspondiente con la clave PR-CUATRO.");
    pa2m_afirmar(hash_obtener(hash, "PR-TRES")==(void*)23, "Se obtiene el elemento correspondiente con la clave PR-TRES.");
    pa2m_afirmar(hash_obtener(hash, "PR-UNO")==(void*)21, "Se obtiene el elemento correspondiente con la clave PR-UNO.");

    hash_destruir(hash);
}

//CONTIENE
void dadoUnHash_contieneLosElementosCorrespondientes(){
    hash_t* hash = hash_crear(NULL, 8);
    pa2m_afirmar(hash != NULL, "Se crea correctamente un hash");
    pa2m_afirmar(hash_insertar(hash, "E-UNO", "Elemento-uno")== EXITOSO, "Se inserta correctamente un elemento[1].");
    pa2m_afirmar(hash_insertar(hash, "E-DOS", "Elemento-dos")== EXITOSO, "Se inserta correctamente un elemento[2].");
    pa2m_afirmar(hash_insertar(hash, "E-TRES", "Elemento-tres")== EXITOSO, "Se inserta correctamente un elemento[3].");
    pa2m_afirmar(hash_insertar(hash, "E-CUATRO", "Elemento-cuatro")== EXITOSO, "Se inserta correctamente un elemento[4].");
    pa2m_afirmar(hash_insertar(hash, "E-CINCO", "Elemento-cinco")== EXITOSO, "Se inserta correctamente un elemento[5].");
    pa2m_afirmar(hash_cantidad(hash)== 5, "La cantidad de elementos dentro del hash es 5.");
    pa2m_afirmar(hash_contiene(hash, "E-DOS")== true, "Se consulta si contiene un elemento con la clave E-DOS, el resultado es el correspondiente.");
    pa2m_afirmar(hash_contiene(hash, "E-SEIS")== false, "Se consulta si contiene un elemento no insertado.");
    pa2m_afirmar(hash_insertar(hash, "E-SEIS", "Elemento-seis")== EXITOSO, "Se inserta correctamente un elemento[6].");
    pa2m_afirmar(hash_contiene(hash, "E-SEIS")== true, "Se consulta si contiene el elemento recien insertado.");
    pa2m_afirmar(hash_insertar(hash, "E-UNO", "Elemento-uno")== EXITOSO, "Se inserta correctamente un elemento[1].");
    hash_destruir(hash);
}

//Siempre retorna falso
bool es_falso(){
    return false;
}

//ITERADOR
void dadoUnHash_iteraCorrectamenteLaCantidadDeVecesCorrespondientes(){
    hash_t* hash = hash_crear(NULL, 20);
    pa2m_afirmar(hash != NULL, "Se crea correctamente un hash");
    hash_insertar(hash, "Billie Eilish", "ilomilo");
    hash_insertar(hash, "Avicci", "Hey Brother");
    hash_insertar(hash, "Alec Benjamin", "1994");
    hash_insertar(hash, "U2", "Bloody Sunday");
    hash_insertar(hash, "Keane", "SOWN");
    hash_insertar(hash, "Duki", "Goteo");
    hash_insertar(hash, "Artic Monkeys", "R U mine");
    hash_insertar(hash, "RHCP", "Snow");
    pa2m_afirmar(hash_cantidad(hash)==8, "Se insertaron correctamente 8 elementos.");
    pa2m_afirmar(hash_con_cada_clave(hash, es_falso, NULL)== 8, "Se recorrieron 8 elementos, con la funcion falsa.");

    hash_destruir(hash);
}

//TODAS LAS FUNCIONES
void prueboTodasLasFuncionesDeHash(){
    hash_t* hash = hash_crear(NULL, 5);
    pa2m_afirmar(hash != NULL, "Se crea correctamente un hash");
    hash_insertar(hash, "1A", "Primer piso A");
    hash_insertar(hash, "1B", "Primer piso B");
    hash_insertar(hash, "1C", "Primer piso C");
    hash_insertar(hash, "2A", "Segundo piso A");
    hash_insertar(hash, "2B", "Segundo piso B");
    hash_insertar(hash, "2C", "Segundo piso C");
    hash_insertar(hash, "3A", "Tercer piso A");
    hash_insertar(hash, "3B", "Tercer piso B");
    hash_insertar(hash, "3C", "Tercer piso C");
    hash_insertar(hash, "4A", "Cuarto piso A");
    hash_insertar(hash, "4B", "Cuarto piso B");
    hash_insertar(hash, "4C", "Cuarto piso C");
    hash_insertar(hash, "5A", "Quinto piso A");
    hash_insertar(hash, "5B", "Quinto piso B");
    hash_insertar(hash, "5C", "Quinto piso C");

    void* elemento_a_consultar = "Segundo piso A";
    pa2m_afirmar(hash_cantidad(hash)== 15, "La canitdad de los elementos en el haah es la correcta.");
    pa2m_afirmar(hash_contiene(hash, "2A")== true, "Se consulta si contiene el elemento del departamento 2A");
    pa2m_afirmar(hash_obtener(hash, "2A") == elemento_a_consultar, "El elemento dentro del depto 2A es el correspondiente.");
    hash_quitar(hash, "4C");
    hash_quitar(hash, "1B");
    pa2m_afirmar(hash_cantidad(hash)== 13, "Se quitaron dos elementos del hash correctamente.");
    pa2m_afirmar(hash_con_cada_clave(hash, es_falso, NULL) == 13, "Los elementos recorridos son 13.");
    pa2m_afirmar(hash_contiene(hash, "Penthouse")== false, "Se consulta si contiene un elemento no existente.");


    hash_destruir(hash);
}


int main(){
    pa2m_nuevo_grupo(" == TDA HASH == ");

    //NULL
    pa2m_nuevo_grupo("Pruebas con hash NULL");
    dadoHashNull_prueboLasPrimitivas_obtengoResultadoEsperado();

    //CREACION
    pa2m_nuevo_grupo("Pruebas de creacion");
    dadaCapacidadInicialValidaYDestructorNulo_seCreaUnHashCorrectamente();
    
    //INSERTAR
    pa2m_nuevo_grupo("Pruebas de insertar");
    dadoValoresValidos_seInsertanCorrectamenteLosElementos();
    dadoValoresValidos_seInsertanElementosEnHashConMenorCapacidad();

    //QUTAR 
    pa2m_nuevo_grupo("Pruebas de quitar");
    dadoUnHash_seQuitanElementos_obtengoResultadosEsperados();

    //OBTENER
    pa2m_nuevo_grupo("Pruebas de obtener");
    dadoUnHash_seObtienenLosElementosEsperados();

    //CONTIENE
    pa2m_nuevo_grupo("Pruebas de contiene");
    dadoUnHash_contieneLosElementosCorrespondientes();
    
    //ITERADOR
    pa2m_nuevo_grupo("Pruebas de hash con cada clave");
    dadoUnHash_iteraCorrectamenteLaCantidadDeVecesCorrespondientes();

    //TODAS LAS FUNCIONES
    pa2m_nuevo_grupo("Pruebas de todas las funciones en un unico hash");
    prueboTodasLasFuncionesDeHash();

    return pa2m_mostrar_reporte();
}