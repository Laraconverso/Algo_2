#include "pa2mm.h"
#include "src/hospital.h"
#include "src/simulador.h"
#include "src/heap.h"
#include "string.h"
#include <stdbool.h>
#define MAX_ELEMENTOS 20

bool ignorar_pokemon(pokemon_t *p){
    p = p;
    return true;
}

/* No intenten esto en sus casas */
/* Ya vamos a ver como evitar esto en el TDA Lista */
struct{
    pokemon_t *pokemon[500];
    size_t cantidad;
} acumulados;

void resetear_acumulados(){
    acumulados.cantidad = 0;
}

bool acumular_pokemon(pokemon_t *p){
    acumulados.pokemon[acumulados.cantidad] = p;
    acumulados.cantidad++;
    return true;
}

bool acumular_pokemon_hasta_miltank(pokemon_t *p){
    acumulados.pokemon[acumulados.cantidad] = p;
    acumulados.cantidad++;
    return strcmp(pokemon_nombre(p), "miltank");
}

bool acumulados_en_orden_correcto(){
    if (acumulados.cantidad < 2)
        return true;
    pokemon_t *anterior = acumulados.pokemon[0];
    for (int i = 1; i < acumulados.cantidad; i++)
    {
        pokemon_t *actual = acumulados.pokemon[i];
        if (strcmp(pokemon_nombre(anterior), pokemon_nombre(actual)) > 0)
            return false;
    }
    return true;
}

/* Pruebas HOSPITAL POKEMON */

void puedoCrearYDestruirUnHospital(){
    hospital_t *h = NULL;

    pa2m_afirmar((h = hospital_crear()), "Crear un hospital devuelve un hospital");

    pa2m_afirmar(hospital_cantidad_entrenadores(h) == 0, "Un hospital se crea con cero entrenadores");
    pa2m_afirmar(hospital_cantidad_pokemon(h) == 0, "Un hospital se crea con cero pokemon");


    pa2m_afirmar(hospital_a_cada_pokemon(h, ignorar_pokemon) == 0, "Recorrer los pokemon resulta en 0 pokemon recorridos");

    hospital_destruir(h);
}

void dadoUnHospitalNULL_lasPuedoAplicarLasOperacionesDelHospitalSinProblema(){
    hospital_t *h = NULL;

    pa2m_afirmar(hospital_cantidad_entrenadores(h) == 0, "Un hospital NULL tiene cero entrenadores");
    pa2m_afirmar(hospital_cantidad_pokemon(h) == 0, "Un hospital NULL tiene cero pokemon");

    pa2m_afirmar(hospital_a_cada_pokemon(h, ignorar_pokemon) == 0, "Recorrer los pokemon de un hospital NULL resulta en 0 pokemon recorridos");

    hospital_destruir(h);
}

void dadoUnArchivoVacio_NoSeAgreganPokemonAlHospital(){
    hospital_t *h = hospital_crear();

    pa2m_afirmar(hospital_leer_archivo(h, "ejemplos/archivo_vacio.hospital"), "Puedo leer un archivo vacío");

    pa2m_afirmar(hospital_cantidad_entrenadores(h) == 0, "Un hospital vacío tiene cero entrenadores");
    pa2m_afirmar(hospital_cantidad_pokemon(h) == 0, "Un hospital vacío tiene tiene cero pokemon");

    pa2m_afirmar(hospital_a_cada_pokemon(h, ignorar_pokemon) == 0, "Recorrer los pokemon resulta en 0 pokemon recorridos");

    hospital_destruir(h);
}

