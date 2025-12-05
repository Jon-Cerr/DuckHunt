/**
 * @file main.c
 * @author Alan Jonathan Cerritos Hernandez (alanchdez147@gmail.com)
 * @brief Juego Duck Hunt en C
 * @version 0.1
 * @date 2025-12-04
 *
 *
 */

#include "graficos.h"
#include <stdlib.h>
#include <time.h>
#define ALTURA_DEL_PASTO 600
#define VELOCIDAD_CAIDA 8
#define MIN_Y_APARICION 50
#define MAX_Y_APARICION 300

typedef struct
{
    int coorX;
    int coorY;
    int estado;
    int alto;
    int ancho;
    int velX;
    int velY;
    int totalPatos;
    bool mostrarScore;
} Pato;

typedef struct
{
    bool rondaContinuar;
    int puntaje;
    int totalRonda;
    bool mostrarRonda;
} Ronda;

void dibujarEscenarioRes1(Imagen *piso, Imagen *arbol);
void dibujarPato(Imagen *patoImg, Pato *pato);
void vueloPato(Pato *pato);
bool dispararPato(Pato *pato, Ronda *ronda, Imagen *pisoImg, Imagen *arbolImg, Imagen *scoreImg);
void estadoPato(Imagen *patoImg, Pato *pato, Ronda *ronda);
void gameLoop(Imagen *patoImg, Imagen *arbol, Imagen *piso, Pato *pato, Imagen *scoreImg, Ronda *ronda);

int main()
{
    srand(time(0));
    bool fs = false;
    Imagen *pisoImg = ventana.creaImagen("./piso.bmp");
    Imagen *arbolImg = ventana.creaImagen("./arbol.bmp");
    Imagen *patoImg = ventana.creaImagenConMascara("./pato1.bmp", "./patoMascara.bmp");
    Imagen *scoreImg = ventana.creaImagenConMascara("./score.bmp", "./scoreMascara.bmp");
    Pato *pato = (Pato *)malloc(sizeof(Pato));

    if (pato == NULL)
    {
        printf("El pato nacio morido :'/. Error en el programa...");
        ventana.imprimeEnConsola("El pato nacio morido :'/. Error en el programa...");
        return 1;
    }

    pato->coorX = 600;
    pato->coorY = 400;
    pato->estado = 0;
    pato->ancho = 100;
    pato->alto = 100;
    pato->velX = 5;
    pato->velY = -3;
    pato->totalPatos = 10;

    Ronda *ronda = (Ronda *)malloc(sizeof(Ronda));
    ronda->rondaContinuar = true;
    ronda->puntaje = 0;
    ronda->totalRonda = 1;
    ronda->mostrarRonda = true;

    bool derecha_pres = false, izquierda_pres = false;
    ventana.tamanioVentana(1280, 720);
    ventana.tituloVentana("Duck Hunt");
    ventana.colorFondo(COLORES.AZULC);
    int tecla = ventana.teclaPresionada();

    while (tecla != TECLAS.ESCAPE && (ronda->rondaContinuar))
    {
        tecla = ventana.teclaPresionada();
        ventana.limpiaVentana(); // Limpiamos
        if (ronda->rondaContinuar == false)
        {
            break;
        }

        gameLoop(patoImg, arbolImg, pisoImg, pato, scoreImg, ronda);
        ventana.actualizaVentana(); // Mostramos
        ventana.espera(10);
    }
    if (ronda->rondaContinuar == false)
    {
        ventana.limpiaVentana();
        gameLoop(patoImg, arbolImg, pisoImg, pato, scoreImg, ronda);
        ventana.actualizaVentana();
        ventana.muestraMensaje("Hasta luego!");
    }
    ventana.cierraVentana();
    free(pato);
    return 0;
}

void dibujarEscenarioRes1(Imagen *piso, Imagen *arbol)
{
    ventana.muestraImagenEscalada(0, ventana.altoVentana() - 250, ventana.anchoVentana(), 350, piso);
    ventana.muestraImagenEscalada(100, ventana.altoVentana() - 570, 150, 350, arbol);
}

void dibujarPato(Imagen *patoImg, Pato *pato)
{
    ventana.muestraImagenEscalada(pato->coorX, pato->coorY, pato->ancho, pato->alto, patoImg);
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

    if (pato->coorY >= (ventana.altoVentana() - 300) && pato->estado == 0)
    {
        pato->estado = 2;
        pato->totalPatos--;
    }
}

