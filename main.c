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
#include "simplecontroller.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#define VELOCIDAD_CAIDA 8
#define MIN_Y_APARICION 50
#define MAX_Y_APARICION 300
#define JX 15
#define JY 2
#define SW 4
#define MOTOR 13
#define MIN_PUNTUACION 20000

#define AJUSTEX 0.45
#define AJUSTEY 0.35

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

typedef enum
{
    ESTADO_MENU,
    ESTADO_JUGANDO,
    ESTADO_FIN,
    ESTADO_MARCADORES
} EstadoJuego;

typedef struct
{
    Imagen *pisoImg;
    Imagen *arbolImg;
    Imagen *patoImg;
    Imagen *scoreImg;
    Imagen *logoImg;
    Imagen *startIcon;
    Imagen *fondoImg;
    Imagen *marksIcon;
    Imagen *backButon;
} Imagenes;

typedef struct
{
    int puntaje;
} Puntuacion;

Pato *crearPato();
Ronda *crearRonda();
Imagenes *crearImagenes();

void iniciarJuego(Imagenes *imagenes, EstadoJuego *estadoJuego);
void gameLoop(Imagenes *imagenes, Pato *pato, Ronda *ronda);
void dibujarEscenarioRes1(Imagenes *imagenes);
void dibujarPato(Imagenes *imagenes, Pato *pato);
void vueloPato(Pato *pato);
bool dispararPato(Pato *pato, Ronda *ronda, Imagenes *imagenes);
void dibujarMira();
void mostrarInformacion(Pato *pato, Ronda *ronda);
void iniciarMarcador();
void mostrarMarcadores(EstadoJuego *estadoJuego, Imagenes *imagenes);
void estadoPato(Pato *pato, Ronda *ronda);
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
    EstadoJuego estadoJuego = ESTADO_MENU;

    Puntuacion puntuacion;
    puntuacion.puntaje = 0;

    FILE *marcadores;

    int tecla = ventana.teclaPresionada();

    ventana.colorFondo(COLORES.AZULC);
    iniciarJuego(imagenes, &estadoJuego);

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
            gameLoop(imagenes, pato, ronda);
        }

        ventana.actualizaVentana(); // Mostramos
        ventana.espera(10);
    }
    if (ronda->rondaContinuar == false)
    {
        ventana.limpiaVentana();
        gameLoop(imagenes, pato, ronda);
        ventana.actualizaVentana();
        ventana.muestraMensaje("Hasta luego!");
    }
    ventana.cierraVentana();
    free(pato);
    free(ronda);
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
    pato->mostrarScore = false;

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

Imagenes *crearImagenes()
{
    Imagenes *imagenes = (Imagenes *)malloc(sizeof(Imagenes));

    if (imagenes == NULL)
    {
        printf("Error: No se pudo asignar memoria para la creacion de Imagenes.\n");
        return NULL;
    }

    imagenes->arbolImg = ventana.creaImagen("./assets/img/arbol.bmp");
    imagenes->patoImg = ventana.creaImagenConMascara("./assets/img/pato1.bmp", "./assets/img/patoMascara.bmp");
    imagenes->scoreImg = ventana.creaImagenConMascara("./assets/img/score.bmp", "./assets/img/scoreMascara.bmp");
    imagenes->logoImg = ventana.creaImagenConMascara("./assets/img/logo.bmp", "./assets/img/logoMascara.bmp");
    imagenes->startIcon = ventana.creaImagenConMascara("./assets/img/startIcon.bmp", "./assets/img/startIconMascara.bmp");
    imagenes->fondoImg = ventana.creaImagen("./assets/img/fondoInicio.bmp");
    imagenes->marksIcon = ventana.creaImagenConMascara("./assets/img/marcadores.bmp", "./assets/img/marcadoresMascara.bmp");
    imagenes->backButon = ventana.creaImagenConMascara("./assets/img/backBtn.bmp", "./assets/img/backBtnMascara.bmp");
    imagenes->pisoImg = ventana.creaImagen("./assets/img/piso.bmp");
    return imagenes;
}

void iniciarJuego(Imagenes *imagenes, EstadoJuego *estadoJuego)
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
        ventana.muestraImagenEscalada((ventana.anchoVentana() / 2) - 170, ventana.altoVentana() - 740, 350, 350, imagenes->logoImg);
        ventana.muestraImagenEscalada(coorStartBtnX + 10, coorStartBtnY - 20, 350, 100, imagenes->startIcon);
        ventana.muestraImagen((ventana.anchoVentana() / 2) - 90, ventana.altoVentana() - 220, imagenes->marksIcon);
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
                mostrarMarcadores(estadoJuego, imagenes);
            }
        }

        if (ventana.teclaPresionada() == TECLAS.ESCAPE)
        {
            ventana.reproducirAudio(NULL);
            exit(0);
        }
    }
}

void gameLoop(Imagenes *imagenes, Pato *pato, Ronda *ronda)
{
    pato->mostrarScore = true;
    ronda->mostrarRonda = true;
    dibujarEscenarioRes1(imagenes);
    ventana.muestraImagenEscalada(150, ventana.altoVentana() - 200, 50, 50, imagenes->scoreImg);

    if (ronda->rondaContinuar)
    {
        vueloPato(pato);
        dispararPato(pato, ronda, imagenes);
        estadoPato(pato, ronda);
    }

    if (ronda->rondaContinuar && pato->estado != 2)
    {
        dibujarPato(imagenes, pato);
    }

    ventana.color(COLORES.BLANCO);
    if (pato->mostrarScore && ronda->mostrarRonda)
    {
        mostrarInformacion(pato, ronda);
    }

    ventana.color(COLORES.NEGRO);
    dibujarMira();
}