void dadoUnArchivoConUnEntrenador_SeAgregaElEntrenadorYSusPokemonAlHospital(){
    hospital_t *h = hospital_crear();

    pa2m_afirmar(hospital_leer_archivo(h, "ejemplos/un_entrenador.hospital"), "Puedo leer un archivo con un entrenador");

    pa2m_afirmar(hospital_cantidad_entrenadores(h) == 1, "El hospital tiene 1 entrenador");
    pa2m_afirmar(hospital_cantidad_pokemon(h) == 3, "El hospital tiene 3 pokemon");

    resetear_acumulados();
    pa2m_afirmar(hospital_a_cada_pokemon(h, acumular_pokemon) == 3, "Recorrer los pokemon resulta en 3 pokemon recorridos");
    pa2m_afirmar(acumulados_en_orden_correcto(), "Los pokemon se recorrieron en orden alfabetico");

    hospital_destruir(h);
}

void dadoUnArchivoConVariosEntrenadores_SeAgreganLosEntrenadoresYSusPokemonAlHospital(){
    hospital_t *h = hospital_crear();

    pa2m_afirmar(hospital_leer_archivo(h, "ejemplos/varios_entrenadores.hospital"), "Puedo leer un archivo con varios entrenadores");

    pa2m_afirmar(hospital_cantidad_entrenadores(h) == 5, "El hospital tiene 5 entrenadores");
    pa2m_afirmar(hospital_cantidad_pokemon(h) == 24, "El hospital tiene 24 pokemon");

    resetear_acumulados();
    pa2m_afirmar(hospital_a_cada_pokemon(h, acumular_pokemon) == 24, "Recorrer los pokemon resulta en 24 pokemon recorridos");
    pa2m_afirmar(acumulados_en_orden_correcto(), "Los pokemon se recorrieron en orden alfabetico");

    hospital_destruir(h);
}

void dadosVariosArchivos_puedoAgregarlosTodosAlMismoHospital(){
    hospital_t *h = hospital_crear();

    pa2m_afirmar(hospital_leer_archivo(h, "ejemplos/varios_entrenadores.hospital"), "Puedo leer un archivo con varios entrenadores");
    pa2m_afirmar(hospital_leer_archivo(h, "ejemplos/varios_entrenadores.hospital"), "Puedo leer otro archivo con varios entrenadores");
    pa2m_afirmar(hospital_leer_archivo(h, "ejemplos/varios_entrenadores.hospital"), "Puedo leer un tercer archivo con varios entrenadores");

    pa2m_afirmar(hospital_cantidad_entrenadores(h) == 15, "El hospital tiene 15 entrenadores");
    pa2m_afirmar(hospital_cantidad_pokemon(h) == 72, "El hospital tiene 72 pokemon");

    resetear_acumulados();
    pa2m_afirmar(hospital_a_cada_pokemon(h, acumular_pokemon) == 72, "Recorrer los pokemon resulta en 72 pokemon recorridos");
    pa2m_afirmar(acumulados_en_orden_correcto(), "Los pokemon se recorrieron en orden alfabetico");

    hospital_destruir(h);
}

/* PRUEBAS HEAP*/
//compara dos elementos.
int comparador_heap(void *elemento_uno, void *elemento_dos){
    if((int*)elemento_uno > (int*) elemento_dos) return 1;
    else if((int*)elemento_uno == (int*) elemento_dos) return 0;
    return -1;
}

//simula ser un destructor.
void destructor_fantasma(void *elementos){
    return;
}

void heapPruebasNull(){
    pa2m_afirmar(heap_crear(NULL, NULL) == NULL, "No se crea un nuevo heap cuando su comparador es nulo.");
    pa2m_afirmar(heap_insertar(NULL, NULL) == NULL, "No se pueden insertar elementos de un hepa nulo.");
    pa2m_afirmar(heap_raiz(NULL) == NULL, "La raiz de un heap inexistente es nula.");
    pa2m_afirmar(heap_quitar_raiz(NULL) == NULL, "Quitar la raiz de un heap nulo retorna null.");
}

