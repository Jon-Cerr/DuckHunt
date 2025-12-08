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
#define MAX_JUAGDORES 5

#define AJUSTEX 0.45
#define AJUSTEY 0.35

typedef struct Pato
{
    int coorX;
    int coorY;
    int estado;
    int alto;
    int ancho;
    int velX;
    int velY;
    int totalPatos;
} Pato;

typedef struct Ronda
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
} Imagenes;

typedef struct Puntuacion
{
    int puntaje;
    int nombreJugador;
} Puntuacion;

typedef struct Juego
{
    Pato *pato;
    Puntuacion *puntuacion;
    Ronda *ronda;
} Juego;

/**
 * @brief Funcion que crea una instancia de un tipo de dato struct pato con todo y sus campos y que retorna el puntero completo a la struct
 * 
 * @return Pato* 
 */
Pato *crearPato();

/**
 * @brief Funcion que crea una instancia de un tipo de dato struct ronda con todo y sus campos y que retorna el puntero completo a la struct
 * 
 * @return Ronda* 
 */
Ronda *crearRonda();

/**
 * @brief Funcion que crea una instancia de un tipo de dato struct imagenes con todo y sus campos y que retorna el puntero completo a la struct
 * 
 * @return Imagenes* 
 */
Imagenes *crearImagenes();

/**
 * @brief Funcion que inicia el juego en estado menu, tiene dos opciones: iniciar juego y ver marcadores. Dibuja el logo y los botones, ademas de reproducir audio
 * @param juego Puntero a la struct Juego que contiene puntero a las stucts Pato, Ronda y Puntuacion
 * @param imagenes Puntero a la struct Imagenes que contiene las direcciones de imagenes a dibujar
 * @param estadoJuego Puntero a la enumeracion EstadoJuego que maneja los estaaos del juego: menu, inicio y fin
 */
void iniciarJuego(Juego *juego, Imagenes *imagenes, EstadoJuego *estadoJuego);

/**
 * @brief Funcion que ejecuta el ciclo del juego, internamente dibuja el estado del pato, las imagenes, asi como la puntuacion, ronda y patos restantes.
 * @param imagenes Puntero a la struct Imagenes que contiene las direcciones de imagenes a dibujar
 * @param juego Puntero a la struct Juego que contiene puntero a las stucts Pato, Ronda y Puntuacion
 */
void gameLoop(Imagenes *imagenes, Juego *juego);

/**
 * @brief Funcion encargada de dibujar el escenario
 *
 * @param imagenes Puntero a la struct Imagenes que contiene las direcciones de imagenes a dibujar
 */
void dibujarEscenarioRes1(Imagenes *imagenes);

/**
 * @brief Funcion que se encarga de dibujar al pato en pantalla a corde al estado del mismo
 *
 * @param imagenes Puntero a la struct Imagenes que contiene las direcciones de imagenes a dibujar
 * @param pato Puntero a la struct pato que contiene las caracteristicas de un pato
 */
void dibujarPato(Imagenes *imagenes, Pato *pato);

/**
 * @brief Funcion encargada de dibuajr al pato en pantalla, asignandole coordenadas y velocidades si se cumplen ciertos criterios
 *
 * @param pato Puntero a la struct pato que contiene las caracteristicas de un pato
 */
void vueloPato(Pato *pato);

/**
 * @brief Funcion encargada de ejecutar la logica de disparo hacia el pato, las coordenadas del mouse y click izquierdo para disparar
 *
 * @param juego Puntero a la struct Juego que contiene las structs Pato y Ronda
 * @param imagenes Puntero a la struct Imagenes que contiene las direcciones de imagenes a dibujar
 * @return true si el jugador acepta otra ronda
 * @return false Si el pato no fue disparado
 */
bool dispararPato(Juego *juego, Imagenes *imagenes);

/**
 * @brief Dibuja una mira en la  pantalla acorde a las coordenadas del mouse
 *
 */
void dibujarMira();

/**
 * @brief Nuestra la informacion de puntaje, totalRonda y totalPatos en la ventana
 *
 * @param pato Puntero a la struct Pato que contiene las caracteristicas de un pato
 * @param ronda Puntero a la struct Ronda que contiene la informacion de la ronda
 */
void mostrarInformacion(Juego *juego);

/**
 * @brief Funcion que dibuja en pantalla el esatdo marcadores, es decir, el apartado de puntuacion. Dibuja un boton, icono y el puntaje tomado de un archivo binario, con musica de fondo.
 *
 * @param estadoJuego Puntero a la enum EstadoJuego que contiene los estados del juego
 * @param imagenes Puntero a la struct Imagenes que contiene las direcciones de imagenes a dibujar
 * @param juego Puntero a la struct Juego que contiene puntero a las stucts Pato, Ronda y Puntuacion
 */
void mostrarMarcadores(EstadoJuego *estadoJuego, Imagenes *imagenes, Juego *juego);

/**
 * @brief Funcion que trabaja la logica del estado del pato, si se encuntra cayendo (1), si esta fuera del mapa (2), y si el pato esta fuera del mapa, dibuja uno nuevo en coordenadas aleatorias y velocidad aleatoria
 *
 * @param Juego Puntero a la struct Juego que contiene las estructuras Pato y Ronda
 * @param imagenes Puntero a la struct Imagenes que contiene las direcciones de imagenes a dibujar
 */
