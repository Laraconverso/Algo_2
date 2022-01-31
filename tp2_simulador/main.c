#include "src/hospital.h"
#include "src/simulador.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#define PUNTAJE_MAX 5000

typedef struct{
    simulador_t* simulador;
    bool jugando;
}Juego;

//invento 2 dificultades nuevas
unsigned calcular_puntaje_estandar(unsigned cant_intentos){
    return (PUNTAJE_MAX/cant_intentos);
}

int verificar_nivel_dificultades_estandar(unsigned nivel_a_adivinar, unsigned nivel_pokemon){
    int verificacion_estandar = (int)nivel_a_adivinar-(int)nivel_pokemon;
    if(nivel_a_adivinar == nivel_pokemon) return 0;
    return verificacion_estandar;
}

const char* cerca(int verificacion){
    if(verificacion == 0) return "Felicitaciones!! Adivinaste el nivel;)";
    if(verificacion > 5) return "Estas a mas de 5";
    else return "Estas a menos de 5.";
}

const char* temperatura(int verificacion){
    if(verificacion == 0) return "Hirviendo!! Adivinaste el nivel;)";
    if(verificacion >0) return "Frio, todavia no llegaste.";
    else return "Frio, te pasaste.";
}

char leer_comando(){
    char linea[100];
    char* leido;
    leido = fgets(linea, 100, stdin);
    if(!leido)
        return 0;
    while(*leido == ' ')
        leido++;
    return (char)tolower(*leido);
}

int leer_comando_numeros(){
    char linea[100];
    char* leido;
    leido = fgets(linea, 100, stdin);
    if(!leido)
        return 0;
    while(*leido == ' ')
        leido++;
    return atoi(leido);
}

void mostrar_estadisticas(Juego* juego){
    EstadisticasSimulacion e;
    simulador_simular_evento(juego->simulador, ObtenerEstadisticas, &e);
    printf("\n");
    printf("\t******************************************* \n");
    printf("\t**                                       ** \n");
    printf("\t**             ESTADISTICAS              ** \n");
    printf("\t**                                       ** \n");
    printf("\t******************************************* \n");
    printf("\n");
    printf("\t # Cantidad Entrenadores En Hospital %i\n", e.entrenadores_totales);
    printf("\t # Cantidad Pokemones En Hospital    %i\n", e.pokemon_totales);
    printf("\t # Cantidad Entrenadores Atendidos   %i\n", e.entrenadores_atendidos);
    printf("\t # Cantidad Pokemones Atendidos      %i\n", e.pokemon_atendidos);
    printf("\t # Cantidad Pokemones en Espera      %i\n", e.pokemon_en_espera);
    printf("\t # Cantidad Eventos Simulados        %i\n", e.cantidad_eventos_simulados);
    printf("\t # PUNTOS TOTALES                    %i\n", e.puntos);
    printf("\n");
    printf("\n");
}

void atender_proximo_entrenador(Juego* juego){
    simulador_simular_evento(juego->simulador, AtenderProximoEntrenador, NULL);
    printf("\n");
    printf("\t******************************************** \n");
    printf("\t*     Que pase el siguiente entrenador..  ** \n");
    printf("\t*  ***************************************** \n");
    printf("\t* *\n");
    printf("\t*\n");
    printf("\n");
    printf(" >> *Se atiende el siguiente entrenador*\n");
    printf("\n");
}

void mostrar_info_pokemon_en_consultorio(Juego* juego){
    InformacionPokemon info;
    simulador_simular_evento(juego->simulador, ObtenerInformacionPokemonEnTratamiento,&info);
    if(info.nombre_pokemon ==NULL && info.nombre_entrenador == NULL){
        printf(" ATENCION! No se encuentran pokemones en el consultorio \n Si desea ingresarlos debe atender a un entrenador.\n [Opcion p].\n");
    } else{
        printf("\n");
        printf("\t******************************************* \n");
        printf("\t**   Informacion Pokemon en Tratamiento  ** \n");
        printf("\t******************************************* \n");
        printf("\n");
        printf("\t # NOMBRE:      %s\n", info.nombre_pokemon);
        printf("\t # ENTRENADOR:  %s\n", info.nombre_entrenador);
        printf("\n");
    }
}

