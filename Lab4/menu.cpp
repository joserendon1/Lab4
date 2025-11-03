#include "menu.h"
#include <iostream>

using namespace std;

Menu::Menu(Red* red) : red(red) {}

void Menu::mostrarMenuPrincipal() {
    int opcion;
    string entrada;

    do {
        cout << "\n=========================================" << endl;
        cout << "      SISTEMA DE SIMULACION DE RED" << endl;
        cout << "=========================================" << endl;
        cout << "1. Gestionar Enrutadores" << endl;
        cout << "2. Agregar Conexion" << endl;
        cout << "3. Eliminar Conexion" << endl;
        cout << "4. Actualizar Costo de Conexion" << endl;
        cout << "5. Mostrar Tablas de Enrutamiento" << endl;
        cout << "6. Mostrar Topologia Completa" << endl;
        cout << "7. Cargar Red desde Archivo" << endl;
        cout << "8. Generar Red Aleatoria" << endl;
        cout << "9. Encontrar Camino Mas Corto" << endl;
        cout << "0. Salir" << endl;
        cout << "-----------------------------------------" << endl;
        cout << "Seleccione una opcion: ";

        cin >> entrada;

        bool esNumero = true;
        for (char c : entrada) {
            if (!isdigit(c)) {
                esNumero = false;
                break;
            }
        }

        if (esNumero) {
            opcion = stoi(entrada);
        } else {
            opcion = -1;
        }

        switch(opcion) {
        case 1:
            gestionarEnrutadores();
            break;
        case 2:
            gestionarConexiones();
            break;
        case 3:
            eliminarConexion();
            break;
        case 4:
            actualizarCostoConexion();
            break;
        case 5:
            mostrarInformacion();
            break;
        case 6:
            mostrarTopologia();
            break;
        case 7:
            cargarRedDesdeArchivo();
            break;
        case 8:
            generarRedAleatoria();
            break;
        case 9: {
            string origen, destino;
            cout << "\n=== ENCONTRAR CAMINO MAS CORTO ===" << endl;
            cout << "Origen: ";
            cin >> origen;
            cout << "Destino: ";
            cin >> destino;
            red->encontrarYMostrarCamino(origen, destino);
            break;
        }
        case 0:
            cout << "Saliendo del sistema..." << endl;
            break;
        default:
            cout << "Error: Opcion invalida! Por favor seleccione un numero entre 0 y 9." << endl;
            // Limpiar el estado de error de cin si es necesario
            cin.clear();
        }
    } while(opcion != 0);
}

void Menu::gestionarEnrutadores() {
    int opcion;
    string nombre;

    cout << "\n=== GESTION DE ENRUTADORES ===" << endl;
    cout << "1. Agregar Enrutador" << endl;
    cout << "2. Eliminar Enrutador" << endl;
    cout << "3. Listar Enrutadores Existentes" << endl;
    cout << "Seleccione una opcion: ";
    cin >> opcion;

    if (opcion == 1 || opcion == 2) {
        cout << "Nombre del enrutador: ";
        cin >> nombre;
    }

    switch(opcion) {
    case 1:
        if (red->existeEnrutador(nombre)) {
            cout << "Error: El enrutador '" << nombre << "' ya existe en la red." << endl;
        } else {
            red->agregarEnrutador(nombre);
            cout << "Enrutador '" << nombre << "' agregado exitosamente." << endl;
        }
        break;
    case 2:
        if (!red->existeEnrutador(nombre)) {
            cout << "Error: El enrutador '" << nombre << "' no existe en la red." << endl;
        } else {
            red->eliminarEnrutador(nombre);
            cout << "Enrutador '" << nombre << "' eliminado exitosamente." << endl;
        }
        break;
    case 3:
    {
        auto enrutadores = red->obtenerNombresEnrutadores();
        cout << "\nEnrutadores en la red (" << enrutadores.size() << "):" << endl;
        if (enrutadores.empty()) {
            cout << "No hay enrutadores en la red." << endl;
        } else {
            for(const auto& nom : enrutadores) {
                cout << "- " << nom << endl;
            }
        }
    }
    break;
    default:
        cout << "Error: Opcion invalida!" << endl;
    }
}

void Menu::gestionarConexiones() {
    string origen, destino;
    int costo;

    cout << "Origen: "; cin >> origen;
    cout << "Destino: "; cin >> destino;
    cout << "Costo: "; cin >> costo;

    red->agregarConexion(origen, destino, costo);
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

void Menu::eliminarConexion() {
    string origen, destino;

    cout << "\n=== ELIMINAR CONEXION ===" << endl;
    cout << "Origen: ";
    cin >> origen;
    cout << "Destino: ";
    cin >> destino;

    if (!red->existeEnrutador(origen)) {
        cout << "Error: El enrutador origen '" << origen << "' no existe" << endl;
        return;
    }

    if (!red->existeEnrutador(destino)) {
        cout << "Error: El enrutador destino '" << destino << "' no existe" << endl;
        return;
    }

    red->eliminarConexion(origen, destino);
    cout << "Conexion entre '" << origen << "' y '" << destino << "' eliminada." << endl;
}

void Menu::actualizarCostoConexion() {
    string origen, destino;
    int nuevoCosto;

    cout << "\n=== ACTUALIZAR COSTO DE CONEXION ===" << endl;
    cout << "Origen: ";
    cin >> origen;
    cout << "Destino: ";
    cin >> destino;
    cout << "Nuevo costo: ";
    cin >> nuevoCosto;

    if (nuevoCosto <= 0) {
        cout << "Error: El costo debe ser un numero positivo" << endl;
        return;
    }

    if (!red->existeEnrutador(origen)) {
        cout << "Error: El enrutador origen '" << origen << "' no existe" << endl;
        return;
    }

    if (!red->existeEnrutador(destino)) {
        cout << "Error: El enrutador destino '" << destino << "' no existe" << endl;
        return;
    }

    red->actualizarCostoConexion(origen, destino, nuevoCosto);
    cout << "Costo de conexion entre '" << origen << "' y '" << destino << "' actualizado a " << nuevoCosto << endl;
}

void Menu::mostrarTopologia() {
    cout << "\n=== TOPOLOGIA COMPLETA DE LA RED ===" << endl;
    red->mostrarTopologia();
}
