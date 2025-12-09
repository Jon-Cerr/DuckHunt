#ifndef IMAGENES_H_
#define IMAGENES_H_

#include"graficos.h"

typedef struct Imagenes
{
    Imagen *pisoImg;
    Imagen *arbolImg;
    Imagen *fondoJuegoImg;
    Imagen *patoImg;
    Imagen *logoImg;
    Imagen *startIcon;
    Imagen *fondoImg;
    Imagen *marksIcon;
    Imagen *backButon;
    Imagen *patoAbatidoImg;
    Imagen *patoCayendoImg;
} Imagenes;

/**
 * @brief Funcion que crea una instancia de un tipo de dato struct imagenes con todo y sus campos y que retorna el puntero completo a la struct
 * 
 * @return Imagenes* 
 */
Imagenes *crearImagenes();

#endif