void dadoUnComaparadorValido_SeCreaUnHeapCorrectamente(){
    heap_t *heap = heap_crear(comparador_heap, NULL);
    if (!heap)
        return;
    pa2m_afirmar(heap != NULL, "Se crea un heap correctamente. (con destructor nulo)");
    pa2m_afirmar(heap_tamanio(heap) == 0, "Un heap recien creado no contiene elementos.");

    heap_destruir(heap);

    heap_t *heap_2 = heap_crear(comparador_heap, destructor_fantasma);
    if (!heap)
        return;
    pa2m_afirmar(heap_2 != NULL, "Se crea un nuevo heap corrcetamente.");
    pa2m_afirmar(heap_tamanio(heap_2) == 0, "El nuevo heap se encuentra vacio.");

    heap_destruir(heap_2);
}

void dadoVariosElementos_SeInsertanEnUnHeapCorrectamente(){
    void *ocho = (void *)8;
    void *trece = (void *)13;
    void *dos = (void *)2;
    void *diez = (void *)10;

    heap_t *heap = heap_crear(comparador_heap, NULL);
    pa2m_afirmar(heap != NULL, "Se crea un heap correctamente.");
    pa2m_afirmar(heap_tamanio(heap)==0, "El heap recien creado se encuentra vacio.");
    pa2m_afirmar(heap_insertar(heap, ocho) != NULL, "Se puede insertar un elemento correctamente.");
    pa2m_afirmar(heap_raiz(heap) == ocho, "El elemento recien insertado es la raiz del heap.");
    pa2m_afirmar(!heap_tamanio(heap)==0, "Un heap con un elemento ya no se encuentra vacio.");
    pa2m_afirmar(heap_insertar(heap, trece) != NULL, "Se inserta un segundo elemento.");
    pa2m_afirmar(heap_insertar(heap, dos) != NULL, "Se inserta un tercer elemento al heap correctemente.");
    pa2m_afirmar(heap_insertar(heap, diez) != NULL, "Se inserta el cuarto elemento correctamente");
    pa2m_afirmar(heap_raiz(heap) == dos, "La raiz de este heap minimal luego de insertar los valores 8, 13, 2 y 10 es 2.");

    heap_destruir(heap);
}

void dadoUnHeapConElementosOrdenados_SeQuitanLasRaicesCorrespondientes(){
    void *e_1 = (void *)1;
    void *e_2 = (void *)2;
    void *e_3 = (void *)3;
    void *e_4 = (void *)4;
    void *e_5 = (void *)5;
    void *e_6 = (void *)6;
    void *e_7 = (void *)7;
    void *e_8 = (void *)8;
    void *e_9 = (void *)9;

    heap_t *heap = heap_crear(comparador_heap, destructor_fantasma);
    pa2m_afirmar(heap != NULL, "Se crea un heap correctamente.");
    pa2m_afirmar(heap_tamanio(heap)==0, "El heap recien creado se encuentra vacio.");
    pa2m_afirmar(heap_insertar(heap, e_1) != NULL, "Se inserto el elemento '1' correctamente.");
    pa2m_afirmar(heap_insertar(heap, e_2) != NULL, "Se inserto el elemento '2' correctamente.");
    pa2m_afirmar(heap_insertar(heap, e_3) != NULL, "Se inserto el elemento '3' correctamente.");
    pa2m_afirmar(heap_insertar(heap, e_4) != NULL, "Se inserto el elemento '4' correctamente.");
    pa2m_afirmar(heap_insertar(heap, e_5) != NULL, "Se inserto el elemento '5' correctamente.");
    pa2m_afirmar(heap_insertar(heap, e_6) != NULL, "Se inserto el elemento '6' correctamente.");
    pa2m_afirmar(heap_insertar(heap, e_7) != NULL, "Se inserto el elemento '7' correctamente.");
    pa2m_afirmar(heap_insertar(heap, e_8) != NULL, "Se inserto el elemento '8' correctamente.");
    pa2m_afirmar(heap_insertar(heap, e_9) != NULL, "Se inserto el elemento '9' correctamente.");
    pa2m_afirmar(heap_raiz(heap) == e_1, "La raiz de este heap (heap minimal) es 1.");
    pa2m_afirmar(heap_quitar_raiz(heap) == e_1, "La raiz quitada es el elemento mas chico.");
    pa2m_afirmar(heap_quitar_raiz(heap) == e_2, "La raiz quitada es el elemento mas chico.");
    pa2m_afirmar(heap_quitar_raiz(heap) == e_3, "La raiz quitada es el elemento mas chico.");
    pa2m_afirmar(heap_quitar_raiz(heap) == e_4, "La raiz quitada es el elemento mas chico.");
    pa2m_afirmar(heap_quitar_raiz(heap) == e_5, "La raiz quitada es el elemento mas chico.");
    pa2m_afirmar(heap_quitar_raiz(heap) == e_6, "La raiz quitada es el elemento mas chico.");
    pa2m_afirmar(heap_quitar_raiz(heap) == e_7, "La raiz quitada es el elemento mas chico.");
    pa2m_afirmar(heap_quitar_raiz(heap) == e_8, "La raiz quitada es el elemento mas chico.");
    pa2m_afirmar(heap_quitar_raiz(heap) == e_9, "La raiz quitada es el elemento mas chico.");
    pa2m_afirmar(heap_tamanio(heap)==0, "Se quitaron todas las raices.");

    heap_destruir(heap);
}

