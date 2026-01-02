/**
 * @file juego.c
 * @author Alan Jonathan Cerritos Hernandez (alanchdez147@gmail.com) - 2026640080
 * @author Ricardo Chaparro Zarco (ricardochzarco@gmail.com) - 2026640302
 * @author Jesus Roque Lopez (jesusroquelopez22@gmail.com) - 2026640210
 * @brief Archivo de definiciones para juego.c
 * @version 0.1
 * @date 2025-12-04
 * Practica 9: Manejo de Puerto Serie
 */
#include "graficos.h"
#include "juego.h"
#include "imagenes.h"
#include "pato.h"
#include "marcadores.h"
#include "simplecontroller.h"

#define JX 15
#define JY 4
#define SW 18
#define MOTOR 19

void iniciarJuego(Juego *juego, Imagenes *imagenes, EstadoJuego *estadoJuego)
{
    int coorStartBtnX = 450 + 10;
    int coorStartBtnY = 400 - 20;
    int coorMarksBtnX = (ventana.anchoVentana() / 2) - 80;
    int coorMarksBtnY = ventana.altoVentana() - 220;
    int rx, ry;

    if (*estadoJuego == ESTADO_MENU)
    {
        ventana.reproducirAudio("./assets/audio/audio.wav");
    }

    while (*estadoJuego == ESTADO_MENU)
    {
        ventana.limpiaVentana();
        ventana.muestraImagenEscalada(0, 0, ventana.anchoVentana(), ventana.altoVentana(), imagenes->fondoImg);
        ventana.muestraImagenEscalada((ventana.anchoVentana() / 2) - 190, ventana.altoVentana() - 740, 400, 400, imagenes->logoImg);
        ventana.muestraImagenEscalada(coorStartBtnX + 10, coorStartBtnY - 20, 350, 100, imagenes->startIcon);
        ventana.muestraImagenEscalada((ventana.anchoVentana() / 2) - 90, ventana.altoVentana() - 220, 200, 50, imagenes->marksIcon);
        ventana.raton(&rx, &ry);
        ventana.actualizaVentana();
        ventana.espera(10);
        if (ventana.ratonBotonIzquierdo())
        {
            if (rx >= coorStartBtnX &&
                rx <= (coorStartBtnX + 350) &&
                ry >= coorStartBtnY &&
                ry <= (coorStartBtnY + 50))
            {
                ventana.reproducirAudio(NULL);
                ventana.LimpiarEstadoBotonIzquierdo();
                *estadoJuego = ESTADO_JUGANDO;
            }
            else if (rx >= coorMarksBtnX &&
                     rx <= (coorMarksBtnX + 200) &&
                     ry >= coorMarksBtnY &&
                     ry <= (coorMarksBtnY + 50))
            {
                ventana.reproducirAudio(NULL);
                ventana.LimpiarEstadoBotonIzquierdo();
                *estadoJuego = ESTADO_MARCADORES;
                mostrarMarcadores(estadoJuego, imagenes, juego);
            }
        }
        if (ventana.teclaPresionada() == TECLAS.ESCAPE)
        {
            ventana.reproducirAudio(NULL);
            exit(0);
        }
    }
}

void gameLoop(Imagenes *imagenes, Juego *juego, Board *esp32)
{
    juego->ronda->mostrarRonda = true;
    dibujarEscenarioRes1(imagenes);

    if (juego->ronda->rondaContinuar)
    {
        float joyX = 0.0, joyY = 0.0;
        bool btn = false;
        if (esp32 != NULL)
        {
            joyX = esp32->analogRead(esp32, JX);
            joyY = esp32->analogRead(esp32, JY);
            btn = !esp32->digitalRead(esp32, SW);
        }
        vueloPato(juego->pato);
        dispararPato(juego, imagenes, joyX, joyY, btn, esp32);
        if (juego->pato->duracionVibracion > 0)
        {
            juego->pato->duracionVibracion--;
            if (juego->pato->duracionVibracion == 0)
            {
                esp32->digitalWrite(esp32, MOTOR, false);
            }
            
        }
        estadoPato(juego, imagenes);
    }
    if (juego->ronda->rondaContinuar && juego->pato->estado != 3)
    {
        dibujarPato(imagenes, juego->pato);
    }
    ventana.color(COLORES.BLANCO);
    if (juego->ronda->mostrarRonda)
    {
        mostrarInformacion(juego);
    }
    ventana.color(COLORES.NEGRO);
}

void dibujarEscenarioRes1(Imagenes *imagenes)
{
    ventana.muestraImagenEscalada(0, 0, ventana.anchoVentana(), ventana.altoVentana(), imagenes->fondoJuegoImg);
}

void mostrarInformacion(Juego *juego)
{
    ventana.muestraTextoParametroInt(100, 600, "Puntaje: %d", 30, "Arial", (juego->ronda->puntaje));
    ventana.muestraTextoParametroInt(ventana.anchoVentana() - 200, 600, "Ronda: %d", 30, "Arial", (juego->ronda->totalRonda));
    ventana.muestraTextoParametroInt(((ventana.anchoVentana() / 2) - 100), 600, "Patos restantes: %d", 30, "Arial", (juego->pato->totalPatos));
}
