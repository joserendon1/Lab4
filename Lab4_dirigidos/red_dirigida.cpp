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
        std::cerr << "Error: El nombre del enrutador no puede estar vacio" << std::endl;
        return;
    }

    if (nombre.length() > 20) {
        std::cerr << "Error: El nombre del enrutador es demasiado largo (max 20 caracteres)" << std::endl;
        return;
    }

    for (char c : nombre) {
        if (!std::isalnum(c) && c != '_') {
            std::cerr << "Error: El nombre solo puede contener letras, numeros y _" << std::endl;
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
        std::cerr << "Error critico: Memoria insuficiente para crear enrutador" << std::endl;
    }
}

void RedDirigida::eliminarEnrutador(const std::string& nombre) {
    if (!existeEnrutador(nombre)) {
        std::cerr << "Error: No se puede eliminar - el enrutador '" << nombre << "' no existe" << std::endl;
        return;
    }

    if (enrutadores.size() <= 1) {
        std::cerr << "Error: No se puede eliminar el unico enrutador de la red" << std::endl;
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
        std::cerr << "Error: No se puede crear conexion de un enrutador consigo mismo" << std::endl;
        return;
    }

    if (costo <= 0) {
        std::cerr << "Error: El costo debe ser un numero positivo mayor que cero" << std::endl;
        return;
    }

    if (costo > 1000) {
        std::cerr << "Error: El costo es demasiado alto (maximo 1,000)" << std::endl;
        return;
    }

    if (enrutadores.size() > 0 && costo > std::numeric_limits<int>::max() / enrutadores.size()) {
        std::cerr << "Error: Costo demasiado alto para el tamaño de la red" << std::endl;
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
            std::cerr << "Error: Ya existe una conexion de " << origen << " a " << destino << std::endl;
            return;
        }
    }

    try {
        conexiones.push_back(new ConexionDirigida(origen, destino, costo));
        enrutadores[origen]->agregarVecinoSaliente(destino, costo);
        ejecutarAlgoritmoRutas();
        std::cout << "Conexion dirigida agregada: " << origen << " -> " << destino << " (costo: " << costo << ")" << std::endl;
    } catch (const std::bad_alloc& e) {
        std::cerr << "Error critico: Memoria insuficiente para crear conexion" << std::endl;
    }
}

void RedDirigida::eliminarConexion(const std::string& origen, const std::string& destino) {
    if (origen == destino) {
        std::cerr << "Error: Parametros invalidos para eliminar conexion" << std::endl;
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
        std::cout << "Conexion eliminada: " << origen << " -> " << destino << std::endl;
    } else {
        std::cerr << "Error: No existe la conexion de " << origen << " a " << destino << std::endl;
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

        int valorInfinitoSeguro = std::numeric_limits<int>::max() / 2;
        for (const auto& par : enrutadores) {
            distancias[par.first] = valorInfinitoSeguro;
            visitado[par.first] = false;
        }

        distancias[origen] = 0;
        caminos[origen] = {origen};

        using ParDistanciaNodo = std::pair<int, std::string>;
        std::priority_queue<ParDistanciaNodo, std::vector<ParDistanciaNodo>, std::greater<ParDistanciaNodo>> cola;
        cola.push({0, origen});

        const size_t MAX_ITERACIONES = enrutadores.size() * enrutadores.size() + 100;
        size_t iteraciones = 0;

        while (!cola.empty() && iteraciones < MAX_ITERACIONES) {
            iteraciones++;

            std::string actual = cola.top().second;
            cola.pop();

            if (visitado[actual]) continue;
            visitado[actual] = true;

            if (!existeEnrutador(actual)) {
                std::cerr << "Advertencia: Enrutador '" << actual << "' eliminado durante calculo de rutas" << std::endl;
                continue;
            }

            auto vecinosActual = enrutadores[actual]->obtenerVecinosSalientes();
            for (const auto& vecino : vecinosActual) {
                std::string nombreVecino = vecino.first;
                int costoEnlace = vecino.second;

                if (!existeEnrutador(nombreVecino)) {
                    std::cerr << "Advertencia: Vecino '" << nombreVecino << "' eliminado durante calculo de rutas" << std::endl;
                    continue;
                }

                if (!visitado[nombreVecino]) {
                    if (distancias[actual] > valorInfinitoSeguro - costoEnlace) {
                        std::cerr << "Advertencia: Overflow potencial detectado en ruta de "
                                  << origen << " a " << nombreVecino << std::endl;
                        continue;
                    }

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

        if (iteraciones >= MAX_ITERACIONES) {
            std::cerr << "Error: Dijkstra excedio el limite maximo de iteraciones ("
                      << MAX_ITERACIONES << ") para " << origen
                      << ". Verifique la topologia en busca de ciclos." << std::endl;
            return;
        }

        for (const auto& par : distancias) {
            if (par.second < valorInfinitoSeguro && existeEnrutador(par.first)) {
                enrutadores[origen]->actualizarTablaCostos(par.first, par.second);
                enrutadores[origen]->actualizarTablaCaminos(par.first, caminos[par.first]);
            }
        }

    } catch (const std::exception& e) {
        std::cerr << "Error critico en Dijkstra: " << e.what() << std::endl;
    }
}

void RedDirigida::ejecutarAlgoritmoRutas() {
    if (enrutadores.size() > 100) {
        std::cout << "Advertencia: Red muy grande (" << enrutadores.size()
        << " enrutadores). El calculo de rutas puede demorar." << std::endl;
    }

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
        std::cerr << "Error: El nombre del archivo no puede estar vacio" << std::endl;
        return;
    }

    std::ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo '" << nombreArchivo << "'" << std::endl;
        std::cerr << "Verifique que el archivo existe" << std::endl;
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
                std::cerr << "Error linea " << numeroLinea << ": Nombres vacios" << std::endl;
                errores++;
                continue;
            }

            if (costo <= 0) {
                std::cerr << "Error linea " << numeroLinea << ": Costo debe ser positivo" << std::endl;
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
            std::cerr << "Error linea " << numeroLinea << ": Formato invalido. Se espera: origen destino costo" << std::endl;
            errores++;
        }
    }

    archivo.close();

    if (enrutadores.empty()) {
        std::cerr << "Advertencia: El archivo no contenia datos validos o estaba vacio" << std::endl;
        return;
    }

    std::cout << "Red cargada exitosamente desde: " << nombreArchivo << std::endl;
    std::cout << "Resumen: " << enrutadores.size() << " enrutadores, "
              << conexiones.size() << " conexiones cargadas" << std::endl;

    if (errores > 0) {
        std::cout << "Advertencia: Se encontraron " << errores << " errores en el archivo" << std::endl;
    }

    std::cout << "\nTopologia cargada:" << std::endl;
    for (const auto& conexion : conexiones) {
        std::cout << "  " << conexion->obtenerOrigen() << " -> "
                  << conexion->obtenerDestino() << " (costo: "
                  << conexion->obtenerCosto() << ")" << std::endl;
    }
}

