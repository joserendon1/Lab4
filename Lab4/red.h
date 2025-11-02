#ifndef RED_H
#define RED_H

#include "enrutador.h"
#include "conexion.h"
#include <vector>
#include <map>
#include <string>

class Red {
private:
    std::string nombre;
    std::map<std::string, Enrutador*> enrutadores;
    std::vector<Conexion*> conexiones;

    void ejecutarAlgoritmoRutas();
    void actualizarTablasTodosEnrutadores();
    void dijkstra(const std::string& origen);

public:
    Red(const std::string& nombre);
    ~Red();

    void agregarEnrutador(const std::string& nombre);
    void eliminarEnrutador(const std::string& nombre);
    bool existeEnrutador(const std::string& nombre) const;

    void agregarConexion(const std::string& origen, const std::string& destino, int costo);
    void eliminarConexion(const std::string& origen, const std::string& destino);

    void cargarDesdeArchivo(const std::string& nombreArchivo);
    void generarRedAleatoria(int numEnrutadores, double probabilidadConexion, int costoMaximo = 10);

    void imprimirTablaEnrutador(const std::string& nombre) const;
    void encontrarYMostrarCamino(const std::string& origen, const std::string& destino) const;

    std::vector<std::string> obtenerNombresEnrutadores() const;
    std::vector<Conexion*> obtenerConexiones() const;

    void limpiarRed();
};

#endif
