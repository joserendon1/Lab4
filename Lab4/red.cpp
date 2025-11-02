#include "red.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <random>
#include <queue>
#include <limits>
#include <algorithm>

Red::Red(const std::string& nombre) : nombre(nombre) {}

Red::~Red() {
    limpiarRed();
}

void Red::limpiarRed() {
    for (auto& par : enrutadores) {
        delete par.second;
    }
    enrutadores.clear();

    for (auto conexion : conexiones) {
        delete conexion;
    }
    conexiones.clear();
}

void Red::agregarEnrutador(const std::string& nombre) {
    if (enrutadores.find(nombre) == enrutadores.end()) {
        enrutadores[nombre] = new Enrutador(nombre);
        ejecutarAlgoritmoRutas();
    }
}

void Red::eliminarEnrutador(const std::string& nombre) {
    auto it = enrutadores.find(nombre);
    if (it != enrutadores.end()) {
        delete it->second;
        enrutadores.erase(it);

        auto itConexion = conexiones.begin();
        while (itConexion != conexiones.end()) {
            Conexion* conexion = *itConexion;
            if (conexion->obtenerOrigen() == nombre || conexion->obtenerDestino() == nombre) {
                delete conexion;
                itConexion = conexiones.erase(itConexion);
            } else {
                ++itConexion;
            }
        }

        ejecutarAlgoritmoRutas();
    }
}

bool Red::existeEnrutador(const std::string& nombre) const {
    return enrutadores.find(nombre) != enrutadores.end();
}

void Red::agregarConexion(const std::string& origen, const std::string& destino, int costo) {
    if (existeEnrutador(origen) && existeEnrutador(destino)) {
        conexiones.push_back(new Conexion(origen, destino, costo));
        conexiones.push_back(new Conexion(destino, origen, costo));
        enrutadores[origen]->agregarVecino(destino, costo);
        enrutadores[destino]->agregarVecino(origen, costo);

        ejecutarAlgoritmoRutas();
    }
}

void Red::eliminarConexion(const std::string& origen, const std::string& destino) {
    auto it = conexiones.begin();
    while (it != conexiones.end()) {
        Conexion* conexion = *it;
        if ((conexion->obtenerOrigen() == origen && conexion->obtenerDestino() == destino) ||
            (conexion->obtenerOrigen() == destino && conexion->obtenerDestino() == origen)) {
            delete conexion;
            it = conexiones.erase(it);
        } else {
            ++it;
        }
    }

    if (existeEnrutador(origen)) {
        enrutadores[origen]->eliminarVecino(destino);
    }
    if (existeEnrutador(destino)) {
        enrutadores[destino]->eliminarVecino(origen);
    }

    ejecutarAlgoritmoRutas();
}

void Red::dijkstra(const std::string& origen) {
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

        auto vecinosActual = enrutadores[actual]->obtenerVecinos();
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

void Red::ejecutarAlgoritmoRutas() {
    for (const auto& par : enrutadores) {
        dijkstra(par.first);
    }
}

void Red::actualizarTablasTodosEnrutadores() {
    ejecutarAlgoritmoRutas();
}

void Red::cargarDesdeArchivo(const std::string& nombreArchivo) {
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

        if (ss >> origen >> destino >> costo) {
            if (!existeEnrutador(origen)) {
                agregarEnrutador(origen);
            }
            if (!existeEnrutador(destino)) {
                agregarEnrutador(destino);
            }
            agregarConexion(origen, destino, costo);
        }
    }

    archivo.close();
    std::cout << "Red cargada desde archivo: " << nombreArchivo << std::endl;
}

void Red::generarRedAleatoria(int numEnrutadores, double probabilidadConexion, int costoMaximo) {
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
        for (size_t j = i + 1; j < nombres.size(); ++j) {
            if (disProb(gen) < probabilidadConexion) {
                int costo = disCosto(gen);
                agregarConexion(nombres[i], nombres[j], costo);
            }
        }
    }

    std::cout << "Red aleatoria generada con " << numEnrutadores << " enrutadores" << std::endl;
}

void Red::imprimirTablaEnrutador(const std::string& nombre) const {
    if (!existeEnrutador(nombre)) {
        std::cout << "Error: Enrutador '" << nombre << "' no existe" << std::endl;
        return;
    }

    Enrutador* enrutador = enrutadores.at(nombre);
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

void Red::encontrarYMostrarCamino(const std::string& origen, const std::string& destino) const {
    if (!existeEnrutador(origen) || !existeEnrutador(destino)) {
        std::cout << "Error: Uno o ambos enrutadores no existen" << std::endl;
        return;
    }

    Enrutador* enrutadorOrigen = enrutadores.at(origen);
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
        std::cout << "No existe camino entre " << origen << " y " << destino << std::endl;
    }
}

std::vector<std::string> Red::obtenerNombresEnrutadores() const {
    std::vector<std::string> nombres;
    for (const auto& par : enrutadores) {
        nombres.push_back(par.first);
    }
    return nombres;
}

std::vector<Conexion*> Red::obtenerConexiones() const {
    return conexiones;
}

void Red::actualizarCostoConexion(const std::string& origen, const std::string& destino, int nuevoCosto) {
    eliminarConexion(origen, destino);
    agregarConexion(origen, destino, nuevoCosto);
}

void Red::mostrarTopologia() const {
    std::cout << "\n=== TOPOLOGIA DE LA RED ===" << std::endl;
    std::cout << "Conexiones:" << std::endl;
    for(const auto& conexion : conexiones) {
        std::cout << conexion->obtenerOrigen() << " --(" << conexion->obtenerCosto()
        << ")--> " << conexion->obtenerDestino() << std::endl;
    }
}
