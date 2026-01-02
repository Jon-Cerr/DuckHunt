/**
 * @file marcadores.h
 * @author Alan Jonathan Cerritos Hernandez (alanchdez147@gmail.com) - 2026640080
 * @author Ricardo Chaparro Zarco (ricardochzarco@gmail.com) - 2026640302
 * @author Jesus Roque Lopez (jesusroquelopez22@gmail.com) - 2026640210
 * @brief Archivo de cabecera para las declaraciones de funciones, structs y enums dentro de marcadores
 * @version 0.1
 * @date 2025-12-04
 * Practica 9: Manejo de Puerto Serie
 */
#ifndef MARCADORES_H_
#define MARCADORES_H_

#include "juego.h"
#include "imagenes.h"
#define MAX_JUAGDORES 5

/**
 * @brief Funcion que dibuja en pantalla el esatdo marcadores, es decir, el apartado de puntuacion. Dibuja un boton, icono y el puntaje tomado de un archivo binario, con musica de fondo.
 *
 * @param estadoJuego Puntero a la enum EstadoJuego que contiene los estados del juego
 * @param imagenes Puntero a la struct Imagenes que contiene las direcciones de imagenes a dibujar
 * @param juego Puntero a la struct Juego que contiene puntero a las stucts Pato, Ronda y Puntuacion
 */
void mostrarMarcadores(EstadoJuego *estadoJuego, Imagenes *imagenes, Juego *juego);

/**
 * @brief Funcion que inicializa un nuevo archivo binario para la insercion de informacion del puntaje, si ya exist, se lee el contenido en modo rb, si no existe, se crea el contenido en modo wb.
 *
 * @param puntuaciones Puntero a la struct Puntuaciones que almacena un array de 5 elementos de la misma struct
 */
void inicializarMarcador(Puntuacion puntuaciones[MAX_JUAGDORES]);

/**
 * @brief Funcion que actualiza el marcador acorde al puntaje obtenido al final de la ronda, si no se cumple el puntaje minimo (comparacion en todo el puntaje) el jugador no califica, caso contrario se carga su puntaje en el archivo binario y se ordena la informacion con la funcion ordenamientoBurbuja()
 *
 * @param puntajeActual Puntero a la struct Puntaje que accede al puntaje actual de la ronda
 * @param nuevoJugador Tipo de dato Puntuacion que accede al nombre del jugador
 */
void actualizarMarcador(Puntuacion puntajeActual[], Puntuacion nuevoJugador);

/**
 * @brief Funcion que ordena el puntaje de mayor a menjor para la insercion de un nuevo puntaje, metodo de burbuja
 *
 * @param puntajeActual Puntero a la struct Puntuacion que contiene el puntaje actual de la ejecucion del juego
 */
void ordenamientoBurbuja(Puntuacion puntajeActual[]);

#endif