void adivinar_nivel(Juego* juego){
    Intento intento;
    InformacionPokemon info_pkm;
    simulador_simular_evento(juego->simulador, ObtenerInformacionPokemonEnTratamiento, &info_pkm);
    if(!info_pkm.nombre_entrenador){
        printf("No hay pokemon en el consultorio.\n");
        return;
    }
    printf("\n");
    printf("\t************************************* \n");
    printf("\t**     Adivinar nivel Pokemon      ** \n");
    printf("\t************************************* \n");
    printf("\n");
    printf("\n");
    printf("\t # NOMBRE:      %s\n", info_pkm.nombre_pokemon);
    printf("\n");
    printf("Ingrese nivel: \n");
    printf(" >>");
    intento.nivel_adivinado = (unsigned)leer_comando_numeros();
    simulador_simular_evento(juego->simulador, AdivinarNivelPokemon, &intento);
    while (!intento.es_correcto){
        printf("\t %s \n", intento.resultado_string);
        printf("Ingrese nivel: \n");
        printf(" >>");
        intento.nivel_adivinado = (unsigned)leer_comando_numeros();
        simulador_simular_evento(juego->simulador, AdivinarNivelPokemon, &intento);
    }
    printf(" Correcto!  %s\n", intento.resultado_string);
    printf("\n");
}

void sumar_dificultades(Juego* juego){
    printf("Elija una opcion de las dificultades disponibles:\n");
    printf("[ ] CERCANIA -> ingrese 'c' \n[ ] FRIO_CALIENTE-> ingrese 'f'\n");
    printf("\n");
    printf(">> \n");
    char opcion = leer_comando();
    printf("\n");
    if(opcion =='c'){
        DatosDificultad cercania = {.nombre = "CERCANIA", .calcular_puntaje =calcular_puntaje_estandar, .verificacion_a_string = cerca, .verificar_nivel= verificar_nivel_dificultades_estandar};
        ResultadoSimulacion agregada = simulador_simular_evento(juego->simulador, AgregarDificultad, &cercania);
        if(agregada == ExitoSimulacion)
            printf("Se agrego exitosamente la dificultad cercania!\n");
        else printf("ERROR en la carga de la nueva dificultad.\n");
    } else if( opcion == 'f'){
        DatosDificultad frio_caliente = {.nombre = "FRIO_CALIENTE", .calcular_puntaje = calcular_puntaje_estandar, .verificacion_a_string = temperatura, .verificar_nivel = verificar_nivel_dificultades_estandar};
        ResultadoSimulacion agregada = simulador_simular_evento(juego->simulador, AgregarDificultad, &frio_caliente);
        if(agregada == ExitoSimulacion)
            printf("Se agrego exitosamente la dificultad frio_caliente!\n");
        else printf("ERROR en la carga de la nueva dificultad.\n");
    }

}

void mostrar_info_dificultad(Juego* juego){
    InformacionDificultad info_dificultad;
    info_dificultad.id = 0;
    ResultadoSimulacion resultado = simulador_simular_evento(juego->simulador, ObtenerInformacionDificultad, &info_dificultad);
    printf("\n");
    printf("\t************************************* \n");
    printf("\t**  Informacion Dificultad Activa   * \n");
    printf("\t************************************* \n");
    printf("\n");
    while(resultado == ExitoSimulacion){
        printf("\t *[%i] %s", info_dificultad.id, info_dificultad.nombre_dificultad);
        if(info_dificultad.en_uso) printf(" <-- En uso\n");
        else printf("\n");
        info_dificultad.id++;
        resultado = simulador_simular_evento(juego->simulador, ObtenerInformacionDificultad, &info_dificultad);
    }
    printf("\n");
}

