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

#include <stdlib.h>
#include <time.h>
#define VELOCIDAD_CAIDA 8
#define MIN_Y_APARICION 50
#define MAX_Y_APARICION 300
#define JX 15
#define JY 2
#define SW 4
#define MOTOR 13

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

typedef struct
{
    Pato pato;
    Ronda ronda;
} Juego;

typedef enum
{
    ESTADO_MENU,
    ESTADO_JUGANDO,
    ESTADO_FIN,
} EstadoJuego;

Pato *crearPato();
Ronda *crearRonda();
void iniciarJuego(Imagen *logoImg, EstadoJuego *estadoJuego);
void dibujarEscenarioRes1(Imagen *piso, Imagen *arbol);
void dibujarPato(Imagen *patoImg, Pato *pato);
void vueloPato(Pato *pato);
bool dispararPato(Pato *pato, Ronda *ronda, Imagen *pisoImg, Imagen *arbolImg, Imagen *scoreImg);
void gameLoop(Imagen *patoImg, Imagen *arbol, Imagen *piso, Pato *pato, Imagen *scoreImg, Ronda *ronda);
void dibujarMira();
void mostrarInformacion(Pato *pato, Ronda *ronda);
void estadoPato(Imagen *patoImg, Pato *pato, Ronda *ronda);

int main()
{
    srand(time(0));
    bool fs = false;
    Imagen *pisoImg = ventana.creaImagen("./piso.bmp");
    Imagen *arbolImg = ventana.creaImagen("./arbol.bmp");
    Imagen *patoImg = ventana.creaImagenConMascara("./pato1.bmp", "./patoMascara.bmp");
    Imagen *scoreImg = ventana.creaImagenConMascara("./score.bmp", "./scoreMascara.bmp");
    Imagen *logoImg = ventana.creaImagenConMascara("./logo.bmp", "./logoMascara.bmp");

    Pato *pato = crearPato();
    Ronda *ronda = crearRonda();

    EstadoJuego estadoJuego = ESTADO_MENU;

    ventana.tamanioVentana(1280, 720);
    ventana.tituloVentana("Duck Hunt");
    int tecla = ventana.teclaPresionada();

    iniciarJuego(logoImg, &estadoJuego);
    ventana.colorFondo(COLORES.AZULC);

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
            gameLoop(patoImg, arbolImg, pisoImg, pato, scoreImg, ronda);
        }

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
    free(ronda);
    return 0;
}

void iniciarJuego(Imagen *logoImg, EstadoJuego *estadoJuego)
{
    int coorBtnX = (ventana.anchoVentana() / 2) - (300 / 2);
    int coorBtnY = (ventana.altoVentana() / 2) + 80;
    int rx, ry;

    ventana.reproducirAudio("./audio.wav");
    while (*estadoJuego == ESTADO_MENU)
    {
        Imagen *startIcon = ventana.creaImagenConMascara("./startIcon.bmp", "./startIconMascara.bmp");
        Imagen *fondoImg = ventana.creaImagen("./fondoInicio.bmp");
        ventana.limpiaVentana();
        ventana.muestraImagenEscalada(0, 0, ventana.anchoVentana(), ventana.altoVentana(), fondoImg);
        ventana.muestraImagenEscalada(coorBtnX - 50, coorBtnY - 450, 400, 400, logoImg);
        ventana.color(COLORES.BLANCO);
        ventana.muestraImagenEscalada(coorBtnX - 10, coorBtnY - 10, 350, 100, startIcon);
        ventana.raton(&rx, &ry);
        ventana.actualizaVentana();
        if (ventana.ratonBotonIzquierdo())
        {
            if (rx >= coorBtnX &&
                rx <= (coorBtnX + 300) &&
                ry >= coorBtnY &&
                ry <= (coorBtnY + 80))
            {
                ventana.reproducirAudio(NULL);
                ventana.LimpiarEstadoBotonIzquierdo();
                *estadoJuego = ESTADO_JUGANDO;
            }
        }
        if (ventana.teclaPresionada() == TECLAS.ESCAPE)
        {
            break;
            exit(0);
        }
    }
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

    //ventana.reproducirAudio(NULL);

    if (botonPres && pato->estado == 0)
    {
        ventana.reproducirAudio("./shot.wav");
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
                pato->mostrarScore = true;
                ronda->mostrarRonda = true;
                if (pato->totalPatos == 0)
                {
                    dibujarEscenarioRes1(pisoImg, arbolImg);
                    ventana.muestraImagenEscalada(150, ventana.altoVentana() - 200, 50, 50, scoreImg);
                    ventana.color(COLORES.BLANCO);
                    mostrarInformacion(pato, ronda);
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

    ventana.color(COLORES.BLANCO);
    if (pato->mostrarScore && ronda->mostrarRonda)
    {
        mostrarInformacion(pato, ronda);
    }

    ventana.color(COLORES.NEGRO);
    dibujarMira();
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

void estadoPato(Imagen *patoImg, Pato *pato, Ronda *ronda)
{
    if (pato->estado == 1) // pato cayendo
    {
        pato->coorY += VELOCIDAD_CAIDA;

        if (pato->coorY >= 350)
        {
            pato->estado = 2; // pato fuera del mapa

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

void disparar()
{
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

    ventana.color(COLORES.CYAN);
    ventana.circulo(rx, ry, 20);
    ventana.linea(rx - 10, ry, rx - 30, ry);
    ventana.linea(rx + 10, ry, rx + 30, ry);
    ventana.linea(rx, ry - 10, rx, ry - 30);
    ventana.linea(rx, ry + 10, rx, ry + 30);
    ventana.circuloRelleno(rx, ry, 2);

    if (btn)
    {
        esp32->digitalWrite(esp32, MOTOR, true);
    }
    else
    {
        esp32->digitalWrite(esp32, MOTOR, false);
    }
}