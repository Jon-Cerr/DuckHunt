/**
 * @file juego.h
 * @author Alan Jonathan Cerritos Hernandez (alanchdez147@gmail.com) - 2026640080
 * @author Ricardo Chaparro Zarco (ricardochzarco@gmail.com) - 2026640302
 * @author Jesus Roque Lopez (jesusroquelopez22@gmail.com) - 2026640210
 * @brief Archivo de declaraciones de funciones, structs y enums para juego
 * @version 0.1
 * @date 2025-12-04
 * Practica 9: Manejo de Puerto Serie
 */
#ifndef JUEGO_H_
#define JUEGO_H_

#include "imagenes.h"
#include "simplecontroller.h"
typedef struct Pato Pato;
typedef struct Puntuacion Puntuacion;

typedef struct Ronda
{
    bool rondaContinuar;
    int puntaje;
    int totalRonda;
    bool mostrarRonda;
} Ronda;

typedef enum
{
    ESTADO_MENU,
    ESTADO_JUGANDO,
    ESTADO_FIN,
    ESTADO_MARCADORES
} EstadoJuego;

typedef struct Puntuacion
{
    int puntaje;
    int nombreJugador;
} Puntuacion;

typedef struct Juego
{
    Pato *pato;
    Puntuacion *puntuacion;
    Ronda *ronda;
    int miraX;
    int miraY;
} Juego;

/**
 * @brief Funcion que crea una instancia de un tipo de dato struct ronda con todo y sus campos y que retorna el puntero completo a la struct
 *
 * @return Ronda*
 */
Ronda *crearRonda();

/**
 * @brief Funcion que inicia el juego en estado menu, tiene dos opciones: iniciar juego y ver marcadores. Dibuja el logo y los botones, ademas de reproducir audio
 * @param juego Puntero a la struct Juego que contiene puntero a las stucts Pato, Ronda y Puntuacion
 * @param imagenes Puntero a la struct Imagenes que contiene las direcciones de imagenes a dibujar
 * @param estadoJuego Puntero a la enumeracion EstadoJuego que maneja los estaaos del juego: menu, inicio y fin
 */
void iniciarJuego(Juego *juego, Imagenes *imagenes, EstadoJuego *estadoJuego);

/**
 * @brief Funcion que ejecuta el ciclo del juego, internamente dibuja el estado del pato, las imagenes, asi como la puntuacion, ronda y patos restantes.
 * @param imagenes Puntero a la struct Imagenes que contiene las direcciones de imagenes a dibujar
 * @param juego Puntero a la struct Juego que contiene puntero a las stucts Pato, Ronda y Puntuacion
 */
void gameLoop(Imagenes *imagenes, Juego *juego, Board *esp32);

/**
 * @brief Funcion encargada de dibujar el escenario
 *
 * @param imagenes Puntero a la struct Imagenes que contiene las direcciones de imagenes a dibujar
 */
void dibujarEscenarioRes1(Imagenes *imagenes);


/**
 * @brief Nuestra la informacion de puntaje, totalRonda y totalPatos en la ventana
 *
 * @param pato Puntero a la struct Pato que contiene las caracteristicas de un pato
 * @param ronda Puntero a la struct Ronda que contiene la informacion de la ronda
 */
void mostrarInformacion(Juego *juego);

#endif