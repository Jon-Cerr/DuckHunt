#include "graficos.h"
#include "juego.h"
#include "imagenes.h"
#include "pato.h"
#include "marcadores.h"

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

void gameLoop(Imagenes *imagenes, Juego *juego)
{
    juego->ronda->mostrarRonda = true;
    dibujarEscenarioRes1(imagenes);

    if (juego->ronda->rondaContinuar)
    {
        vueloPato(juego->pato);
        dispararPato(juego, imagenes);
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
    dibujarMira();
}

void dibujarEscenarioRes1(Imagenes *imagenes)
{
    /*
    ventana.muestraImagenEscalada(0, ventana.altoVentana() - 250, ventana.anchoVentana(), 350, imagenes->pisoImg);
    ventana.muestraImagenEscalada(100, ventana.altoVentana() - 570, 150, 350, imagenes->arbolImg);
    */
    ventana.muestraImagenEscalada(0, 0, ventana.anchoVentana(), ventana.altoVentana(), imagenes->fondoJuegoImg);
}

void dibujarMira()
{
    int rx = ventana.ratonX(), ry = ventana.ratonY();
    ventana.circulo(rx, ry, 20);
    ventana.linea(rx - 10, ry, rx - 30, ry);
    ventana.linea(rx + 10, ry, rx + 30, ry);
    ventana.linea(rx, ry - 10, rx, ry - 30);
    ventana.linea(rx, ry + 10, rx, ry + 30);
    ventana.circuloRelleno(rx, ry, 2);
}

void mostrarInformacion(Juego *juego)
{
    ventana.muestraTextoParametroInt(100, 600, "Puntaje: %d", 30, "Arial", (juego->ronda->puntaje));
    ventana.muestraTextoParametroInt(ventana.anchoVentana() - 200, 600, "Ronda: %d", 30, "Arial", (juego->ronda->totalRonda));
    ventana.muestraTextoParametroInt(((ventana.anchoVentana() / 2) - 100), 600, "Patos restantes: %d", 30, "Arial", (juego->pato->totalPatos));
}
