#include "conexion.h"

Conexion::Conexion(const std::string& orig, const std::string& dest, int cost)
    : origen(orig), destino(dest), costo(cost) {}

std::string Conexion::obtenerOrigen() const {
    return origen;
}

std::string Conexion::obtenerDestino() const {
    return destino;
}

int Conexion::obtenerCosto() const {
    return costo;
}

void Conexion::establecerCosto(int cost) {
    costo = cost;
}
