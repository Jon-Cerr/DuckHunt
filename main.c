/**
 * @file main.c
 * @author Alan Jonathan Cerritos Hernandez (alanchdez147@gmail.com)
 * @brief Juego Duck Hunt en C
 * @version 0.1
 * @date 2025-12-04
 *
 *
 */

#include "./src/graficos.h"
#include "./src/simplecontroller.h"
#include "./src/pato.h"
#include "./src/juego.h"
#include "./src/imagenes.h"
#include "./src/marcadores.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#define JX 15
#define JY 2
#define SW 4
#define MOTOR 13
#define MAX_JUAGDORES 5

#define AJUSTEX 0.45
#define AJUSTEY 0.35

/**
 * @brief Funcion que libera de la memoria las imagenes creadas durante todo el juego
 *
 * @param imagenes Puntero a la struct Imagenes que contiene las direcciones de imagenes a dibujar
 */
void liberarImagenes(Imagenes *imagenes);

int main()
{
    srand(time(0));
    bool fs = false;
    ventana.tamanioVentana(1280, 720);
    ventana.tituloVentana("Duck Hunt");

    Pato *pato = crearPato();
    Ronda *ronda = crearRonda();
    Imagenes *imagenes = crearImagenes();
    Juego *juego = (Juego *)malloc(sizeof(Juego));
    juego->pato = pato;
    juego->ronda = ronda;
    EstadoJuego estadoJuego = ESTADO_MENU;
    Puntuacion puntuacion[MAX_JUAGDORES];
    juego->puntuacion = puntuacion;
    inicializarMarcador(juego->puntuacion);
    int tecla = ventana.teclaPresionada();

    ventana.colorFondo(COLORES.AZULC);
    iniciarJuego(juego, imagenes, &estadoJuego);

    while (tecla != TECLAS.ESCAPE && (ronda->rondaContinuar))
    {
        tecla = ventana.teclaPresionada();
        ventana.limpiaVentana(); // Limpiamos
        if (ronda->rondaContinuar == false)
        {
            break;
        }

        if (estadoJuego == ESTADO_JUGANDO)
        {
            gameLoop(imagenes, juego);
        }

        ventana.actualizaVentana(); // Mostramos
        ventana.espera(10);
    }
    if (ronda->rondaContinuar == false)
    {
        Puntuacion nuevaPuntuacion;
        nuevaPuntuacion.nombreJugador = ronda->totalRonda;
        nuevaPuntuacion.puntaje = ronda->puntaje;
        actualizarMarcador(puntuacion, nuevaPuntuacion);
        ventana.limpiaVentana();
        gameLoop(imagenes, juego);
        ventana.actualizaVentana();
    }
    ventana.cierraVentana();
    free(pato);
    free(ronda);
    free(juego);
    liberarImagenes(imagenes);
    return 0;
}

Pato *crearPato()
{
    Pato *pato = (Pato *)malloc(sizeof(Pato));

    if (pato == NULL)
    {
        printf("El pato nacio morido :'/. Error en el programa...");
        ventana.imprimeEnConsola("El pato nacio morido :'/. Error en el programa...");
        return NULL;
    }

    pato->coorX = 600;
    pato->coorY = 400;
    pato->estado = 0;
    pato->ancho = 100;
    pato->alto = 100;
    pato->velX = 5;
    pato->velY = -3;
    pato->totalPatos = 10;

    return pato;
}

Ronda *crearRonda()
{
    Ronda *ronda = (Ronda *)malloc(sizeof(Ronda));

    if (ronda == NULL)
    {
        return NULL;
    }

    ronda->rondaContinuar = true;
    ronda->puntaje = 0;
    ronda->totalRonda = 1;
    ronda->mostrarRonda = true;

    return ronda;
}

void liberarImagenes(Imagenes *imagenes)
{
    ventana.eliminaImagen(imagenes->arbolImg);
    ventana.eliminaImagen(imagenes->arbolImg);
    ventana.eliminaImagen(imagenes->patoImg);
    ventana.eliminaImagen(imagenes->logoImg);
    ventana.eliminaImagen(imagenes->startIcon);
    ventana.eliminaImagen(imagenes->fondoImg);
    ventana.eliminaImagen(imagenes->marksIcon);
    ventana.eliminaImagen(imagenes->patoAbatidoImg);
}