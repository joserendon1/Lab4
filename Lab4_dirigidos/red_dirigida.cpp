#include "red_dirigida.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <random>
#include <queue>
#include <limits>
#include <algorithm>
#include <stdexcept>

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
    if (nombre.empty()) {
        std::cerr << "Error: El nombre del enrutador no puede estar vacío" << std::endl;
        return;
    }

    if (nombre.length() > 20) {
        std::cerr << "Error: El nombre del enrutador es demasiado largo (máx 20 caracteres)" << std::endl;
        return;
    }

    for (char c : nombre) {
        if (!std::isalnum(c) && c != '_') {
            std::cerr << "Error: El nombre solo puede contener letras, números y _" << std::endl;
            return;
        }
    }

    if (enrutadores.find(nombre) != enrutadores.end()) {
        std::cerr << "Error: El enrutador '" << nombre << "' ya existe" << std::endl;
        return;
    }

    try {
        enrutadores[nombre] = new EnrutadorDirigido(nombre);
        ejecutarAlgoritmoRutas();
        std::cout << "Enrutador '" << nombre << "' agregado exitosamente." << std::endl;
    } catch (const std::bad_alloc& e) {
        std::cerr << "Error crítico: Memoria insuficiente para crear enrutador" << std::endl;
    }
}

void RedDirigida::eliminarEnrutador(const std::string& nombre) {
    if (!existeEnrutador(nombre)) {
        std::cerr << "Error: No se puede eliminar - el enrutador '" << nombre << "' no existe" << std::endl;
        return;
    }

    if (enrutadores.size() <= 1) {
        std::cerr << "Error: No se puede eliminar el único enrutador de la red" << std::endl;
        return;
    }

    auto it = enrutadores.find(nombre);
    if (it != enrutadores.end()) {
        for (auto& par : enrutadores) {
            if (par.first != nombre) {
                par.second->eliminarVecinoSaliente(nombre);
            }
        }

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
        std::cout << "Enrutador '" << nombre << "' eliminado exitosamente." << std::endl;
    }
}

bool RedDirigida::existeEnrutador(const std::string& nombre) const {
    return enrutadores.find(nombre) != enrutadores.end();
}

void RedDirigida::agregarConexionDirigida(const std::string& origen, const std::string& destino, int costo) {
    if (origen == destino) {
        std::cerr << "Error: No se puede crear conexión de un enrutador consigo mismo" << std::endl;
        return;
    }

    if (costo <= 0) {
        std::cerr << "Error: El costo debe ser un número positivo mayor que cero" << std::endl;
        return;
    }

    if (costo > 1000000) {
        std::cerr << "Error: El costo es demasiado alto (máximo 1,000,000)" << std::endl;
        return;
    }

    if (!existeEnrutador(origen)) {
        std::cerr << "Error: El enrutador origen '" << origen << "' no existe" << std::endl;
        return;
    }

    if (!existeEnrutador(destino)) {
        std::cerr << "Error: El enrutador destino '" << destino << "' no existe" << std::endl;
        return;
    }

    for (const auto& conexion : conexiones) {
        if (conexion->obtenerOrigen() == origen && conexion->obtenerDestino() == destino) {
            std::cerr << "Error: Ya existe una conexión de " << origen << " a " << destino << std::endl;
            return;
        }
    }

    try {
        conexiones.push_back(new ConexionDirigida(origen, destino, costo));
        enrutadores[origen]->agregarVecinoSaliente(destino, costo);
        ejecutarAlgoritmoRutas();
        std::cout << "Conexión dirigida agregada: " << origen << " -> " << destino << " (costo: " << costo << ")" << std::endl;
    } catch (const std::bad_alloc& e) {
        std::cerr << "Error crítico: Memoria insuficiente para crear conexión" << std::endl;
    }
}

void RedDirigida::agregarConexionBidireccional(const std::string& nodo1, const std::string& nodo2, int costo1, int costo2) {
    if (nodo1 == nodo2) {
        std::cerr << "Error: No se puede crear conexión bidireccional con el mismo nodo" << std::endl;
        return;
    }

    std::cout << "Agregando conexión bidireccional entre " << nodo1 << " y " << nodo2 << std::endl;
    agregarConexionDirigida(nodo1, nodo2, costo1);
    agregarConexionDirigida(nodo2, nodo1, costo2);
}

