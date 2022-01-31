#include "simulador.h"
#include "hospital.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "lista.h"
#include "heap.h"
#include "cola.h"
#include "hospital_structs.h"

#define FACIL "FACIL"
#define NORMAL "NORMAL"
#define DIFICIL "DIFICIL"
#define PUNTAJE_MAX 5000

typedef struct dificultad{
    char* nombre;
    InformacionDificultad* info;
    DatosDificultad* datos;
}dificultad_t;

struct _simulador_t{
    hospital_t* hospital;
    bool simulacion_finalizada;
    unsigned puntos_simulador;
    unsigned cantidad_eventos;
    unsigned intentos;
    lista_t* dificultades;
    DatosDificultad* dificultad_activa;
    int id_dificultad_en_uso;
};

ResultadoSimulacion agregar_dificultad(simulador_t* simulador, void* datos);
ResultadoSimulacion seleccionar_dificultad(simulador_t* simulador, void* datos);


/*
* Libera la memoria de la dificultad incluyendo su nombre;
*/
void dificultad_destructor(void* p_dificultad){
    if(!p_dificultad) return;
    dificultad_t* dificultad = (dificultad_t*)p_dificultad;
    if(!dificultad) return;
    free(dificultad->nombre);
    free(dificultad);
}

/*
* Calcula el puntaje en relacion a la cantidad de intentos y un valor maximo (5000)
*/
unsigned calcular_puntaje(unsigned cant_intentos){
    return (PUNTAJE_MAX/cant_intentos);
}

/*
* Compara ambos niveles recibidos por parametro, retorna 0 en caso de ser iguales.
*/
int verificar_nivel_dificultades(unsigned nivel_a_adivinar, unsigned nivel_pokemon){
    int verificacion_estandar = (int)nivel_a_adivinar-(int)nivel_pokemon;
    if(nivel_a_adivinar == nivel_pokemon) return 0;
    return verificacion_estandar;
}

/*
* Retorna string segun el resultado de la verificacion del nivel del pokemon recibida por parametro (version facil).
*/
const char* verificacion_a_string_facil(int resultado){
    if(resultado == 0) return "Adivinaste el nivel ;)";
    if(resultado > 0) return "Te faltan unos niveles!";
    else return "Te pasaste unos niveles!";
}

/*
* Retorna string segun el resultado de la verificacion del nivel del pokemon recibida por parametro (version normal).
*/
const char* verificacion_a_string_normal(int resultado){
    if(resultado == 0) return "Adivinaste el nivel ;)";
    if(resultado > 0) return "Te falta...";
    else return "mmm medio lejos te fuiste...";
}

/*
* Retorna string segun el resultado de la verificacion del nivel del pokemon recibida por parametro (version dificil).
*/
const char* verificacion_a_string_dificil(int resultado){
    if(resultado == 0) return "Adivinaste el nivel ;)";
    else return "Este no es el nivel ;(";
}

/*
*Inicializa las dificultades estandar del juego, facil, normal y dificl.
*/
void inicializar_dificultades_estandar(simulador_t* simulador){
    DatosDificultad dificultad_facil_datos;
    dificultad_facil_datos.nombre = FACIL;
    dificultad_facil_datos.verificar_nivel = verificar_nivel_dificultades;
    dificultad_facil_datos.calcular_puntaje = calcular_puntaje;
    dificultad_facil_datos.verificacion_a_string = verificacion_a_string_facil;

    DatosDificultad dificultad_normal_datos;
    dificultad_normal_datos.nombre = NORMAL;
    dificultad_normal_datos.verificar_nivel = verificar_nivel_dificultades;
    dificultad_normal_datos.calcular_puntaje = calcular_puntaje;
    dificultad_normal_datos.verificacion_a_string = verificacion_a_string_normal;

    DatosDificultad dificultad_dificil_datos;
    dificultad_dificil_datos.nombre = DIFICIL;
    dificultad_dificil_datos.verificar_nivel = verificar_nivel_dificultades;
    dificultad_dificil_datos.calcular_puntaje = calcular_puntaje;
    dificultad_dificil_datos.verificacion_a_string = verificacion_a_string_dificil;

    agregar_dificultad(simulador, &dificultad_facil_datos);
    agregar_dificultad(simulador, &dificultad_normal_datos);
    agregar_dificultad(simulador, &dificultad_dificil_datos);
}

