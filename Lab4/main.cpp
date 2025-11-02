#include <iostream>
#include "red.h"

using namespace std;

int main() {
    Red red("MiRed");

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

    cout << "=== RED EJEMPLO ===" << endl;

    red.encontrarYMostrarCamino("A", "C");
    red.imprimirTablaEnrutador("A");

    cout << "\n=== RED ALEATORIA ===" << endl;

    Red redAleatoria("RedAleatoria");
    redAleatoria.generarRedAleatoria(6, 0.5, 15);
    redAleatoria.encontrarYMostrarCamino("E0", "E3");
    redAleatoria.imprimirTablaEnrutador("E0");

    return 0;
}
