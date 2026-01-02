/**
 * @file pato.h
 * @author Alan Jonathan Cerritos Hernandez (alanchdez147@gmail.com) - 2026640080
 * @author Ricardo Chaparro Zarco (ricardochzarco@gmail.com) - 2026640302
 * @author Jesus Roque Lopez (jesusroquelopez22@gmail.com) - 2026640210
 * @brief Archivo de cabecera para las declaraciones de funciones, struct y enums para pato
 * @version 0.1
 * @date 2025-12-04
 * Practica 9: Manejo de Puerto Serie
 */
#ifndef PATO_H_
#define PATO_H_

#include "imagenes.h"
#include "juego.h"
#include "graficos.h"
typedef struct Pato
{
    int coorX;
    int coorY;
    int estado;
    int alto;
    int ancho;
    int velX;
    int velY;
    int duracionImpacto;
    int duracionVibracion;
    int totalPatos;
} Pato;

/**
 * @brief Funcion que crea una instancia de un tipo de dato struct pato con todo y sus campos y que retorna el puntero completo a la struct
 *
 * @return Pato*
 */
Pato *crearPato();

/**
 * @brief Funcion que se encarga de dibujar al pato en pantalla a corde al estado del mismo
 *
 * @param imagenes Puntero a la struct Imagenes que contiene las direcciones de imagenes a dibujar
 * @param pato Puntero a la struct pato que contiene las caracteristicas de un pato
 */
void dibujarPato(Imagenes *imagenes, Pato *pato);

/**
 * @brief Funcion encargada de dibuajr al pato en pantalla, asignandole coordenadas y velocidades si se cumplen ciertos criterios
 *
 * @param pato Puntero a la struct pato que contiene las caracteristicas de un pato
 */
void vueloPato(Pato *pato);

/**
 * @brief Funcion encargada de ejecutar la logica de disparo hacia el pato, las coordenadas del mouse y click izquierdo para disparar
 *
 * @param juego Puntero a la struct Juego que contiene las structs Pato y Ronda
 * @param imagenes Puntero a la struct Imagenes que contiene las direcciones de imagenes a dibujar
 * @return true si el jugador acepta otra ronda
 * @return false Si el pato no fue disparado
 */
bool dispararPato(Juego *juego, Imagenes *imagenes, float joyX, float joyY, bool btn, Board *esp32);

/**
 * @brief Funcion que trabaja la logica del estado del pato, si se encuntra cayendo (1), si esta fuera del mapa (2), y si el pato esta fuera del mapa, dibuja uno nuevo en coordenadas aleatorias y velocidad aleatoria
 *
 * @param Juego Puntero a la struct Juego que contiene las estructuras Pato y Ronda
 * @param imagenes Puntero a la struct Imagenes que contiene las direcciones de imagenes a dibujar
 */
void estadoPato(Juego *juego, Imagenes *imagenes);

int dibujarMira(int rx, int ry);

#endif
