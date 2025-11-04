#include "menu.h"
#include <iostream>


Menu::Menu(Red* red) : red(red) {}

void Menu::mostrarMenuPrincipal() {
    int opcion;
    std::string entrada;

    do {
        std::cout << "\n=========================================" << std::endl;
        std::cout << "      SISTEMA DE SIMULACION DE RED" << std::endl;
        std::cout << "=========================================" << std::endl;
        std::cout << "1. Gestionar Enrutadores" << std::endl;
        std::cout << "2. Agregar Conexion" << std::endl;
        std::cout << "3. Eliminar Conexion" << std::endl;
        std::cout << "4. Actualizar Costo de Conexion" << std::endl;
        std::cout << "5. Mostrar Tablas de Enrutamiento" << std::endl;
        std::cout << "6. Mostrar Topologia Completa" << std::endl;
        std::cout << "7. Cargar Red desde Archivo" << std::endl;
        std::cout << "8. Generar Red Aleatoria" << std::endl;
        std::cout << "9. Encontrar Camino Mas Corto" << std::endl;
        std::cout << "0. Salir" << std::endl;
        std::cout << "-----------------------------------------" << std::endl;
        std::cout << "Seleccione una opcion: ";

        std::cin >> entrada;

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
            std::string origen, destino;
            std::cout << "\n=== ENCONTRAR CAMINO MAS CORTO ===" << std::endl;
            std::cout << "Origen: ";
            std::cin >> origen;
            std::cout << "Destino: ";
            std::cin >> destino;
            red->encontrarYMostrarCamino(origen, destino);
            break;
        }
        case 0:
            std::cout << "Saliendo del sistema..." << std::endl;
            break;
        default:
            std::cout << "Error: Opcion invalida! Por favor seleccione un numero entre 0 y 9." << std::endl;
            std::cin.clear();
        }
    } while(opcion != 0);
}

void Menu::gestionarEnrutadores() {
    int opcion;
    std::string nombre;

    std::cout << "\n=== GESTION DE ENRUTADORES ===" << std::endl;
    std::cout << "1. Agregar Enrutador" << std::endl;
    std::cout << "2. Eliminar Enrutador" << std::endl;
    std::cout << "3. Listar Enrutadores Existentes" << std::endl;
    std::cout << "Seleccione una opcion: ";
    std::cin >> opcion;

    if (opcion == 1 || opcion == 2) {
        std::cout << "Nombre del enrutador: ";
        std::cin >> nombre;
    }

    switch(opcion) {
    case 1:
        if (red->existeEnrutador(nombre)) {
            std::cout << "Error: El enrutador '" << nombre << "' ya existe en la red." << std::endl;
        } else {
            red->agregarEnrutador(nombre);
            std::cout << "Enrutador '" << nombre << "' agregado exitosamente." << std::endl;
        }
        break;
    case 2:
        if (!red->existeEnrutador(nombre)) {
            std::cout << "Error: El enrutador '" << nombre << "' no existe en la red." << std::endl;
        } else {
            red->eliminarEnrutador(nombre);
            std::cout << "Enrutador '" << nombre << "' eliminado exitosamente." << std::endl;
        }
        break;
    case 3:
    {
        auto enrutadores = red->obtenerNombresEnrutadores();
        std::cout << "\nEnrutadores en la red (" << enrutadores.size() << "):" << std::endl;
        if (enrutadores.empty()) {
            std::cout << "No hay enrutadores en la red." << std::endl;
        } else {
            for(const auto& nom : enrutadores) {
                std::cout << "- " << nom << std::endl;
            }
        }
    }
    break;
    default:
        std::cout << "Error: Opcion invalida!" << std::endl;
    }
}

void Menu::gestionarConexiones() {
    std::string origen, destino;
    int costo;

    std::cout << "Origen: "; std::cin >> origen;
    std::cout << "Destino: "; std::cin >> destino;
    std::cout << "Costo: "; std::cin >> costo;

    red->agregarConexion(origen, destino, costo);
}

void Menu::mostrarInformacion() {
    auto enrutadores = red->obtenerNombresEnrutadores();

    std::cout << "\nEnrutadores en la red:" << std::endl;
    for(const auto& nombre : enrutadores) {
        red->imprimirTablaEnrutador(nombre);
    }
}

void Menu::cargarRedDesdeArchivo() {
    std::string nombreArchivo;
    std::cout << "Nombre del archivo: ";
    std::cin >> nombreArchivo;
    red->cargarDesdeArchivo(nombreArchivo);
}

void Menu::generarRedAleatoria() {
    int numEnrutadores, costoMaximo;
    double probabilidad;

    std::cout << "Numero de enrutadores: ";
    std::cin >> numEnrutadores;
    std::cout << "Probabilidad de conexion (0-1): ";
    std::cin >> probabilidad;
    std::cout << "Costo maximo: ";
    std::cin >> costoMaximo;

    red->generarRedAleatoria(numEnrutadores, probabilidad, costoMaximo);
}

void Menu::eliminarConexion() {
    std::string origen, destino;

    std::cout << "\n=== ELIMINAR CONEXION ===" << std::endl;
    std::cout << "Origen: ";
    std::cin >> origen;
    std::cout << "Destino: ";
    std::cin >> destino;

    if (!red->existeEnrutador(origen)) {
        std::cout << "Error: El enrutador origen '" << origen << "' no existe" << std::endl;
        return;
    }

    if (!red->existeEnrutador(destino)) {
        std::cout << "Error: El enrutador destino '" << destino << "' no existe" << std::endl;
        return;
    }

    red->eliminarConexion(origen, destino);
    std::cout << "Conexion entre '" << origen << "' y '" << destino << "' eliminada." << std::endl;
}

void Menu::actualizarCostoConexion() {
    std::string origen, destino;
    int nuevoCosto;

    std::cout << "\n=== ACTUALIZAR COSTO DE CONEXION ===" << std::endl;
    std::cout << "Origen: ";
    std::cin >> origen;
    std::cout << "Destino: ";
    std::cin >> destino;
    std::cout << "Nuevo costo: ";
    std::cin >> nuevoCosto;

    if (nuevoCosto <= 0) {
        std::cout << "Error: El costo debe ser un numero positivo" << std::endl;
        return;
    }

    if (!red->existeEnrutador(origen)) {
        std::cout << "Error: El enrutador origen '" << origen << "' no existe" << std::endl;
        return;
    }

    if (!red->existeEnrutador(destino)) {
        std::cout << "Error: El enrutador destino '" << destino << "' no existe" << std::endl;
        return;
    }

    red->actualizarCostoConexion(origen, destino, nuevoCosto);
    std::cout << "Costo de conexion entre '" << origen << "' y '" << destino << "' actualizado a " << nuevoCosto << std::endl;
}

void Menu::mostrarTopologia() {
    std::cout << "\n=== TOPOLOGIA COMPLETA DE LA RED ===" << std::endl;
    red->mostrarTopologia();
}
