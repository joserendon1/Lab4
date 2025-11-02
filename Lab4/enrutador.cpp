#include "enrutador.h"
#include <algorithm>

Enrutador::Enrutador(const std::string& nombre) : nombre(nombre) {}

std::string Enrutador::obtenerNombre() const {
    return nombre;
}

std::map<std::string, int> Enrutador::obtenerTablaCostos() const {
    return tablaCostos;
}

std::map<std::string, std::vector<std::string>> Enrutador::obtenerTablaCaminos() const {
    return tablaCaminos;
}

std::map<std::string, int> Enrutador::obtenerVecinos() const {
    return vecinos;
}

void Enrutador::agregarVecino(const std::string& vecino, int costo) {
    vecinos[vecino] = costo;
}

void Enrutador::eliminarVecino(const std::string& vecino) {
    vecinos.erase(vecino);
}

void Enrutador::actualizarTablaCostos(const std::string& destino, int costo) {
    tablaCostos[destino] = costo;
}

void Enrutador::actualizarTablaCaminos(const std::string& destino, const std::vector<std::string>& camino) {
    tablaCaminos[destino] = camino;
}

void Enrutador::limpiarTablas() {
    tablaCostos.clear();
    tablaCaminos.clear();
}

bool Enrutador::esVecino(const std::string& enrutador) const {
    return vecinos.find(enrutador) != vecinos.end();
}

int Enrutador::obtenerCostoHacia(const std::string& destino) const {
    auto it = tablaCostos.find(destino);
    if (it != tablaCostos.end()) {
        return it->second;
    }
    return -1;
}

std::vector<std::string> Enrutador::obtenerCaminoHacia(const std::string& destino) const {
    auto it = tablaCaminos.find(destino);
    if (it != tablaCaminos.end()) {
        return it->second;
    }
    return std::vector<std::string>();
}