//PRE & POST en simulador.h
simulador_t* simulador_crear(hospital_t* hospital){
    if(!hospital) return NULL;
    simulador_t* simulador_creado = calloc(1,sizeof(simulador_t));
    if(!simulador_creado) return NULL;
    simulador_creado->dificultades = lista_crear(dificultad_destructor);
    if(!simulador_creado->dificultades){
        free(simulador_creado);
        return NULL;
    }
    simulador_creado->hospital = hospital;
    simulador_creado->simulacion_finalizada = false;
    simulador_creado->intentos = 0;
    simulador_creado->puntos_simulador = (unsigned)0;
    simulador_creado->cantidad_eventos = 0;
    inicializar_dificultades_estandar(simulador_creado);
    int dificultad_default = 1;
    seleccionar_dificultad(simulador_creado, &dificultad_default);
    return simulador_creado;
}

//Inicializa la estructura pasada por parámetro con los valores corerspondientes.
//- Error posible: Dato nulo
ResultadoSimulacion obtener_estadisticas(simulador_t* simulador, void* datos){
    if(!datos) return ErrorSimulacion;
    EstadisticasSimulacion* estadisticas = (EstadisticasSimulacion*)datos;
    hospital_t* hospital = simulador->hospital;
    estadisticas->puntos = simulador->puntos_simulador;
    estadisticas->pokemon_totales = (unsigned)hospital_cantidad_pokemon(hospital);
    estadisticas->entrenadores_totales = (unsigned)hospital_cantidad_entrenadores(hospital);
    estadisticas->pokemon_en_espera = (unsigned)heap_tamanio(hospital->heap_pokeones_sala_espera);
    estadisticas->entrenadores_atendidos = (unsigned)lista_tamanio(hospital->lista_entrenadores_atendidos);
    estadisticas->pokemon_atendidos = (unsigned)lista_tamanio(hospital->lista_pokemones_atendidos);
    estadisticas->cantidad_eventos_simulados = simulador->cantidad_eventos;
    return ExitoSimulacion;
}

/*
* Ingresa al siguiente pokemon al consultorio(quita la raiz del heap de salas de espera);
*/
void atender_pokemon(hospital_t* hospital){
    if(hospital->pokemon_atendiendose) return;
    pokemon_t* pokemon_atendido = heap_quitar_raiz(hospital->heap_pokeones_sala_espera);
    hospital->pokemon_atendiendose = pokemon_atendido;
}

//Si quedan entrenadores en la sala de espera lo pasa a estado de atendido y sus pokemon quedan en espera.
//- Error posible: No hay entrenadores en espera
ResultadoSimulacion atender_prox_entrenador(simulador_t* simulador){
    hospital_t* hospital = simulador->hospital;
    if(cola_vacia(hospital->cola_entrenadores_sala_espera))return ErrorSimulacion;
    entrenador_t* entrenador = cola_desencolar(hospital->cola_entrenadores_sala_espera);
    size_t tamanio = lista_tamanio(entrenador->pokemones);
    for(size_t i = 0 ; i < tamanio; i++){
        pokemon_t* pokemon = lista_quitar(entrenador->pokemones);
        heap_t* aux = heap_insertar(hospital->heap_pokeones_sala_espera,pokemon);
        if(!aux) return ErrorSimulacion;
    }
    lista_t* lista_auxiliar = lista_insertar(hospital->lista_entrenadores_atendidos, entrenador);
    if(!lista_auxiliar) return ErrorSimulacion;
    atender_pokemon(hospital);
    return ExitoSimulacion;
}

