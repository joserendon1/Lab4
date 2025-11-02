#ifndef ENRUTADOR_H
#define ENRUTADOR_H

#include <string>
#include <map>
#include <vector>

class Enrutador {
private:
    std::string nombre;
    std::map<std::string, int> tablaCostos;
    std::map<std::string, std::vector<std::string>> tablaCaminos;
    std::map<std::string, int> vecinos;

public:
    Enrutador(const std::string& nombre);

    std::string obtenerNombre() const;
    std::map<std::string, int> obtenerTablaCostos() const;
    std::map<std::string, std::vector<std::string>> obtenerTablaCaminos() const;
    std::map<std::string, int> obtenerVecinos() const;

    void agregarVecino(const std::string& vecino, int costo);
    void eliminarVecino(const std::string& vecino);

    void actualizarTablaCostos(const std::string& destino, int costo);
    void actualizarTablaCaminos(const std::string& destino, const std::vector<std::string>& camino);
    void limpiarTablas();

    bool esVecino(const std::string& enrutador) const;
    int obtenerCostoHacia(const std::string& destino) const;
    std::vector<std::string> obtenerCaminoHacia(const std::string& destino) const;
};

#endif