void dadoUnHeapConElementos_SeQuitanLasRaicesCorrespondientes(){
    void *e_10 = (void *)10;
    void *e_7 = (void *)7;
    void *e_2 = (void *)2;
    void *e_5 = (void *)5;
    

    heap_t *heap = heap_crear(comparador_heap, destructor_fantasma);
    pa2m_afirmar(heap != NULL, "Se crea un heap correctamente.");
    pa2m_afirmar(heap_tamanio(heap)==0, "El heap recien creado se encuentra vacio.");
    pa2m_afirmar(heap_insertar(heap, e_10) != NULL, "Se inserto el elemento '10' correctamente.");
    pa2m_afirmar(heap_insertar(heap, e_7) != NULL, "Se inserto el elemento '7' correctamente.");
    pa2m_afirmar(heap_insertar(heap, e_2) != NULL, "Se inserto el elemento '2' correctamente.");
    pa2m_afirmar(heap_insertar(heap, e_5) != NULL, "Se inserto el elemento '7' correctamente.");
    pa2m_afirmar(heap_quitar_raiz(heap) == e_2, "La raiz quitada es el elemento mas chico.");
    pa2m_afirmar(heap_quitar_raiz(heap) == e_5, "La raiz quitada es el elemento mas chico.");
    pa2m_afirmar(heap_quitar_raiz(heap) == e_7, "La raiz quitada es el elemento mas chico.");
    pa2m_afirmar(heap_quitar_raiz(heap) == e_10, "La raiz quitada es el elemento mas chico.");

    heap_destruir(heap);
}

/* PRUEBAS SIMULADOR */

void dadoUnHospitalNulo_NoSeCreaElSimulador(){
    hospital_t *hospital_nulo = NULL;
    simulador_t* simulador = simulador_crear(hospital_nulo);
    pa2m_afirmar(simulador == NULL, "No se puede crear un simulador dado un hospital nulo.");
}

