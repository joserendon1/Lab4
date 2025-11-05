#include "menu_dirigido.h"
#include <iostream>
#include <stdexcept>
#include <cctype>
#include <limits>


MenuDirigido::MenuDirigido(RedDirigida* red) : red(red) {}

void MenuDirigido::mostrarMenuPrincipal() {
    int opcion;
    std::string entrada;

    do {
        std::cout << "\n=========================================" << std::endl;
        std::cout << "    SISTEMA DE RED CONEXIONES DIRIGIDAS" << std::endl;
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
        std::cout << "10. Mostrar Formato de Archivo" << std::endl;
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
            std::string origen, destino;
            std::cout << "\n=== ELIMINAR CONEXION ===" << std::endl;
            std::cout << "Origen: "; std::cin >> origen;
            std::cout << "Destino: "; std::cin >> destino;
            red->eliminarConexion(origen, destino);
        }
        break;
        case 4: actualizarCostoConexion(); break;
        case 5: mostrarInformacion(); break;
        case 6: mostrarTopologia(); break;
        case 7: cargarRedDesdeArchivo(); break;
        case 8: generarRedAleatoria(); break;
        case 9: encontrarCaminoMasCorto(); break;
        case 10: red->mostrarFormatoArchivo(); break;
        case 0:
            std::cout << "Saliendo del sistema" << std::endl;
            break;
        default:
            std::cout << "Error: Opcion invalida. Por favor seleccione 0-10." << std::endl;
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

    if (!(std::cin >> opcion)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cerr << "Error: Entrada invalida" << std::endl;
        return;
    }

    if (opcion == 1 || opcion == 2) {
        std::cout << "Nombre del enrutador: ";
        if (!(std::cin >> nombre)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cerr << "Error: Nombre invalido" << std::endl;
            return;
        }
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
        std::cerr << "Error: Opcion invalida" << std::endl;
    }
}

void MenuDirigido::gestionarConexiones() {
    std::string origen, destino;
    int costo;

    std::cout << "\n=== AGREGAR CONEXION DIRIGIDA ===" << std::endl;
    std::cout << "Origen: ";
    std::cin >> origen;
    std::cout << "Destino: ";
    std::cin >> destino;
    std::cout << "Costo: ";

    std::string entradaCosto;
    std::cin >> entradaCosto;

    bool esNumeroValido = true;
    for (char c : entradaCosto) {
        if (!std::isdigit(c)) {
            esNumeroValido = false;
            break;
        }
    }

    if (!esNumeroValido) {
        std::cerr << "Error: El costo debe ser un numero entero positivo" << std::endl;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return;
    }

    try {
        costo = std::stoi(entradaCosto);
    } catch (const std::out_of_range& e) {
        std::cerr << "Error: El costo es demasiado grande (maximo: " << std::numeric_limits<int>::max() << ")" << std::endl;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return;
    } catch (const std::invalid_argument& e) {
        std::cerr << "Error: Entrada invalida para el costo" << std::endl;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return;
    }

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
    std::cout << "Origen: ";
    if (!(std::cin >> origen)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cerr << "Error: Entrada invalida para origen" << std::endl;
        return;
    }

    std::cout << "Destino: ";
    if (!(std::cin >> destino)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cerr << "Error: Entrada invalida para destino" << std::endl;
        return;
    }

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

    std::cout << "Numero de enrutadores (2-100): ";
    if (!(std::cin >> numEnrutadores) || numEnrutadores < 2 || numEnrutadores > 100) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cerr << "Error: El numero de enrutadores debe estar entre 2 y 100" << std::endl;
        return;
    }

    std::cout << "Probabilidad de conexion (0.1-1.0): ";
    if (!(std::cin >> probabilidad) || probabilidad < 0.1 || probabilidad > 1.0) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cerr << "Error: La probabilidad debe estar entre 0.1 y 1.0" << std::endl;
        return;
    }

    std::cout << "Costo maximo (1-1000): ";
    if (!(std::cin >> costoMaximo) || costoMaximo < 1 || costoMaximo > 1000) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cerr << "Error: El costo maximo debe estar entre 1 y 1000" << std::endl;
        return;
    }

    red->generarRedAleatoria(numEnrutadores, probabilidad, costoMaximo);
}

void MenuDirigido::actualizarCostoConexion() {
    std::string origen, destino;
    std::string entradaCosto;
    int nuevoCosto;

    std::cout << "\n=== ACTUALIZAR COSTO DE CONEXION ===" << std::endl;
    std::cout << "Origen: "; std::cin >> origen;
    std::cout << "Destino: "; std::cin >> destino;
    std::cout << "Nuevo costo: ";
    std::cin >> entradaCosto;

    try {
        nuevoCosto = std::stoi(entradaCosto);
    } catch (const std::out_of_range& e) {
        std::cerr << "Error: El costo es demasiado grande (maximo: " << std::numeric_limits<int>::max() << ")" << std::endl;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return;
    } catch (const std::invalid_argument& e) {
        std::cerr << "Error: El costo debe ser un numero entero valido" << std::endl;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return;
    }

    red->actualizarCostoConexion(origen, destino, nuevoCosto);
}
