#include "conexion_dirigida.h"

ConexionDirigida::ConexionDirigida(const std::string& orig, const std::string& dest, int cost)
    : origen(orig), destino(dest), costo(cost) {}

std::string ConexionDirigida::obtenerOrigen() const {
    return origen;
}

std::string ConexionDirigida::obtenerDestino() const {
    return destino;
}

int ConexionDirigida::obtenerCosto() const {
    return costo;
}

void ConexionDirigida::establecerCosto(int cost) {
    costo = cost;
}
