#ifndef CONEXION_H
#define CONEXION_H

#include <string>

class Conexion {
private:
    std::string origen;
    std::string destino;
    int costo;

public:
    Conexion(const std::string& orig, const std::string& dest, int cost);

    std::string obtenerOrigen() const;
    std::string obtenerDestino() const;
    int obtenerCosto() const;

    void establecerCosto(int cost);
};

#endif