void dibujarEscenarioRes1(Imagenes *imagenes)
{
    ventana.muestraImagenEscalada(0, ventana.altoVentana() - 250, ventana.anchoVentana(), 350, imagenes->pisoImg);
    ventana.muestraImagenEscalada(100, ventana.altoVentana() - 570, 150, 350, imagenes->arbolImg);
}

void dibujarPato(Imagenes *imagenes, Pato *pato)
{
    ventana.muestraImagenEscalada(pato->coorX, pato->coorY, pato->ancho, pato->alto, imagenes->patoImg);
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

bool dispararPato(Pato *pato, Ronda *ronda, Imagenes *imagenes)
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

    // ventana.reproducirAudio(NULL);

    if (botonPres && pato->estado == 0)
    {
        ventana.reproducirAudio("./assets/audio/shot.wav");
        ventana.LimpiarEstadoBotonIzquierdo();
        if (rx >= pato->coorX &&
            rx <= (pato->coorX + pato->ancho))
        {
            if (ry >= pato->coorY &&
                ry <= (pato->coorY + pato->alto))
            {
                pato->estado = 1;
                pato->totalPatos--;
                ronda->puntaje += 1000;
                if (pato->totalPatos == 0)
                {
                    dibujarEscenarioRes1(imagenes);
                    ventana.muestraImagenEscalada(150, ventana.altoVentana() - 200, 50, 50, imagenes->scoreImg);
                    ventana.color(COLORES.BLANCO);
                    mostrarInformacion(pato, ronda);
                    ventana.actualizaVentana();
                    ronda->rondaContinuar = ventana.muestraPreguntaParamInt("Ronda %d finalizada! :D. Continuar?", "Aviso", (ronda->totalRonda));
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

void mostrarInformacion(Pato *pato, Ronda *ronda)
{
    ventana.muestraTextoParametroInt(100, 600, "Puntaje: %d", 30, "Arial", (ronda->puntaje));
    ventana.muestraTextoParametroInt(ventana.anchoVentana() - 200, 600, "Ronda: %d", 30, "Arial", (ronda->totalRonda));
    ventana.muestraTextoParametroInt(((ventana.anchoVentana() / 2) - 100), 600, "Patos restantes: %d", 30, "Arial", (pato->totalPatos));
}

void iniciarMarcador()
{
    FILE *marcadores;
    marcadores = fopen("marcadores.bin", "wb");
    if (marcadores == NULL)
    {
        printf("No se pudo inicializar el marcador.");
        exit(1);
    }

    Puntuacion puntaje[5];

    fclose(marcadores);
}

void mostrarMarcadores(EstadoJuego *estadoJuego, Imagenes *imagenes)
{
    int coorBackBtnX = 0;
    int coorBackBtnY = 0;
    int rx, ry;
    ventana.reproducirAudio("./assets/audio/espera.wav");
    while (*estadoJuego == ESTADO_MARCADORES)
    {
        ventana.limpiaVentana();
        ventana.muestraImagenEscalada(0, 0, ventana.anchoVentana(), ventana.altoVentana(), imagenes->fondoImg);
        ventana.muestraImagenEscalada(coorBackBtnX, coorBackBtnY, 100, 100, imagenes->backButon);
        ventana.muestraImagenEscalada((ventana.anchoVentana() / 2) - 150, 10, 300, 150, imagenes->marksIcon);
        ventana.raton(&rx, &ry);
        ventana.actualizaVentana();
        ventana.espera(10);
        if (ventana.ratonBotonIzquierdo())
        {
            if (rx >= coorBackBtnX &&
                rx <= (coorBackBtnX + 100) &&
                ry >= coorBackBtnY &&
                ry <= (coorBackBtnY + 100))
            {
                ventana.reproducirAudio(NULL);
                ventana.LimpiarEstadoBotonIzquierdo();
                *estadoJuego = ESTADO_MENU;
            }
        }

        if (ventana.teclaPresionada() == TECLAS.ESCAPE)
        {
            ventana.reproducirAudio(NULL);
            exit(0);
        }
    }
}

void estadoPato(Pato *pato, Ronda *ronda)
{
    if (pato->estado == 1) // pato cayendo
    {
        pato->coorY += VELOCIDAD_CAIDA;

        if (pato->coorY >= 350)
        {
            pato->estado = 2; // pato fuera del mapa
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

void liberarImagenes(Imagenes *imagenes)
{
    ventana.eliminaImagen(imagenes->arbolImg);
    ventana.eliminaImagen(imagenes->arbolImg);
    ventana.eliminaImagen(imagenes->patoImg);
    ventana.eliminaImagen(imagenes->scoreImg);
    ventana.eliminaImagen(imagenes->logoImg);
    ventana.eliminaImagen(imagenes->startIcon);
    ventana.eliminaImagen(imagenes->fondoImg);
    ventana.eliminaImagen(imagenes->marksIcon);
}