void RedDirigida::eliminarConexion(const std::string& origen, const std::string& destino) {
    if (origen == destino) {
        std::cerr << "Error: Parámetros inválidos para eliminar conexión" << std::endl;
        return;
    }

    if (!existeEnrutador(origen)) {
        std::cerr << "Error: El enrutador origen '" << origen << "' no existe" << std::endl;
        return;
    }

    if (!existeEnrutador(destino)) {
        std::cerr << "Error: El enrutador destino '" << destino << "' no existe" << std::endl;
        return;
    }

    bool conexionEncontrada = false;
    auto it = conexiones.begin();
    while (it != conexiones.end()) {
        ConexionDirigida* conexion = *it;
        if (conexion->obtenerOrigen() == origen && conexion->obtenerDestino() == destino) {
            delete conexion;
            it = conexiones.erase(it);
            conexionEncontrada = true;
            break;
        } else {
            ++it;
        }
    }

    if (conexionEncontrada) {
        enrutadores[origen]->eliminarVecinoSaliente(destino);
        ejecutarAlgoritmoRutas();
        std::cout << "Conexión eliminada: " << origen << " -> " << destino << std::endl;
    } else {
        std::cerr << "Error: No existe la conexión de " << origen << " a " << destino << std::endl;
    }
}

void RedDirigida::dijkstra(const std::string& origen) {
    if (!existeEnrutador(origen)) {
        std::cerr << "Error en Dijkstra: El enrutador origen '" << origen << "' no existe" << std::endl;
        return;
    }

    try {
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

            if (!existeEnrutador(actual)) {
                std::cerr << "Advertencia: Enrutador '" << actual << "' eliminado durante cálculo de rutas" << std::endl;
                continue;
            }

            auto vecinosActual = enrutadores[actual]->obtenerVecinosSalientes();
            for (const auto& vecino : vecinosActual) {
                std::string nombreVecino = vecino.first;
                int costoEnlace = vecino.second;

                if (!existeEnrutador(nombreVecino)) {
                    std::cerr << "Advertencia: Vecino '" << nombreVecino << "' eliminado durante cálculo de rutas" << std::endl;
                    continue;
                }

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
            if (par.second != std::numeric_limits<int>::max() && existeEnrutador(par.first)) {
                enrutadores[origen]->actualizarTablaCostos(par.first, par.second);
                enrutadores[origen]->actualizarTablaCaminos(par.first, caminos[par.first]);
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error crítico en Dijkstra: " << e.what() << std::endl;
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
    if (nombreArchivo.empty()) {
        std::cerr << "Error: El nombre del archivo no puede estar vacío" << std::endl;
        return;
    }

    std::ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo '" << nombreArchivo << "'" << std::endl;
        std::cerr << "Verifique que el archivo existe y tiene permisos de lectura" << std::endl;
        return;
    }

    limpiarRed();

    std::string linea;
    int numeroLinea = 0;
    int conexionesCargadas = 0;
    int errores = 0;

    while (std::getline(archivo, linea)) {
        numeroLinea++;

        if (linea.empty()) {
            continue;
        }

        std::istringstream ss(linea);
        std::string origen, destino;
        int costo;

        if (ss >> origen >> destino >> costo) {
            if (origen.empty() || destino.empty()) {
                std::cerr << "Error línea " << numeroLinea << ": Nombres vacíos" << std::endl;
                errores++;
                continue;
            }

            if (costo <= 0) {
                std::cerr << "Error línea " << numeroLinea << ": Costo debe ser positivo" << std::endl;
                errores++;
                continue;
            }

            if (!existeEnrutador(origen)) {
                agregarEnrutador(origen);
            }
            if (!existeEnrutador(destino)) {
                agregarEnrutador(destino);
            }
            agregarConexionDirigida(origen, destino, costo);
            conexionesCargadas++;
        } else {
            std::cerr << "Error línea " << numeroLinea << ": Formato inválido. Se espera: origen destino costo" << std::endl;
            errores++;
        }
    }

    archivo.close();

    if (enrutadores.empty()) {
        std::cerr << "Advertencia: El archivo no contenía datos válidos o estaba vacío" << std::endl;
        return;
    }

    std::cout << "Red cargada exitosamente desde: " << nombreArchivo << std::endl;
    std::cout << "Resumen: " << enrutadores.size() << " enrutadores, "
              << conexiones.size() << " conexiones cargadas" << std::endl;

    if (errores > 0) {
        std::cout << "Advertencia: Se encontraron " << errores << " errores en el archivo" << std::endl;
    }

    std::cout << "\nTopología cargada:" << std::endl;
    for (const auto& conexion : conexiones) {
        std::cout << "  " << conexion->obtenerOrigen() << " -> "
                  << conexion->obtenerDestino() << " (costo: "
                  << conexion->obtenerCosto() << ")" << std::endl;
    }
}

void RedDirigida::generarRedAleatoria(int numEnrutadores, double probabilidadConexion, int costoMaximo) {
    if (numEnrutadores < 2 || numEnrutadores > 100) {
        std::cerr << "Error: Número de enrutadores debe estar entre 2 y 100" << std::endl;
        return;
    }

    if (probabilidadConexion < 0.1 || probabilidadConexion > 1.0) {
        std::cerr << "Error: Probabilidad debe estar entre 0.1 y 1.0" << std::endl;
        return;
    }

    if (costoMaximo < 1 || costoMaximo > 1000) {
        std::cerr << "Error: Costo máximo debe estar entre 1 y 1000" << std::endl;
        return;
    }

    try {
        limpiarRed();

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> disProb(0.0, 1.0);
        std::uniform_int_distribution<> disCosto(1, costoMaximo);

        std::cout << "Creando " << numEnrutadores << " enrutadores..." << std::endl;
        for (int i = 0; i < numEnrutadores; ++i) {
            std::string nombre = "E" + std::to_string(i);
            agregarEnrutador(nombre);
        }

        std::vector<std::string> nombres = obtenerNombresEnrutadores();
        int conexionesCreadas = 0;
        int conexionesPosibles = numEnrutadores * (numEnrutadores - 1);

        std::cout << "Generando conexiones (probabilidad: " << (probabilidadConexion * 100) << "%)..." << std::endl;

        for (size_t i = 0; i < nombres.size(); ++i) {
            for (size_t j = 0; j < nombres.size(); ++j) {
                if (i != j && disProb(gen) < probabilidadConexion) {
                    int costo = disCosto(gen);
                    agregarConexionDirigida(nombres[i], nombres[j], costo);
                    conexionesCreadas++;
                }
            }
        }

        double densidad = (conexionesCreadas * 100.0) / conexionesPosibles;

        std::cout << "\n Red aleatoria generada exitosamente:" << std::endl;
        std::cout << "   Enrutadores creados: " << numEnrutadores << std::endl;
        std::cout << "   Conexiones creadas: " << conexionesCreadas << std::endl;
        std::cout << "   Densidad de red: " << densidad << "%" << std::endl;
        std::cout << "   Costo máximo configurado: " << costoMaximo << std::endl;

        if (densidad < 10.0) {
            std::cout << "   Advertencia: La red tiene baja conectividad" << std::endl;
        }

    } catch (const std::exception& e) {
        std::cerr << "Error crítico generando red aleatoria: " << e.what() << std::endl;
        limpiarRed();
    }
}

void RedDirigida::mostrarFormatoArchivo() const {
    std::cout << "\n=== FORMATO ESPERADO DEL ARCHIVO ===" << std::endl;
    std::cout << "Cada línea debe contener: ORIGEN DESTINO COSTO" << std::endl;
    std::cout << "Ejemplo:" << std::endl;
    std::cout << "A B 5" << std::endl;
    std::cout << "A C 3" << std::endl;
    std::cout << "B C 2" << std::endl;
    std::cout << "B D 4" << std::endl;
    std::cout << "C D 1" << std::endl;
    std::cout << "===============================" << std::endl;
}
