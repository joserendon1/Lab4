#include "red_dirigida.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <random>
#include <queue>
#include <limits>
#include <algorithm>

RedDirigida::RedDirigida(const std::string& nombre) : nombre(nombre) {}

RedDirigida::~RedDirigida() {
    limpiarRed();
}

void RedDirigida::limpiarRed() {
    for (auto& par : enrutadores) {
        delete par.second;
    }
    enrutadores.clear();

    for (auto conexion : conexiones) {
        delete conexion;
    }
    conexiones.clear();
}

void RedDirigida::agregarEnrutador(const std::string& nombre) {
    if (enrutadores.find(nombre) == enrutadores.end()) {
        enrutadores[nombre] = new EnrutadorDirigido(nombre);
        ejecutarAlgoritmoRutas();
        std::cout << "Enrutador '" << nombre << "' agregado." << std::endl;
    }
}

void RedDirigida::eliminarEnrutador(const std::string& nombre) {
    auto it = enrutadores.find(nombre);
    if (it != enrutadores.end()) {
        delete it->second;
        enrutadores.erase(it);

        auto itConexion = conexiones.begin();
        while (itConexion != conexiones.end()) {
            ConexionDirigida* conexion = *itConexion;
            if (conexion->obtenerOrigen() == nombre || conexion->obtenerDestino() == nombre) {
                delete conexion;
                itConexion = conexiones.erase(itConexion);
            } else {
                ++itConexion;
            }
        }

        ejecutarAlgoritmoRutas();
        std::cout << "Enrutador '" << nombre << "' eliminado." << std::endl;
    }
}

bool RedDirigida::existeEnrutador(const std::string& nombre) const {
    return enrutadores.find(nombre) != enrutadores.end();
}

void RedDirigida::agregarConexionDirigida(const std::string& origen, const std::string& destino, int costo) {
    if (existeEnrutador(origen) && existeEnrutador(destino)) {
        conexiones.push_back(new ConexionDirigida(origen, destino, costo));
        enrutadores[origen]->agregarVecinoSaliente(destino, costo);

        ejecutarAlgoritmoRutas();
        std::cout << "Conexion dirigida agregada: " << origen << " -> " << destino << " (costo: " << costo << ")" << std::endl;
    }
}

void RedDirigida::agregarConexionBidireccional(const std::string& nodo1, const std::string& nodo2, int costo1, int costo2) {
    agregarConexionDirigida(nodo1, nodo2, costo1);
    agregarConexionDirigida(nodo2, nodo1, costo2);
}

void RedDirigida::eliminarConexion(const std::string& origen, const std::string& destino) {
    auto it = conexiones.begin();
    while (it != conexiones.end()) {
        ConexionDirigida* conexion = *it;
        if (conexion->obtenerOrigen() == origen && conexion->obtenerDestino() == destino) {
            delete conexion;
            it = conexiones.erase(it);
        } else {
            ++it;
        }
    }

    if (existeEnrutador(origen)) {
        enrutadores[origen]->eliminarVecinoSaliente(destino);
    }

    ejecutarAlgoritmoRutas();
}