void elegir_dificultad(Juego* juego){
    int id_ingresado;
    printf("Ingrese el id de la dificultad deseada: \n");
    id_ingresado = leer_comando_numeros();
    ResultadoSimulacion resultado = simulador_simular_evento(juego->simulador, SeleccionarDificultad, &id_ingresado);
    while(resultado == ErrorSimulacion){
        printf("Id invalido :(, volve a intentar: ");
        id_ingresado = leer_comando_numeros();
        resultado = simulador_simular_evento(juego->simulador, SeleccionarDificultad, &id_ingresado);
    }
    InformacionDificultad info_dificultad;
    info_dificultad.id = id_ingresado;
    simulador_simular_evento(juego->simulador, ObtenerInformacionDificultad, &info_dificultad);
    printf("\n");
    printf(" >> *Se cambio la dificultad actual a [%i] %s *\n", info_dificultad.id, info_dificultad.nombre_dificultad);
    printf("\n");
    
}

void comando_equivocado(){
    printf("Opcion invalida, por favor ingrese un comando de la lista\n");
}

void finalizacion_juego(Juego* juego){
    EstadisticasSimulacion e;
    simulador_simular_evento(juego->simulador, ObtenerEstadisticas, &e);
    juego->jugando = false;
    printf("\n");
    printf("\t PUNTOS TOTALES: %i\n\n", e.puntos);
    printf("\t ==== Juego Finalizado ====\n");
    printf("\n");
}

void ejecutar_comando(Juego *juego, char comando){
    if(!juego) return;
    switch (comando) {
        case 'q':
            finalizacion_juego(juego);
            break;
        case 'e':
            mostrar_estadisticas(juego);
            break;
        case 'p':
            atender_proximo_entrenador(juego);
            break;
        case 'i':
            mostrar_info_pokemon_en_consultorio(juego);
            break;
        case 'a':
            adivinar_nivel(juego);
            break;
        case 'm':
            sumar_dificultades(juego);
            break;
        case 'o':
            mostrar_info_dificultad(juego);
            break;
        case 'd':
            elegir_dificultad(juego);
            break;
        default:
            comando_equivocado();
            break;
    }
}

void inicializar_juego(Juego* juego, hospital_t* hospital){
    if(!hospital) return;
    juego->jugando = true;
    juego->simulador = simulador_crear(hospital);
    if(!juego->simulador) return;

}

void destruir_juego(Juego* juego){
    if(!juego->simulador) return;
    simulador_destruir(juego->simulador);
}

void mostrar_comandos(){
    printf("\n");
    printf("\t||=============== LISTA DE COMANDOS =================||\n");
    printf("\t||                                                   ||\n");
    printf("\t|| [e] Obtener Estadisticas                          ||\n");
    printf("\t|| [p] Atender Proximo Entrenador                    ||\n");
    printf("\t|| [i] Obtener Informacion Pokemon En Tratamiento    ||\n");
    printf("\t|| [a] Adivinar nivel pokemon                        ||\n");
    printf("\t|| [m] Agregar dificultad                            ||\n");
    printf("\t|| [o] Obtener Informacion Dificultad                ||\n");
    printf("\t|| [d] Seleccionar Dificultad                        ||\n");
    printf("\t|| [q] Finalizar simulacion                          ||\n");
    printf("\t||===================================================||\n");
    printf("\n");
}

int main(int argc, char *argv[]) {
    hospital_t* hospital = hospital_crear();
    hospital_leer_archivo(hospital, "ejemplos/varios_entrenadores.hospital");

    Juego juego;
    inicializar_juego(&juego, hospital);
    printf(" AVISO: La dificultad prestablecida es la [1]Normal.\n");
    do{
        mostrar_comandos();
        printf( ">>");
        char c = leer_comando();
        system("clear");
        ejecutar_comando(&juego, c);
    }while(juego.jugando);

    destruir_juego(&juego);

    return 0;
}
