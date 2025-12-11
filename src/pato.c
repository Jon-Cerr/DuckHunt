#include <time.h>
#include "imagenes.h"
#include "pato.h"

#define VELOCIDAD_CAIDA 8
#define MIN_Y_APARICION 50
#define MAX_Y_APARICION 300
#define AJUSTEX 0.45
#define AJUSTEY 0.3
#define JX 15
#define JY 4
#define SW 18
#define MOTOR 19

void dibujarPato(Imagenes *imagenes, Pato *pato)
{
    Imagen *imagenPatoEstado;
    if (pato->estado == 1)
    {
        imagenPatoEstado = imagenes->patoAbatidoImg;
    }

    else if (pato->estado == 2)
    {
        imagenPatoEstado = imagenes->patoCayendoImg;
    }

    else
    {
        imagenPatoEstado = imagenes->patoImg;
    }

    ventana.muestraImagenEscalada(pato->coorX, pato->coorY, pato->ancho, pato->alto, imagenPatoEstado);
}

void vueloPato(Pato *pato)
{
    if (pato->estado == 0)
    {
        pato->coorX += pato->velX;
        pato->coorY += pato->velY;
    }

    if (pato->coorY <= 0)
    {
        pato->velY *= -1;
    }

    if (pato->coorX <= 0 || (pato->coorX + pato->ancho) >= ventana.anchoVentana())
    {
        pato->velX *= -1;
    }

    if (pato->coorY >= (ventana.altoVentana() - 200) && pato->estado == 0)
    {
        pato->estado = 2;
        pato->totalPatos--;
    }
}

void estadoPato(Juego *juego, Imagenes *imagenes)
{
    if (juego->pato->estado == 1)
    {
        if (juego->pato->duracionImpacto > 0)
        {
            juego->pato->duracionImpacto--;
        }
        else
        {
            juego->pato->estado = 2;
        }
    }

    else if (juego->pato->estado == 2) // pato cayendo
    {
        juego->pato->coorY += VELOCIDAD_CAIDA;
        if (juego->pato->coorY >= 400)
        {
            juego->pato->estado = 3; // pato fuera del mapa
        }
    }
    else if (juego->pato->estado == 3)
    {
        int maxCoorX = ventana.anchoVentana() - juego->pato->ancho;
        int coorXAleat = rand() % (maxCoorX + 1);
        int coorYAleat = (rand() % (MAX_Y_APARICION - MIN_Y_APARICION + 1)) + MIN_Y_APARICION;
        juego->pato->coorX = coorXAleat;
        juego->pato->coorY = coorYAleat;
        juego->pato->velX = (rand() % 5 + 4) * ((rand() % 2 == 0) ? 1 : -1);
        juego->pato->velY = (rand() % 5) - 2;
        juego->pato->estado = 0;
    }
}

bool dispararPato(Juego *juego, Imagenes *imagenes, float joyX, float joyY, bool btn, Board *esp32)
{
    int ajusteX, ajusteY;

    joyX -= AJUSTEX;

    if (joyX >= 0.0)
        ajusteX = joyX * (9.0 / (1.0 - AJUSTEX));
    else
        ajusteX = joyX * (9.0 / (AJUSTEX));

    joyY -= AJUSTEY;

    if (joyY >= 0.0)
        ajusteY = joyY * (9.0 / (1.0 - AJUSTEY));
    else
        ajusteY = joyY * (9.0 / (AJUSTEY));

    juego->miraX += ajusteX;
    juego->miraY += ajusteY;

    if (juego->miraX < 0)
    {
        juego->miraX = 0;
    }
    else if (juego->miraX > ventana.anchoVentana())
    {
        juego->miraX = ventana.anchoVentana();
        ;
    }

    if (juego->miraY < 0)
    {
        juego->miraY = 0;
    }
    else if (juego->miraY > ventana.altoVentana())
    {
        juego->miraY = ventana.altoVentana();
        ;
    }

    ventana.color(COLORES.NEGRO);
    dibujarMira(juego->miraX, juego->miraY);

    if (btn && juego->pato->estado == 0)
    {
        ventana.reproducirAudio("./assets/audio/shot.wav");
        esp32->digitalWrite(esp32, MOTOR, true);
        juego->pato->duracionVibracion = 4;
        if (juego->miraX >= juego->pato->coorX &&
            juego->miraX <= (juego->pato->coorX + juego->pato->ancho))
        {
            if (juego->miraY >= juego->pato->coorY &&
                juego->miraY <= (juego->pato->coorY + juego->pato->alto))
            {
                juego->pato->estado = 1;
                juego->pato->duracionImpacto = 30;
                juego->pato->totalPatos--;
                juego->ronda->puntaje += 1000;
                if (juego->pato->totalPatos == 0)
                {
                    dibujarEscenarioRes1(imagenes);
                    ventana.color(COLORES.BLANCO);
                    mostrarInformacion(juego);
                    ventana.actualizaVentana();
                    juego->ronda->rondaContinuar = ventana.muestraPreguntaParamInt("Ronda %d finalizada! :D. Continuar?", "Aviso", (juego->ronda->totalRonda));
                    if (juego->ronda->rondaContinuar == true)
                    {
                        juego->pato->totalPatos = 10;
                        juego->pato->estado = 2;
                        juego->ronda->totalRonda++;
                        juego->pato->velX = ((rand() % 5 + 4) * ((rand() % 2 == 0) ? 1 : -1));
                        juego->pato->velY = ((rand() % 5) - 2);
                    }
                }
                return false;
            }
        }
    }
}

int dibujarMira(int rx, int ry)
{
    ventana.circulo(rx, ry, 20);
    ventana.linea(rx - 10, ry, rx - 30, ry);
    ventana.linea(rx + 10, ry, rx + 30, ry);
    ventana.linea(rx, ry - 10, rx, ry - 30);
    ventana.linea(rx, ry + 10, rx, ry + 30);
    ventana.circuloRelleno(rx, ry, 2);
}