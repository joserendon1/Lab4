#include "menu.h"
#include <iostream>

using namespace std;

Menu::Menu(Red* red) : red(red) {}

void Menu::mostrarMenuPrincipal() {
    int opcion;
    do {
        cout << "\n=== SISTEMA DE SIMULACION DE RED ===" << endl;
        cout << "1. Gestionar Enrutadores" << endl;
        cout << "2. Gestionar Conexiones" << endl;
        cout << "3. Mostrar Informacion" << endl;
        cout << "4. Cargar Red desde Archivo" << endl;
        cout << "5. Generar Red Aleatoria" << endl;
        cout << "6. Encontrar Camino Mas Corto" << endl;
        cout << "0. Salir" << endl;
        cout << "Seleccione una opcion: ";
        cin >> opcion;

        switch(opcion) {
        case 1: gestionarEnrutadores(); break;
        case 2: gestionarConexiones(); break;
        case 3: mostrarInformacion(); break;
        case 4: cargarRedDesdeArchivo(); break;
        case 5: generarRedAleatoria(); break;
        case 6: {
            string origen, destino;
            cout << "Origen: "; cin >> origen;
            cout << "Destino: "; cin >> destino;
            red->encontrarYMostrarCamino(origen, destino);
            break;
        }
        case 0: cout << "Saliendo..." << endl; break;
        default: cout << "Opcion invalida!" << endl;
        }
    } while(opcion != 0);
}

void Menu::gestionarEnrutadores() {
    int opcion;
    string nombre;

    cout << "\n1. Agregar Enrutador" << endl;
    cout << "2. Eliminar Enrutador" << endl;
    cout << "Seleccione: ";
    cin >> opcion;

    cout << "Nombre del enrutador: ";
    cin >> nombre;

    if(opcion == 1) {
        red->agregarEnrutador(nombre);
        cout << "Enrutador " << nombre << " agregado." << endl;
    } else if(opcion == 2) {
        red->eliminarEnrutador(nombre);
        cout << "Enrutador " << nombre << " eliminado." << endl;
    }
}

void Menu::gestionarConexiones() {
    string origen, destino;
    int costo;

    cout << "Origen: "; cin >> origen;
    cout << "Destino: "; cin >> destino;
    cout << "Costo: "; cin >> costo;

    red->agregarConexion(origen, destino, costo);
    cout << "Conexion agregada." << endl;
}

void Menu::mostrarInformacion() {
    auto enrutadores = red->obtenerNombresEnrutadores();

    cout << "\nEnrutadores en la red:" << endl;
    for(const auto& nombre : enrutadores) {
        red->imprimirTablaEnrutador(nombre);
    }
}

void Menu::cargarRedDesdeArchivo() {
    string nombreArchivo;
    cout << "Nombre del archivo: ";
    cin >> nombreArchivo;
    red->cargarDesdeArchivo(nombreArchivo);
}

void Menu::generarRedAleatoria() {
    int numEnrutadores, costoMaximo;
    double probabilidad;

    cout << "Numero de enrutadores: ";
    cin >> numEnrutadores;
    cout << "Probabilidad de conexion (0-1): ";
    cin >> probabilidad;
    cout << "Costo maximo: ";
    cin >> costoMaximo;

    red->generarRedAleatoria(numEnrutadores, probabilidad, costoMaximo);
}
