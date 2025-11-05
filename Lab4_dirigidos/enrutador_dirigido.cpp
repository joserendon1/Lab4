#include "enrutador_dirigido.h"
#include <algorithm>

EnrutadorDirigido::EnrutadorDirigido(const std::string& nombre) : nombre(nombre) {}

std::string EnrutadorDirigido::obtenerNombre() const {
    return nombre;
}

std::map<std::string, int> EnrutadorDirigido::obtenerTablaCostos() const {
    return tablaCostos;
}

std::map<std::string, std::vector<std::string>> EnrutadorDirigido::obtenerTablaCaminos() const {
    return tablaCaminos;
}

std::map<std::string, int> EnrutadorDirigido::obtenerVecinosSalientes() const {
    return vecinosSalientes;
}

void EnrutadorDirigido::agregarVecinoSaliente(const std::string& vecino, int costo) {
    vecinosSalientes[vecino] = costo;
}

void EnrutadorDirigido::eliminarVecinoSaliente(const std::string& vecino) {
    vecinosSalientes.erase(vecino);
}

void EnrutadorDirigido::actualizarTablaCostos(const std::string& destino, int costo) {
    tablaCostos[destino] = costo;
}

void EnrutadorDirigido::actualizarTablaCaminos(const std::string& destino, const std::vector<std::string>& camino) {
    tablaCaminos[destino] = camino;
}

void EnrutadorDirigido::limpiarTablas() {
    tablaCostos.clear();
    tablaCaminos.clear();
}

bool EnrutadorDirigido::esVecinoSaliente(const std::string& enrutador) const {
    return vecinosSalientes.find(enrutador) != vecinosSalientes.end();
}

int EnrutadorDirigido::obtenerCostoHacia(const std::string& destino) const {
    auto it = tablaCostos.find(destino);
    if (it != tablaCostos.end()) {
        return it->second;
    }
    return -1;
}

std::vector<std::string> EnrutadorDirigido::obtenerCaminoHacia(const std::string& destino) const {
    auto it = tablaCaminos.find(destino);
    if (it != tablaCaminos.end()) {
        return it->second;
    }
    return std::vector<std::string>();
}