void pruebasSimularEventosSimuladorNulo(){
    pa2m_afirmar(simulador_simular_evento(NULL, ObtenerEstadisticas, NULL) == ErrorSimulacion, "No es posible simular evento: ObtenerEstadisticas con simulador inexistente.");
    pa2m_afirmar(simulador_simular_evento(NULL, AtenderProximoEntrenador, NULL) == ErrorSimulacion, "No es posible simular evento: AtenderProximoEntrenador con simulador inexistente.");
    pa2m_afirmar(simulador_simular_evento(NULL, ObtenerInformacionPokemonEnTratamiento, NULL) == ErrorSimulacion, "No es posible simular evento: ObtenerInformacionPokemonEnTratamiento con simulador inexistente.");
    pa2m_afirmar(simulador_simular_evento(NULL, AdivinarNivelPokemon, NULL) == ErrorSimulacion, "No es posible simular evento: AdivinarNivelPokemon con simulador inexistente.");
    pa2m_afirmar(simulador_simular_evento(NULL, AgregarDificultad, NULL) == ErrorSimulacion, "No es posible simular evento: ArgegarDificultad con simulador inexistente.");
    pa2m_afirmar(simulador_simular_evento(NULL, SeleccionarDificultad, NULL) == ErrorSimulacion, "No es posible simular evento: Seleccionardificultad con simulador inexistente.");
    pa2m_afirmar(simulador_simular_evento(NULL, ObtenerInformacionDificultad, NULL) == ErrorSimulacion, "No es posible simular evento: ObtenerInformacionDificultad con simulador inexistente.");
    pa2m_afirmar(simulador_simular_evento(NULL, FinalizarSimulacion, NULL) == ErrorSimulacion, "No es posible simular evento: FinalizarSimulacion con simulador inexistente.");
}

void dadoUnHospitalValido_SeCreaExitosamenteUnSimulador(){
    hospital_t* hospital_archivo_vacio = hospital_crear();
    simulador_t* simulador_archivo_vacio = simulador_crear(hospital_archivo_vacio);
    EstadisticasSimulacion estadisticas;
    hospital_leer_archivo(hospital_archivo_vacio, "ejemplos/archivo_vacio.hospital");
    pa2m_afirmar(simulador_archivo_vacio != NULL, "Se crea un simulador con un archivo vacio.");

    pa2m_afirmar(simulador_simular_evento(simulador_archivo_vacio, ObtenerEstadisticas, NULL)==ErrorSimulacion, "No se pueden simular el evento: ObtenerEstadisticas con datos nulos.");
    pa2m_afirmar(simulador_simular_evento(simulador_archivo_vacio, ObtenerEstadisticas, &estadisticas)==ExitoSimulacion, "Se pueden obtener las estadisticas de un hospital vacio.");
    pa2m_afirmar(estadisticas.pokemon_totales == 0 && estadisticas.pokemon_en_espera ==0 && estadisticas.pokemon_atendidos == 0, "El archivo se encuentra vacio, no hay pokemones por atender ni fueron atendidos.");
    pa2m_afirmar(estadisticas.entrenadores_atendidos == 0 && estadisticas.entrenadores_totales ==0, "El archivo se encuentra vacio, no hay entrenadores atendidos ni por atender.")
    pa2m_afirmar(estadisticas.cantidad_eventos_simulados == 2, "Se simularon 2 evento (ObtenerEstadisticas con y sin datos nulos).");
    pa2m_afirmar(estadisticas.puntos == 0, "No se sumaron puntos.");

    InformacionDificultad info_dificultad_facil;
    info_dificultad_facil.id = 0;
    pa2m_afirmar(simulador_simular_evento(simulador_archivo_vacio, ObtenerInformacionDificultad, &info_dificultad_facil)==ExitoSimulacion, "Se puede obtener la informacion de la dificultad[Facil].");
    pa2m_afirmar((info_dificultad_facil.id == 0 && strcmp(info_dificultad_facil.nombre_dificultad, "FACIL")==0 && !info_dificultad_facil.en_uso), "La dificultad 0 es la facil, y no se encuentra en uso.");

    InformacionDificultad info_dificultad_normal;
    info_dificultad_normal.id = 1;
    pa2m_afirmar(simulador_simular_evento(simulador_archivo_vacio, ObtenerInformacionDificultad, &info_dificultad_normal)==ExitoSimulacion, "Se puede obtener la informacion de la dificultad[Normal].");
    pa2m_afirmar((info_dificultad_normal.id == 1 && strcmp(info_dificultad_normal.nombre_dificultad, "NORMAL")==0 && info_dificultad_normal.en_uso), "La dificultad 1 es la normal, y se encuentra en uso dado a que es la preestablecida.");

    InformacionDificultad info_dificultad_dificil;
    info_dificultad_dificil.id = 2;
    pa2m_afirmar(simulador_simular_evento(simulador_archivo_vacio, ObtenerInformacionDificultad, &info_dificultad_dificil)==ExitoSimulacion, "Se puede obtener la informacion de la dificultad[Dificil].");
    pa2m_afirmar((info_dificultad_dificil.id == 2 && strcmp(info_dificultad_dificil.nombre_dificultad, "DIFICIL")==0 && !info_dificultad_dificil.en_uso), "La dificultad 2 es la dificil, y no se encuentra en uso");

    simulador_destruir(simulador_archivo_vacio);
}