// Llena los datos de la estructura con el pokemon que está siendo o debe ser atendido.
//- Error posible: Puntero nulo, no hay pokemon para atender (pone en null los punteros de la estructura).
ResultadoSimulacion obtener_info_pkm_tratamiento(simulador_t* simulador, void* datos){
    if(!datos) return ErrorSimulacion;
    hospital_t* hospital = simulador->hospital;
    InformacionPokemon* info = (InformacionPokemon*)datos;
    if(heap_tamanio(hospital->heap_pokeones_sala_espera) == 0){
        info->nombre_entrenador = NULL;
        info->nombre_pokemon = NULL;
        return ErrorSimulacion;
    }
    pokemon_t* pokemon = hospital->pokemon_atendiendose;
    if(!pokemon)
        pokemon = heap_raiz(hospital->heap_pokeones_sala_espera);
    info->nombre_pokemon = pokemon->nombre;
    info->nombre_entrenador = pokemon->entrenador->nombre;
    return ExitoSimulacion;
}

//- Operación: Verifica si el nivel pasado es el correcto del pokemon atendido (usando las funciones que corresponden según la dificultad activa) y llena la estructura con el resultado. El campo es_correcto es true si se acerta el nivel. El campo resultado_string contiene la salida de la función verificacion_a_string para esa dificultad.
//- Error posible: Puntero nulo. No hay pokemon siendo atendido.
ResultadoSimulacion adivinar_nivel_pkm(simulador_t* simulador, void* datos){
    if(!datos || !simulador) return ErrorSimulacion;
    Intento* intento_actual= (Intento*)datos;
    hospital_t* hospital = simulador->hospital;
    if(!hospital->pokemon_atendiendose) return ErrorSimulacion;
    simulador->intentos++;
    int resultado = simulador->dificultad_activa->verificar_nivel(intento_actual->nivel_adivinado, (unsigned)simulador->hospital->pokemon_atendiendose->nivel);
    intento_actual->resultado_string = simulador->dificultad_activa->verificacion_a_string(resultado);
    if(resultado == 0){
        intento_actual->es_correcto = true;
        simulador->puntos_simulador += simulador->dificultad_activa->calcular_puntaje(simulador->intentos);
        simulador->intentos = 0;
        lista_t* lista_aux = lista_insertar(hospital->lista_pokemones_atendidos, hospital->pokemon_atendiendose);
        if(!lista_aux) return ErrorSimulacion;
        hospital->pokemon_atendiendose = NULL;
        atender_pokemon(hospital);
    } else intento_actual->es_correcto = false;
    return ExitoSimulacion;
}


//- Operación: Agrega una nueva dificultad al simulador. El campo nombre indica el nombre de la dificultad para mostrarse por pantalla, calcular_puntaje devuelve la cantidad de puntos ganados por acertar dada la cantidad de intentos requeridos, verificar_nivel recibe el numero adivinado y el correcto y devuelve 0 en caso de acertar (y cualquier otro valor en otro caso) y verificacion_a_string recibe el numero resultado de la función verificar_nivel y lo transforma en un string que representa el resultado en forma de texto.
//- Error posible: Puntero o alguno de los campos nulos. Nombre de dificultad repetido.
ResultadoSimulacion agregar_dificultad(simulador_t* simulador, void* datos){
    if(!datos) return ErrorSimulacion;
    DatosDificultad* dificultad_datos = (DatosDificultad*)datos;
    if(!dificultad_datos->nombre || !dificultad_datos->verificacion_a_string || !dificultad_datos->verificar_nivel || !dificultad_datos->calcular_puntaje) return ErrorSimulacion;
    bool duplicado = false;
    for(size_t i = 0; i <lista_tamanio(simulador->dificultades); i++){
        dificultad_t* dificultad_actual = lista_elemento_en_posicion(simulador->dificultades, i);
        if(strcmp(dificultad_actual->nombre, dificultad_datos->nombre) ==0)
            duplicado = true;
    }
    if(duplicado==true) return ErrorSimulacion;
    else{
        DatosDificultad* copia_dificultad = malloc(sizeof(DatosDificultad));
        if(!copia_dificultad) return ErrorSimulacion;
        copia_dificultad->nombre = malloc(sizeof(char)*(strlen(dificultad_datos->nombre)+1));
        if(!copia_dificultad->nombre){
            free(copia_dificultad);
            return ErrorSimulacion;
        }
        strcpy((char*)copia_dificultad->nombre, (char*)dificultad_datos->nombre);
        copia_dificultad->verificacion_a_string = dificultad_datos->verificacion_a_string;
        copia_dificultad->calcular_puntaje = dificultad_datos->calcular_puntaje;
        copia_dificultad->verificar_nivel = dificultad_datos->verificar_nivel;
        lista_insertar(simulador->dificultades, copia_dificultad);
        return ExitoSimulacion;
    }
}

