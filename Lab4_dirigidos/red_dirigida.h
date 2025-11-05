#ifndef RED_DIRIGIDA_H
#define RED_DIRIGIDA_H

#include "enrutador_dirigido.h"
#include "conexion_dirigida.h"
#include <vector>
#include <map>
#include <string>

class RedDirigida {
private:
    std::string nombre;
    std::map<std::string, EnrutadorDirigido*> enrutadores;
    std::vector<ConexionDirigida*> conexiones;

    void ejecutarAlgoritmoRutas();
    void dijkstra(const std::string& origen);
    void limpiarRed();

public:
    RedDirigida(const std::string& nombre);
    ~RedDirigida();

    void agregarEnrutador(const std::string& nombre);
    void eliminarEnrutador(const std::string& nombre);
    bool existeEnrutador(const std::string& nombre) const;

    void agregarConexionDirigida(const std::string& origen, const std::string& destino, int costo);
    void eliminarConexion(const std::string& origen, const std::string& destino);
    void actualizarCostoConexion(const std::string& origen, const std::string& destino, int nuevoCosto);

    void cargarDesdeArchivo(const std::string& nombreArchivo);
    void generarRedAleatoria(int numEnrutadores, double probabilidadConexion, int costoMaximo = 10);

    void imprimirTablaEnrutador(const std::string& nombre) const;
    void encontrarYMostrarCamino(const std::string& origen, const std::string& destino) const;

    std::vector<std::string> obtenerNombresEnrutadores() const;
    std::vector<ConexionDirigida*> obtenerConexiones() const;
    void mostrarTopologia() const;
    void mostrarFormatoArchivo() const;
};

#endif