void dadoUnHospitalDeVariosEntrenadores_SeEjecutanExitosamenteLosEventos(){
    hospital_t* hospital= hospital_crear();
    simulador_t* simulador = simulador_crear(hospital);
    EstadisticasSimulacion e;
    hospital_leer_archivo(hospital, "ejemplos/varios_entrenadores.hospital");

    pa2m_afirmar(simulador_simular_evento(simulador, ObtenerEstadisticas, &e)==ExitoSimulacion, "Se pueden obtener las estadisticas de un hospital.");
    pa2m_afirmar(e.entrenadores_atendidos == 0, "No se atendio ningun entrenador aun.");
    pa2m_afirmar(e.entrenadores_totales == 5, "El archivo contiene 5 entrenadores.");
    pa2m_afirmar(e.pokemon_atendidos == 0, "No se atendio ningun pokemon.");
    pa2m_afirmar(e.pokemon_en_espera == 0, "No hay pokemones en la sala de espera.");
    pa2m_afirmar(e.pokemon_totales == 24, "Los pokemones totales dentro del hospital son 24.");
    pa2m_afirmar(e.cantidad_eventos_simulados == 1, "Se simulo 1 evento (ObtenerEstadisticas).");
    pa2m_afirmar(simulador_simular_evento(simulador, AtenderProximoEntrenador, &e)==ExitoSimulacion, "Se puede atender al proximo entrenador.");
    pa2m_afirmar(simulador_simular_evento(simulador, ObtenerEstadisticas, &e)==ExitoSimulacion, "Se pueden obtener las estadisticas de un hospital con un entrenador atendido.");
    pa2m_afirmar(e.entrenadores_atendidos == 1, "Se atendio al proximo entrenador.");
    pa2m_afirmar(e.entrenadores_totales == 5, "El archivo contiene 5 entrenadores.");
    pa2m_afirmar(e.pokemon_atendidos == 0, "No se atendio ningun pokemon.");
    pa2m_afirmar(e.pokemon_en_espera == 3, "Hay 3 pokemones en la sala de espera.");
    pa2m_afirmar(e.pokemon_totales == 24, "Los pokemones totales dentro del hospital son 24.");
    pa2m_afirmar(e.cantidad_eventos_simulados == 3, "Se simularon 3 eventos.");

    InformacionPokemon info_pokemon;
    pa2m_afirmar(simulador_simular_evento(simulador,ObtenerInformacionPokemonEnTratamiento, &info_pokemon)==ExitoSimulacion, "Se obtener la informacion del pokemon en tratamiento.");
    pa2m_afirmar(strcmp(info_pokemon.nombre_pokemon,"rampardos")==0 ,"El pokemon en tratamiento es el correspondiente."); 
    pa2m_afirmar(strcmp(info_pokemon.nombre_entrenador, "lucas")==0, "El entrenador es el correspondiente." );
    
    Intento info_intento;
    info_intento.es_correcto = false;
    info_intento.nivel_adivinado = 10;
    info_intento.resultado_string = NULL;
    pa2m_afirmar(simulador_simular_evento(simulador, AdivinarNivelPokemon, &info_intento)==ExitoSimulacion, "Se simula exitosamente el evento de adivinar el nivel del pokemon.");
    pa2m_afirmar(info_intento.es_correcto == true, "El nivel adivinado es el correspondiente.");
    pa2m_afirmar(strcmp(info_intento.resultado_string,"Adivinaste el nivel ;)")==0,"El string devuelto es el correspondiente.");
    pa2m_afirmar(simulador_simular_evento(simulador, ObtenerEstadisticas, &e)==ExitoSimulacion, "Se pueden obtener las estadisticas de un hospital con un entrenador atendido y un pokemon con nivel adivinado.");
    pa2m_afirmar(e.pokemon_atendidos == 1, "Se atendio a un pokemon.");
    pa2m_afirmar(e.pokemon_en_espera == 2, "Hay 2 pokemones en la sala de espera.");
    
    pa2m_afirmar(simulador_simular_evento(simulador, FinalizarSimulacion, NULL) == ExitoSimulacion, "Se finaliza la simulacion.");
    pa2m_afirmar(simulador_simular_evento(simulador, ObtenerEstadisticas, &e)==ErrorSimulacion, "No se pueden simular mas eventos dado a que fue finalizado antes.");
    pa2m_afirmar(e.cantidad_eventos_simulados == 6, "Se simularon 6 eventos en total.");

    simulador_destruir(simulador);
}