void estadoPato(Juego *juego, Imagenes *imagenes);

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
    imagenes->fondoJuegoImg = ventana.creaImagen("./assets/img/fondoJuego.bmp");
    imagenes->logoImg = ventana.creaImagenConMascara("./assets/img/logo.bmp", "./assets/img/logoMascara.bmp");
    imagenes->startIcon = ventana.creaImagenConMascara("./assets/img/startIcon.bmp", "./assets/img/startIconMascara.bmp");
    imagenes->fondoImg = ventana.creaImagen("./assets/img/fondoI.bmp");
    imagenes->marksIcon = ventana.creaImagenConMascara("./assets/img/marcadores.bmp", "./assets/img/marcadoresMascara.bmp");
    imagenes->backButon = ventana.creaImagenConMascara("./assets/img/backBtn.bmp", "./assets/img/backBtnMascara.bmp");
    imagenes->pisoImg = ventana.creaImagen("./assets/img/piso.bmp");
    imagenes->patoAbatidoImg = ventana.creaImagenConMascara("./assets/img/patoAbatido.bmp", "./assets/img/patoAbatidoMascara.bmp");
    return imagenes;
}

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

    if (juego->ronda->rondaContinuar && juego->pato->estado != 2)
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

void dibujarPato(Imagenes *imagenes, Pato *pato)
{
    Imagen *imagenPatoEstado;
    if (pato->estado == 1)
    {
        imagenPatoEstado = imagenes->patoAbatidoImg;
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

void mostrarMarcadores(EstadoJuego *estadoJuego, Imagenes *imagenes, Juego *juego)
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
        int posY_inicial = 200;
        int separacionY = 50;

        ventana.color(COLORES.NEGRO);

        ventana.texto2(250, posY_inicial - 50, "POS.", 30, "Arial", false, true, false);
        ventana.texto1((ventana.anchoVentana() / 2) - 150, posY_inicial - 50, "RONDA(S)", 30, "Arial");
        ventana.texto1((ventana.anchoVentana() / 2) + 150, posY_inicial - 50, "PUNTAJE", 30, "Arial");

        for (int i = 0; i < MAX_JUAGDORES; i++)
        {
            ventana.muestraTextoParametroInt(250, posY_inicial + (i * separacionY), "%d.", 30, "Arial", i + 1);

            ventana.muestraTextoParametroInt(
                (ventana.anchoVentana() / 2) - 150,
                posY_inicial + (i * separacionY),
                "%d",
                30, "Arial",
                juego->puntuacion[i].nombreJugador);

            ventana.muestraTextoParametroInt(
                (ventana.anchoVentana() / 2) + 150,
                posY_inicial + (i * separacionY),
                "%d",
                30, "Arial",
                juego->puntuacion[i].puntaje);
        }

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

void estadoPato(Juego *juego, Imagenes *imagenes)
{
    if (juego->pato->estado == 1) // pato cayendo
    {
        juego->pato->coorY += VELOCIDAD_CAIDA;
        if (juego->pato->coorY >= 400)
        {
            juego->pato->estado = 2; // pato fuera del mapa
        }
    }
    else if (juego->pato->estado == 2)
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

void inicializarMarcador(Puntuacion puntuaciones[MAX_JUAGDORES])
{
    FILE *archivoMarcador = fopen("marcadores.bin", "rb");
    if (archivoMarcador != NULL)
    {
        fread(puntuaciones, sizeof(Puntuacion), MAX_JUAGDORES, archivoMarcador);
        fclose(archivoMarcador);
    }
    else
    {
        for (int i = 0; i < MAX_JUAGDORES; i++)
        {
            puntuaciones[i].nombreJugador = 0;
            puntuaciones[i].puntaje = 0;
        }
        archivoMarcador = fopen("marcadores.bin", "wb");
        if (archivoMarcador != NULL)
        {
            fwrite(puntuaciones, sizeof(Puntuacion), MAX_JUAGDORES, archivoMarcador);
            fclose(archivoMarcador);
        }
        else
        {
            printf("No se pudo guardar el marcador.\n");
            exit(1);
        }
    }
}

void actualizarMarcador(Puntuacion puntajeActual[], Puntuacion nuevoJugador)
{
    if (nuevoJugador.puntaje < puntajeActual[MAX_JUAGDORES - 1].puntaje)
    {
        ventana.muestraMensaje1("Booooooo!", "No entraste al top");
        printf("No entraste al top unu.\n");
        return;
    }
    ventana.muestraMensajeParametroInt("Calificaste! Puntuacion: %d", nuevoJugador.puntaje, "Resultados");
    printf("Calificaste!\n");
    puntajeActual[MAX_JUAGDORES - 1] = nuevoJugador;
    ordenamientoBurbuja(puntajeActual);
    FILE *archivoMarcador = fopen("marcadores.bin", "wb");
    if (archivoMarcador != NULL)
    {
        fwrite(puntajeActual, sizeof(Puntuacion), MAX_JUAGDORES, archivoMarcador);
        fclose(archivoMarcador);
        printf("Marcador actualizado.\n");
    }
    else
    {
        printf("Error al abrir el marcador.\n");
    }
}

void ordenamientoBurbuja(Puntuacion puntajeActual[])
{
    Puntuacion aux;
    // Aplicando el metodo de la burbuja
    for (int i = 0; i < MAX_JUAGDORES - 1; i++)
    {
        for (int j = 0; j < MAX_JUAGDORES - i - 1; j++)
        {
            if (puntajeActual[j].puntaje < puntajeActual[j + 1].puntaje)
            {
                aux = puntajeActual[j];
                puntajeActual[j] = puntajeActual[j + 1];
                puntajeActual[j + 1] = aux;
            }
        }
    }
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