bool dispararPato(Pato *pato, Ronda *ronda, Imagen *pisoImg, Imagen *arbolImg, Imagen *scoreImg)
{
    int rx, ry;
    bool botonPres = ventana.ratonBotonIzquierdo();
    ventana.raton(&rx, &ry);
    if (botonPres && pato->estado == 0)
    {
        if (rx >= pato->coorX &&
            rx <= (pato->coorX + pato->ancho))
        {
            if (ry >= pato->coorY &&
                ry <= (pato->coorY + pato->alto))
            {
                pato->estado = 1;
                pato->totalPatos--;
                ronda->puntaje += 1000;
                pato->mostrarScore = true;
                ronda->mostrarRonda = true;
                if (pato->totalPatos == 0)
                {
                    dibujarEscenarioRes1(pisoImg, arbolImg);
                    ventana.muestraImagenEscalada(150, ventana.altoVentana() - 200, 50, 50, scoreImg);
                    ventana.color(COLORES.BLANCO);
                    ventana.muestraTextoParametroInt(100, 600, "Puntaje: %d", 30, "Arial", (ronda->puntaje));
                    ventana.muestraTextoParametroInt(ventana.anchoVentana() - 200, 600, "Ronda: %d", 30, "Arial", (ronda->totalRonda));
                    ventana.muestraTextoParametroInt(((ventana.anchoVentana() / 2) - 100), 600, "Patos restantes: %d", 30, "Arial", (pato->totalPatos));
                    ventana.actualizaVentana();
                    ronda->rondaContinuar = ventana.muestraPregunta("Terminaste el juego, felicidades! :D. Continuar?");
                    if (ronda->rondaContinuar == true)
                    {
                        pato->totalPatos = 10;
                        pato->estado = 2;
                        ronda->totalRonda++;
                    }
                }
                return false;
            }
        }
    }
}

void gameLoop(Imagen *patoImg, Imagen *arbolImg, Imagen *pisoImg, Pato *pato, Imagen *scoreImg, Ronda *ronda)
{
    dibujarEscenarioRes1(pisoImg, arbolImg);
    ventana.muestraImagenEscalada(150, ventana.altoVentana() - 200, 50, 50, scoreImg);

    if (ronda->rondaContinuar)
    {
        vueloPato(pato);
        dispararPato(pato, ronda, pisoImg, arbolImg, scoreImg);
        estadoPato(patoImg, pato, ronda);
    }

    if (ronda->rondaContinuar && pato->estado != 2)
    {
        dibujarPato(patoImg, pato);
    }

    if (pato->mostrarScore)
    {
        ventana.color(COLORES.BLANCO);
        ventana.muestraTextoParametroInt(100, 600, "Puntaje: %d", 30, "Arial", (ronda->puntaje));
    }

    if (ronda->mostrarRonda)
    {
        ventana.color(COLORES.BLANCO);
        ventana.muestraTextoParametroInt(ventana.anchoVentana() - 200, 600, "Ronda: %d", 30, "Arial", (ronda->totalRonda));
    }

    ventana.color(COLORES.BLANCO);
    ventana.muestraTextoParametroInt(((ventana.anchoVentana() / 2) - 100), 600, "Patos restantes: %d", 30, "Arial", (pato->totalPatos));

    ventana.color(COLORES.NEGRO);
    int rx, ry;
    ventana.raton(&rx, &ry);
    ventana.circuloRelleno(rx, ry, 10);
}

void estadoPato(Imagen *patoImg, Pato *pato, Ronda *ronda)
{
    if (pato->estado == 1)
    {
        pato->coorY += VELOCIDAD_CAIDA;

        if (pato->coorY >= 350)
        {
            pato->estado = 2;

            if (pato->totalPatos == 0)
            {
                ronda->rondaContinuar = false;
                ventana.muestraMensaje1("Ya no hay patos!", "GAME OVER");
            }
        }
    }
    else if (pato->estado == 2)
    {
        int maxCoorX = ventana.anchoVentana() - pato->ancho;
        pato->coorX = rand() % (maxCoorX + 1);
        pato->coorY = (rand() % (MAX_Y_APARICION - MIN_Y_APARICION + 1)) + MIN_Y_APARICION;
        pato->velX = (rand() % 5 + 4) * ((rand() % 2 == 0) ? 1 : -1);
        pato->velY = (rand() % 5) - 2;
        pato->estado = 0;
    }
}