int verificar_nivel_nueva(unsigned nivel_ingresado, unsigned nivel_pkm){
    int verificacion_estandar = (int)nivel_ingresado-(int)nivel_pkm;
    if(nivel_ingresado == nivel_pkm) return 0;
    return verificacion_estandar;
}

unsigned calcular_puntaje_nueva(unsigned intentos){
    return 5000-intentos*4+10;
}

const char* verificar_string_nueva(int resultado_verificaicon){
    if(resultado_verificaicon ==0) return "SEEE es el nivel!:)";
    else return "UFF ese no es.";
}

void dadoUnHospital_SePuedeAgregarDificultadYObtenerSuInformacion(){
    hospital_t* hospital = hospital_crear();
    simulador_t* simulador = simulador_crear(hospital);
    hospital_leer_archivo(hospital, "ejemplos/un_entrenador.hospital");
    
    DatosDificultad datos_nueva_dificultad = {.nombre = "MAS_DIFICIL",.verificar_nivel= verificar_nivel_nueva, .calcular_puntaje=calcular_puntaje_nueva, .verificacion_a_string= verificar_string_nueva};
    pa2m_afirmar(simulador_simular_evento(simulador, AgregarDificultad, &datos_nueva_dificultad)==ExitoSimulacion, "Se agrega exitosamente una dificultad");
    InformacionDificultad info_nueva_dificultad;
    info_nueva_dificultad.id =3;
    pa2m_afirmar(simulador_simular_evento(simulador, ObtenerInformacionDificultad, &info_nueva_dificultad)== ExitoSimulacion, "Se puede consultar la informaicon de la nueva dificultad.");
    pa2m_afirmar(info_nueva_dificultad.id ==3, "El id asignado a la nueva dificultad es el correspondiente.");
    pa2m_afirmar(info_nueva_dificultad.en_uso ==false, "La dificultad nueva no se encuentra en uso.");
    pa2m_afirmar((datos_nueva_dificultad.verificacion_a_string = verificar_string_nueva), "La verificacion a string es la correspondiente");
    pa2m_afirmar((datos_nueva_dificultad.verificar_nivel == verificar_nivel_nueva), "La verificacion del nivel es la correspondiente.");
    pa2m_afirmar((datos_nueva_dificultad.calcular_puntaje == calcular_puntaje_nueva), "El calculo del puntaje de la nuvea dificultad es el correspondiente.");
    pa2m_afirmar(strcmp(datos_nueva_dificultad.nombre, "MAS_DIFICIL")==0, "El nombre de la nueva dificultad se asigno correctamente.");
    pa2m_afirmar(strcmp(info_nueva_dificultad.nombre_dificultad, "MAS_DIFICIL")==0, "Se asigna bien el nombre");
    pa2m_afirmar(simulador_simular_evento(simulador, SeleccionarDificultad, &info_nueva_dificultad.id)==ExitoSimulacion, "Se puede cambiar la dificultad a la dificultad recientemente agregada.");
    pa2m_afirmar(simulador_simular_evento(simulador, ObtenerInformacionDificultad, &info_nueva_dificultad)==ExitoSimulacion, "Se puede obtener la informaicion de la dificultad agregada correctamente.");
    pa2m_afirmar(info_nueva_dificultad.en_uso == true, "La dificultad agregada esta en uso.");

    EstadisticasSimulacion e;
    simulador_simular_evento(simulador, ObtenerEstadisticas, &e);
    pa2m_afirmar(e.cantidad_eventos_simulados == 5, "La cantidad de eventos simulados es la correcta.");

    DatosDificultad datos_dificultad_repetida = {.nombre = "NORMAL",.verificar_nivel= verificar_nivel_nueva, .calcular_puntaje=calcular_puntaje_nueva, .verificacion_a_string= verificar_string_nueva};
    pa2m_afirmar(simulador_simular_evento(simulador, AgregarDificultad, &datos_dificultad_repetida)==ErrorSimulacion, "No se agrega una dificultad repetida.");

    simulador_destruir(simulador);
}


