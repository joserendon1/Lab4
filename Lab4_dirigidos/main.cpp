#include <iostream>
#include "red_dirigida.h"
#include "menu_dirigido.h"

int main() {

    RedDirigida red("Red con Conexiones Dirigidas");
    MenuDirigido menu(&red);
    menu.mostrarMenuPrincipal();

    return 0;
}