void RedDirigida::dijkstra(const std::string& origen) {
    if (!existeEnrutador(origen)) return;

    enrutadores[origen]->limpiarTablas();

    std::map<std::string, int> distancias;
    std::map<std::string, std::vector<std::string>> caminos;
    std::map<std::string, bool> visitado;

    for (const auto& par : enrutadores) {
        distancias[par.first] = std::numeric_limits<int>::max();
        visitado[par.first] = false;
    }

    distancias[origen] = 0;
    caminos[origen] = {origen};

    using ParDistanciaNodo = std::pair<int, std::string>;
    std::priority_queue<ParDistanciaNodo, std::vector<ParDistanciaNodo>, std::greater<ParDistanciaNodo>> cola;
    cola.push({0, origen});

    while (!cola.empty()) {
        std::string actual = cola.top().second;
        cola.pop();

        if (visitado[actual]) continue;
        visitado[actual] = true;

        auto vecinosActual = enrutadores[actual]->obtenerVecinosSalientes();
        for (const auto& vecino : vecinosActual) {
            std::string nombreVecino = vecino.first;
            int costoEnlace = vecino.second;

            if (!visitado[nombreVecino]) {
                int nuevaDistancia = distancias[actual] + costoEnlace;

                if (nuevaDistancia < distancias[nombreVecino]) {
                    distancias[nombreVecino] = nuevaDistancia;
                    caminos[nombreVecino] = caminos[actual];
                    caminos[nombreVecino].push_back(nombreVecino);
                    cola.push({nuevaDistancia, nombreVecino});
                }
            }
        }
    }

    for (const auto& par : distancias) {
        if (par.second != std::numeric_limits<int>::max()) {
            enrutadores[origen]->actualizarTablaCostos(par.first, par.second);
            enrutadores[origen]->actualizarTablaCaminos(par.first, caminos[par.first]);
        }
    }
}

void RedDirigida::ejecutarAlgoritmoRutas() {
    for (const auto& par : enrutadores) {
        dijkstra(par.first);
    }
}

void RedDirigida::imprimirTablaEnrutador(const std::string& nombre) const {
    if (!existeEnrutador(nombre)) {
        std::cout << "Error: Enrutador '" << nombre << "' no existe" << std::endl;
        return;
    }

    EnrutadorDirigido* enrutador = enrutadores.at(nombre);
    auto tablaCostos = enrutador->obtenerTablaCostos();
    auto tablaCaminos = enrutador->obtenerTablaCaminos();

    std::cout << "\nTabla de enrutamiento para " << nombre << ":\n";
    std::cout << "Destino\tCosto\tCamino\n";
    std::cout << "-------\t-----\t------\n";

    for (const auto& par : tablaCostos) {
        std::cout << par.first << "\t" << par.second << "\t";

        auto itCamino = tablaCaminos.find(par.first);
        if (itCamino != tablaCaminos.end()) {
            for (size_t i = 0; i < itCamino->second.size(); ++i) {
                std::cout << itCamino->second[i];
                if (i < itCamino->second.size() - 1) {
                    std::cout << " -> ";
                }
            }
        }
        std::cout << std::endl;
    }
}

void RedDirigida::encontrarYMostrarCamino(const std::string& origen, const std::string& destino) const {
    if (!existeEnrutador(origen) || !existeEnrutador(destino)) {
        std::cout << "Error: Uno o ambos enrutadores no existen" << std::endl;
        return;
    }

    EnrutadorDirigido* enrutadorOrigen = enrutadores.at(origen);
    int costo = enrutadorOrigen->obtenerCostoHacia(destino);
    std::vector<std::string> camino = enrutadorOrigen->obtenerCaminoHacia(destino);

    std::cout << "\nCamino de " << origen << " a " << destino << ":\n";
    if (costo != -1 && !camino.empty()) {
        std::cout << "Costo: " << costo << std::endl;
        std::cout << "Ruta: ";
        for (size_t i = 0; i < camino.size(); ++i) {
            std::cout << camino[i];
            if (i < camino.size() - 1) {
                std::cout << " -> ";
            }
        }
        std::cout << std::endl;
    } else {
        std::cout << "No existe camino dirigido de " << origen << " a " << destino << std::endl;
    }
}

std::vector<std::string> RedDirigida::obtenerNombresEnrutadores() const {
    std::vector<std::string> nombres;
    for (const auto& par : enrutadores) {
        nombres.push_back(par.first);
    }
    return nombres;
}

std::vector<ConexionDirigida*> RedDirigida::obtenerConexiones() const {
    return conexiones;
}

