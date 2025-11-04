#ifndef MENU_DIRIGIDO_H
#define MENU_DIRIGIDO_H

#include "red_dirigida.h"
#include <iostream>

class MenuDirigido {
private:
    RedDirigida* red;

public:
    MenuDirigido(RedDirigida* red);
    void mostrarMenuPrincipal();
    void gestionarEnrutadores();
    void gestionarConexiones();
    void mostrarInformacion();
    void mostrarTopologia();
    void encontrarCaminoMasCorto();
    void cargarRedDesdeArchivo();
    void generarRedAleatoria();
    void actualizarCostoConexion();
};

#endif
