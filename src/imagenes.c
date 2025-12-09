#include"imagenes.h"
#include"graficos.h"

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
    imagenes->patoCayendoImg = ventana.creaImagenConMascara("./assets/img/patoCayendo.bmp", "./assets/img/patoCayendoMascara.bmp");
    return imagenes;
}