void RedDirigida::mostrarTopologia() const {
    std::cout << "\n=== TOPOLOGIA DE LA RED DIRIGIDA ===" << std::endl;
    std::cout << "Conexiones (dirigidas):" << std::endl;

    auto nombres = obtenerNombresEnrutadores();
    std::cout << "Enrutadores: ";
    for (size_t i = 0; i < nombres.size(); ++i) {
        std::cout << nombres[i];
        if (i < nombres.size() - 1) {
            std::cout << ", ";
        }
    }
    std::cout << std::endl;

    std::cout << "\nConexiones:" << std::endl;
    for(const auto& conexion : conexiones) {
        std::cout << conexion->obtenerOrigen() << " --(" << conexion->obtenerCosto()
        << ")--> " << conexion->obtenerDestino() << std::endl;
    }
    std::cout << "Total de conexiones: " << conexiones.size() << std::endl;
}

void RedDirigida::actualizarCostoConexion(const std::string& origen, const std::string& destino, int nuevoCosto) {
    // Validaciones
    if (origen == destino) {
        std::cout << "Error: No se puede actualizar conexion de un enrutador a si mismo" << std::endl;
        return;
    }

    if (nuevoCosto <= 0) {
        std::cout << "Error: El costo debe ser un numero positivo" << std::endl;
        return;
    }

    if (!existeEnrutador(origen) || !existeEnrutador(destino)) {
        std::cout << "Error: Uno o ambos enrutadores no existen" << std::endl;
        return;
    }

    bool conexionEncontrada = false;
    for (auto conexion : conexiones) {
        if (conexion->obtenerOrigen() == origen && conexion->obtenerDestino() == destino) {
            conexion->establecerCosto(nuevoCosto);
            conexionEncontrada = true;
            break;
        }
    }

    if (conexionEncontrada) {
        enrutadores[origen]->eliminarVecinoSaliente(destino);
        enrutadores[origen]->agregarVecinoSaliente(destino, nuevoCosto);

        ejecutarAlgoritmoRutas();
        std::cout << "Costo actualizado: " << origen << " -> " << destino << " = " << nuevoCosto << std::endl;
    } else {
        std::cout << "Error: No existe conexion de " << origen << " a " << destino << std::endl;
    }
}

void RedDirigida::cargarDesdeArchivo(const std::string& nombreArchivo) {
    std::ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo " << nombreArchivo << std::endl;
        return;
    }

    limpiarRed();

    std::string linea;
    while (std::getline(archivo, linea)) {
        std::istringstream ss(linea);
        std::string origen, destino;
        int costo;

        if (linea.empty() || linea[0] == '#') {
            continue;
        }

        if (ss >> origen >> destino >> costo) {
            if (!existeEnrutador(origen)) {
                agregarEnrutador(origen);
            }
            if (!existeEnrutador(destino)) {
                agregarEnrutador(destino);
            }
            agregarConexionDirigida(origen, destino, costo);
        }
    }

    archivo.close();
    std::cout << "Red dirigida cargada desde archivo: " << nombreArchivo << std::endl;
    std::cout << "Enrutadores: " << enrutadores.size() << ", Conexiones: " << conexiones.size() << std::endl;
}

void RedDirigida::generarRedAleatoria(int numEnrutadores, double probabilidadConexion, int costoMaximo) {
    limpiarRed();

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> disProb(0.0, 1.0);
    std::uniform_int_distribution<> disCosto(1, costoMaximo);

    for (int i = 0; i < numEnrutadores; ++i) {
        std::string nombre = "E" + std::to_string(i);
        agregarEnrutador(nombre);
    }

    std::vector<std::string> nombres = obtenerNombresEnrutadores();
    for (size_t i = 0; i < nombres.size(); ++i) {
        for (size_t j = 0; j < nombres.size(); ++j) {
            if (i != j) {
                if (disProb(gen) < probabilidadConexion) {
                    int costo = disCosto(gen);
                    agregarConexionDirigida(nombres[i], nombres[j], costo);
                }
            }
        }
    }

    std::cout << "Red dirigida aleatoria generada con " << numEnrutadores
              << " enrutadores y " << conexiones.size() << " conexiones" << std::endl;
}
