#ifndef CONEXION_DIRIGIDA_H
#define CONEXION_DIRIGIDA_H

#include <string>

class ConexionDirigida {
private:
    std::string origen;
    std::string destino;
    int costo;

public:
    ConexionDirigida(const std::string& orig, const std::string& dest, int cost);

    std::string obtenerOrigen() const;
    std::string obtenerDestino() const;
    int obtenerCosto() const;

    void establecerCosto(int cost);
};

#endif
