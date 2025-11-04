#include "menu_dirigido.h"
#include <iostream>

MenuDirigido::MenuDirigido(RedDirigida* red) : red(red) {}

void MenuDirigido::mostrarMenuPrincipal() {
    int opcion;
    std::string entrada;

    do {
        std::cout << "\n=========================================" << std::endl;
        std::cout << "    SISTEMA DE RED CONEXIONES DIRIGIDAS" << std::endl;
        std::cout << "=========================================" << std::endl;
        std::cout << "1. Gestionar Enrutadores" << std::endl;
        std::cout << "2. Agregar Conexion Dirigida" << std::endl;
        std::cout << "3. Agregar Conexion Bidireccional" << std::endl;
        std::cout << "4. Eliminar Conexion" << std::endl;
        std::cout << "5. Actualizar Costo de Conexion" << std::endl;
        std::cout << "6. Mostrar Tablas de Enrutamiento" << std::endl;
        std::cout << "7. Mostrar Topologia Completa" << std::endl;
        std::cout << "8. Cargar Red desde Archivo" << std::endl;
        std::cout << "9. Generar Red Aleatoria" << std::endl;
        std::cout << "10. Encontrar Camino Mas Corto" << std::endl;
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
            opcion = std::stoi(entrada);
        } else {
            opcion = -1;
        }

        switch(opcion) {
        case 1: gestionarEnrutadores(); break;
        case 2: gestionarConexiones(); break;
        case 3:
        {
            std::string nodo1, nodo2;
            int costo1, costo2;
            std::cout << "\n=== AGREGAR CONEXION BIDIRECCIONAL ===" << std::endl;
            std::cout << "Nodo 1: "; std::cin >> nodo1;
            std::cout << "Nodo 2: "; std::cin >> nodo2;
            std::cout << "Costo de " << nodo1 << " a " << nodo2 << ": "; std::cin >> costo1;
            std::cout << "Costo de " << nodo2 << " a " << nodo1 << ": "; std::cin >> costo2;
            red->agregarConexionBidireccional(nodo1, nodo2, costo1, costo2);
        }
        break;
        case 4:
        {
            std::string origen, destino;
            std::cout << "\n=== ELIMINAR CONEXION ===" << std::endl;
            std::cout << "Origen: "; std::cin >> origen;
            std::cout << "Destino: "; std::cin >> destino;
            red->eliminarConexion(origen, destino);
            std::cout << "Conexion eliminada." << std::endl;
        }
        break;
        case 5: actualizarCostoConexion(); break;
        case 6: mostrarInformacion(); break;
        case 7: mostrarTopologia(); break;
        case 8: cargarRedDesdeArchivo(); break;
        case 9: generarRedAleatoria(); break;
        case 10: encontrarCaminoMasCorto(); break;
        case 0:
            std::cout << "Saliendo del sistema..." << std::endl;
            break;
        default:
            std::cout << "Error: Opcion invalida! Por favor seleccione 0-7." << std::endl;
            std::cin.clear();
        }
    } while(opcion != 0);
}

void MenuDirigido::gestionarEnrutadores() {
    int opcion;
    std::string nombre;

    std::cout << "\n=== GESTION DE ENRUTADORES ===" << std::endl;
    std::cout << "1. Agregar Enrutador" << std::endl;
    std::cout << "2. Eliminar Enrutador" << std::endl;
    std::cout << "3. Listar Enrutadores" << std::endl;
    std::cout << "Seleccione: ";
    std::cin >> opcion;

    if (opcion == 1 || opcion == 2) {
        std::cout << "Nombre del enrutador: ";
        std::cin >> nombre;
    }

    switch(opcion) {
    case 1:
        red->agregarEnrutador(nombre);
        break;
    case 2:
        red->eliminarEnrutador(nombre);
        break;
    case 3:
    {
        auto enrutadores = red->obtenerNombresEnrutadores();
        std::cout << "\nEnrutadores en la red (" << enrutadores.size() << "):" << std::endl;
        for(const auto& nom : enrutadores) {
            std::cout << "- " << nom << std::endl;
        }
    }
    break;
    default:
        std::cout << "Opcion invalida!" << std::endl;
    }
}

void MenuDirigido::gestionarConexiones() {
    std::string origen, destino;
    int costo;

    std::cout << "\n=== AGREGAR CONEXION DIRIGIDA ===" << std::endl;
    std::cout << "Origen: "; std::cin >> origen;
    std::cout << "Destino: "; std::cin >> destino;
    std::cout << "Costo: "; std::cin >> costo;

    red->agregarConexionDirigida(origen, destino, costo);
}

void MenuDirigido::mostrarInformacion() {
    auto enrutadores = red->obtenerNombresEnrutadores();

    std::cout << "\nEnrutadores en la red:" << std::endl;
    for(const auto& nombre : enrutadores) {
        red->imprimirTablaEnrutador(nombre);
    }
}

void MenuDirigido::mostrarTopologia() {
    red->mostrarTopologia();
}

void MenuDirigido::encontrarCaminoMasCorto() {
    std::string origen, destino;
    std::cout << "\n=== ENCONTRAR CAMINO MAS CORTO ===" << std::endl;
    std::cout << "Origen: "; std::cin >> origen;
    std::cout << "Destino: "; std::cin >> destino;
    red->encontrarYMostrarCamino(origen, destino);
}

void MenuDirigido::cargarRedDesdeArchivo() {
    std::string nombreArchivo;
    std::cout << "\n=== CARGAR RED DESDE ARCHIVO ===" << std::endl;
    std::cout << "Nombre del archivo: ";
    std::cin >> nombreArchivo;
    red->cargarDesdeArchivo(nombreArchivo);
}

void MenuDirigido::generarRedAleatoria() {
    int numEnrutadores, costoMaximo;
    double probabilidad;

    std::cout << "\n=== GENERAR RED ALEATORIA ===" << std::endl;
    std::cout << "Numero de enrutadores: ";
    std::cin >> numEnrutadores;
    std::cout << "Probabilidad de conexion (0-1): ";
    std::cin >> probabilidad;
    std::cout << "Costo maximo: ";
    std::cin >> costoMaximo;

    red->generarRedAleatoria(numEnrutadores, probabilidad, costoMaximo);
}

void MenuDirigido::actualizarCostoConexion() {
    std::string origen, destino;
    int nuevoCosto;

    std::cout << "\n=== ACTUALIZAR COSTO DE CONEXION ===" << std::endl;
    std::cout << "Origen: "; std::cin >> origen;
    std::cout << "Destino: "; std::cin >> destino;
    std::cout << "Nuevo costo: "; std::cin >> nuevoCosto;

    red->actualizarCostoConexion(origen, destino, nuevoCosto);
}
