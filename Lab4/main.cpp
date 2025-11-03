#include <iostream>
#include "red.h"
#include "menu.h"



int main() {
    Red red("MiRed");
    Menu menu(&red);

    std::cout << "=== CONFIGURACION INICIAL ===" << std::endl;
    red.agregarEnrutador("A");
    red.agregarEnrutador("B");
    red.agregarEnrutador("C");
    red.agregarEnrutador("D");

    red.agregarConexion("A", "B", 4);
    red.agregarConexion("A", "C", 10);
    red.agregarConexion("A", "D", 5);
    red.agregarConexion("B", "C", 3);
    red.agregarConexion("B", "D", 1);
    red.agregarConexion("C", "D", 2);

    red.encontrarYMostrarCamino("A", "C");
    red.imprimirTablaEnrutador("A");

    menu.mostrarMenuPrincipal();

    return 0;
}