void RedDirigida::generarRedAleatoria(int numEnrutadores, double probabilidadConexion, int costoMaximo) {
    if (numEnrutadores < 2 || numEnrutadores > 100) {
        std::cerr << "Error: Numero de enrutadores debe estar entre 2 y 100" << std::endl;
        return;
    }

    if (probabilidadConexion < 0.1 || probabilidadConexion > 1.0) {
        std::cerr << "Error: Probabilidad debe estar entre 0.1 y 1.0" << std::endl;
        return;
    }

    if (costoMaximo < 1 || costoMaximo > 1000) {
        std::cerr << "Error: Costo maximo debe estar entre 1 y 1000" << std::endl;
        return;
    }

    if (numEnrutadores > 50 && costoMaximo > 100) {
        std::cout << "Advertencia: Reduciendo costo maximo a 100 para red grande de "
                  << numEnrutadores << " enrutadores" << std::endl;
        costoMaximo = 100;
    }

    if (numEnrutadores > 80 && costoMaximo > 50) {
        std::cout << "Advertencia: Reduciendo costo maximo a 50 para red muy grande de "
                  << numEnrutadores << " enrutadores" << std::endl;
        costoMaximo = 50;
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

        const int MAX_CONEXIONES = numEnrutadores * 10;
        bool advertenciaConexiones = false;

        for (size_t i = 0; i < nombres.size(); ++i) {
            for (size_t j = 0; j < nombres.size(); ++j) {
                if (i != j && disProb(gen) < probabilidadConexion) {
                    if (conexionesCreadas >= MAX_CONEXIONES) {
                        if (!advertenciaConexiones) {
                            std::cout << "Advertencia: Limite de conexiones alcanzado ("
                                      << MAX_CONEXIONES << "). Deteniendo generacion." << std::endl;
                            advertenciaConexiones = true;
                        }
                        break;
                    }

                    int costo = disCosto(gen);
                    agregarConexionDirigida(nombres[i], nombres[j], costo);
                    conexionesCreadas++;
                }
            }
            if (advertenciaConexiones) break;
        }

        double densidad = (conexionesCreadas * 100.0) / conexionesPosibles;

        std::cout << "\nRed aleatoria generada exitosamente:" << std::endl;
        std::cout << "   Enrutadores creados: " << numEnrutadores << std::endl;
        std::cout << "   Conexiones creadas: " << conexionesCreadas << std::endl;
        std::cout << "   Densidad de red: " << densidad << "%" << std::endl;
        std::cout << "   Costo maximo utilizado: " << costoMaximo << std::endl;

        if (densidad < 10.0) {
            std::cout << "   Advertencia: La red tiene baja conectividad" << std::endl;
        }

        if (costoMaximo > 500) {
            std::cout << "   Advertencia: Costos altos detectados, verifique el rendimiento" << std::endl;
        }

        if (numEnrutadores > 30) {
            std::cout << "   Advertencia: Red grande, los calculos pueden tomar mas tiempo" << std::endl;
        }

    } catch (const std::exception& e) {
        std::cerr << "Error critico generando red aleatoria: " << e.what() << std::endl;
        limpiarRed();
    }
}

void RedDirigida::mostrarFormatoArchivo() const {
    std::cout << "\n=== FORMATO ESPERADO DEL ARCHIVO ===" << std::endl;
    std::cout << "Cada linea debe contener: ORIGEN DESTINO COSTO" << std::endl;
    std::cout << "Ejemplo:" << std::endl;
    std::cout << "A B 5" << std::endl;
    std::cout << "A C 3" << std::endl;
    std::cout << "B C 2" << std::endl;
    std::cout << "B D 4" << std::endl;
    std::cout << "C D 1" << std::endl;
    std::cout << "===============================" << std::endl;
}
