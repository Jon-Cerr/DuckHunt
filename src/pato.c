#include <time.h>
#include "imagenes.h"
#include "pato.h"

#define VELOCIDAD_CAIDA 8
#define MIN_Y_APARICION 50
#define MAX_Y_APARICION 300

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

bool dispararPato(Juego *juego, Imagenes *imagenes)
{
    int rx, ry;
    bool botonPres = ventana.ratonBotonIzquierdo();
    ventana.raton(&rx, &ry);
    /*
    Board *esp32 = connectDevice("COM7", B115200);
    int rx = 300, ry = 300;
    float joyX, joyY;
    int ajusteX, ajusteY;
    bool btn = ventana.ratonBotonIzquierdo();

    esp32->pinMode(esp32, JX, INPUT);
    esp32->pinMode(esp32, JY, INPUT);
    esp32->pinMode(esp32, SW, INPUT_PULLUP);
    esp32->pinMode(esp32, MOTOR, OUTPUT);

    joyX = esp32->analogRead(esp32, JX);
    joyY = esp32->analogRead(esp32, JY);
    btn = esp32->digitalRead(esp32, SW);

    joyX -= AJUSTEX;

    if (joyX >= 0.0)
        ajusteX = joyX * (10.0 / (1.0 - AJUSTEX));
    else
        ajusteX = joyX * (11.0 / (AJUSTEX));

    joyY -= AJUSTEY;

    if (joyY >= 0.0)
        ajusteY = joyY * (11.0 / (1.0 - AJUSTEY));
    else
        ajusteY = joyY * (11.0 / (AJUSTEY));

    rx += ajusteX;
    ry += ajusteY;
    */

    if (botonPres && juego->pato->estado == 0)
    {
        ventana.reproducirAudio("./assets/audio/shot.wav");
        ventana.LimpiarEstadoBotonIzquierdo();
        if (rx >= juego->pato->coorX &&
            rx <= (juego->pato->coorX + juego->pato->ancho))
        {
            if (ry >= juego->pato->coorY &&
                ry <= (juego->pato->coorY + juego->pato->alto))
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
                        juego->pato->velX = ((rand() % 5 + 4) * ((rand() % 2 == 0) ? 1 : -1)) * 10;
                        juego->pato->velY = ((rand() % 5) - 2) * 10;
                    }
                }
                return false;
            }
        }
    }
}