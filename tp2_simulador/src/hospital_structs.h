#ifndef HOSPITAL_STRUCTS_H_
#define HOSPITAL_STRUCTS_H_
#include <stdio.h>
#include "lista.h"
#include "cola.h"
#include "heap.h"

struct _entrenador_t{
    int id;
    char* nombre;
    lista_t* pokemones;
    bool atendido; //VER
};

struct _hospital_pkm_t{
    size_t cantidad_pokemon;
    heap_t* heap_pokeones_sala_espera;
    lista_t* lista_pokemones_atendidos;
    size_t cantidad_entrenadores;
    cola_t* cola_entrenadores_sala_espera;
    lista_t* lista_entrenadores_atendidos;
    pokemon_t* pokemon_atendiendose;
};


struct _pkm_t{
    char* nombre;
    size_t nivel;
    bool atendido;//VER
    entrenador_t* entrenador;
};


#endif //HOSPITAL_STRUCTS_H_