//- Operación: Selecciona la dificultad especificada y pasa a ser la dificultad activa para los proximos intentos
//- Error posible: Dato nulo, id inexistente.
ResultadoSimulacion seleccionar_dificultad(simulador_t* simulador, void* datos){
    if(!datos || !simulador) return ErrorSimulacion;
    int id = (*(int*)datos);
    if(id > lista_tamanio(simulador->dificultades) || id < 0) return ErrorSimulacion;
    DatosDificultad* dificultad_seleccionada = lista_elemento_en_posicion(simulador->dificultades, (size_t)id);
    if(!dificultad_seleccionada) return ErrorSimulacion;
    simulador->dificultad_activa = dificultad_seleccionada;
    simulador->id_dificultad_en_uso = id;
    return ExitoSimulacion;
}

//- Operación: Busca la dificultad con el id especificado y llena la estructura con la información de la dificultad (nombre y si está en uso o no). Si el id no existe, el nombre se inicializa en NULL y el id se cambia a -1.
//- Error posible: Puntero nulo.
ResultadoSimulacion obtener_info_dificultad(simulador_t* simulador, void* datos){
    if(!datos ||!simulador) return ErrorSimulacion;
    InformacionDificultad* info = (InformacionDificultad*)datos;
    DatosDificultad* dificultad = lista_elemento_en_posicion(simulador->dificultades, (size_t)info->id);
    if(!dificultad){
        info->nombre_dificultad = NULL;
        info->id = -1;
        return ErrorSimulacion;
    }
    info->nombre_dificultad = dificultad->nombre;
    if(strcmp(simulador->dificultad_activa->nombre ,dificultad->nombre)==0){
        info->en_uso = true;
    } else info->en_uso = false;
    return ExitoSimulacion;
}

/*PRE: Recibe un simulador valido
*POST: Finaliza la simulacion conviertiendo el campo simulaicon_finalizada a true.
*/
ResultadoSimulacion finalizar_simulacion(simulador_t* simulador){
    if(!simulador) return ErrorSimulacion;
    simulador->simulacion_finalizada = true;
    return ExitoSimulacion;
}

//PRE & POST en Simulador.h//
ResultadoSimulacion simulador_simular_evento(simulador_t* simulador, EventoSimulacion evento, void* datos){
    if(!simulador || simulador->simulacion_finalizada) return ErrorSimulacion;
    ResultadoSimulacion resultado_simulador = ErrorSimulacion;
    simulador->cantidad_eventos++;
    switch (evento){
        case ObtenerEstadisticas:
            resultado_simulador = obtener_estadisticas(simulador, datos);
            break;
        case AtenderProximoEntrenador:
            resultado_simulador = atender_prox_entrenador(simulador);
            break;
        case ObtenerInformacionPokemonEnTratamiento:
            resultado_simulador = obtener_info_pkm_tratamiento(simulador, datos);
            break;
        case AdivinarNivelPokemon:
            resultado_simulador = adivinar_nivel_pkm(simulador, datos);
            break;
        case AgregarDificultad:
            resultado_simulador = agregar_dificultad(simulador, datos);
            break;
        case SeleccionarDificultad:
            resultado_simulador = seleccionar_dificultad(simulador, datos);
            break;
        case ObtenerInformacionDificultad:
            resultado_simulador = obtener_info_dificultad(simulador, datos);
            break;
        case FinalizarSimulacion:
            resultado_simulador = finalizar_simulacion(simulador);
            break;
    }
    return resultado_simulador;
}


//PRE & POST en Simulador.h//
void simulador_destruir(simulador_t* simulador){
    if(!simulador) return;
    hospital_destruir(simulador->hospital);
    lista_destruir(simulador->dificultades);
    free(simulador);
}