int main(){

    pa2m_nuevo_grupo("== PRUEBAS HOSPITAL POKEMON ==");

    pa2m_nuevo_grupo("Pruebas de  creación y destrucción");
    puedoCrearYDestruirUnHospital();

    pa2m_nuevo_grupo("Pruebas con NULL");
    dadoUnHospitalNULL_lasPuedoAplicarLasOperacionesDelHospitalSinProblema();

    pa2m_nuevo_grupo("Pruebas con un archivo vacío");
    dadoUnArchivoVacio_NoSeAgreganPokemonAlHospital();

    pa2m_nuevo_grupo("Pruebas con un archivo de un entrenador");
    dadoUnArchivoConUnEntrenador_SeAgregaElEntrenadorYSusPokemonAlHospital();

    pa2m_nuevo_grupo("Pruebas con un archivo de varios entrenadores");
    dadoUnArchivoConVariosEntrenadores_SeAgreganLosEntrenadoresYSusPokemonAlHospital();

    pa2m_nuevo_grupo("Pruebas con mas de un archivo");
    dadosVariosArchivos_puedoAgregarlosTodosAlMismoHospital();


    pa2m_nuevo_grupo("== PRUEBAS HEAP ==");

    pa2m_nuevo_grupo("Pruebas de null");
    heapPruebasNull();

    pa2m_nuevo_grupo("Pruebas de creacion");
    dadoUnComaparadorValido_SeCreaUnHeapCorrectamente();

    pa2m_nuevo_grupo("Pruebas de insercion");
    dadoVariosElementos_SeInsertanEnUnHeapCorrectamente();

    pa2m_nuevo_grupo("Pruebas de quitar raiz");
    dadoUnHeapConElementosOrdenados_SeQuitanLasRaicesCorrespondientes();
    dadoUnHeapConElementos_SeQuitanLasRaicesCorrespondientes();


    pa2m_nuevo_grupo("== PRUEBAS SIMULADOR POKEMON ==");

    pa2m_nuevo_grupo("Pruebas con simulador nulo");
    dadoUnHospitalNulo_NoSeCreaElSimulador();
    pruebasSimularEventosSimuladorNulo();

    pa2m_nuevo_grupo("Pruebas simulador archivo vacio");
    dadoUnHospitalValido_SeCreaExitosamenteUnSimulador();

    pa2m_nuevo_grupo("Pruebas simulador archivo varios entrenadores");
    dadoUnHospitalDeVariosEntrenadores_SeEjecutanExitosamenteLosEventos();

    pa2m_nuevo_grupo("Pruebas simulador eventos de dificultad");
    dadoUnHospital_SePuedeAgregarDificultadYObtenerSuInformacion();

    return pa2m_mostrar_reporte();
}
