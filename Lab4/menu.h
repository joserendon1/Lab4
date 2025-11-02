#ifndef MENU_H
#define MENU_H

#include "red.h"
#include <iostream>

class Menu {
private:
    Red* red;

public:
    Menu(Red* red);
    void mostrarMenuPrincipal();
    void gestionarEnrutadores();
    void gestionarConexiones();
    void mostrarInformacion();
    void cargarRedDesdeArchivo();
    void generarRedAleatoria();
};

#endif
