#ifndef ENRUTADOR_DIRIGIDO_H
#define ENRUTADOR_DIRIGIDO_H

#include <string>
#include <map>
#include <vector>

class EnrutadorDirigido {
private:
    std::string nombre;
    std::map<std::string, int> tablaCostos;
    std::map<std::string, std::vector<std::string>> tablaCaminos;
    std::map<std::string, int> vecinosSalientes;

public:
    EnrutadorDirigido(const std::string& nombre);

    std::string obtenerNombre() const;
    std::map<std::string, int> obtenerTablaCostos() const;
    std::map<std::string, std::vector<std::string>> obtenerTablaCaminos() const;
    std::map<std::string, int> obtenerVecinosSalientes() const;

    void agregarVecinoSaliente(const std::string& vecino, int costo);
    void eliminarVecinoSaliente(const std::string& vecino);

    void actualizarTablaCostos(const std::string& destino, int costo);
    void actualizarTablaCaminos(const std::string& destino, const std::vector<std::string>& camino);
    void limpiarTablas();

    bool esVecinoSaliente(const std::string& enrutador) const;
    int obtenerCostoHacia(const std::string& destino) const;
    std::vector<std::string> obtenerCaminoHacia(const std::string& destino) const;
};

#endif
