#include "juego.h"
#include "imagenes.h"
#include "marcadores.h"

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
    if (nuevoJugador.puntaje <= puntajeActual[MAX_JUAGDORES - 1].puntaje)
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