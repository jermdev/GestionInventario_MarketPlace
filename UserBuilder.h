#pragma once
#include <string>
#include "Usuario.h"
using namespace std;

// Builder abstracto para construir objetos Usuario.
// Fluent interface: cada setter retorna UsuarioBuilder& para encadenamiento.
// Subclases concretas implementan build() y crean el tipo especifico de usuario.
class UsuarioBuilder {
protected:
    int    id;
    string nombre;
    string correo;
    string direccion;
    string contraseniaHash;

public:
    UsuarioBuilder()
        : id(0), nombre(""), correo(""), direccion(""), contraseniaHash("")
    {}

    virtual ~UsuarioBuilder() {}

    UsuarioBuilder& setId(int id) {
        this->id = id;
        return *this;
    }

    UsuarioBuilder& setNombre(const string& nombre) {
        this->nombre = nombre;
        return *this;
    }

    UsuarioBuilder& setCorreo(const string& correo) {
        this->correo = correo;
        return *this;
    }

    UsuarioBuilder& setDireccion(const string& direccion) {
        this->direccion = direccion;
        return *this;
    }

    UsuarioBuilder& setContraseniaHash(const string& hash) {
        this->contraseniaHash = hash;
        return *this;
    }

    // Limpia todos los campos acumulados para reusar el builder.
    void reset() {
        id = 0;
        nombre = correo = direccion = contraseniaHash = "";
    }

    // Construye el objeto Usuario concreto. El CALLER es dueno del puntero devuelto.
    virtual Usuario* build() = 0;
};
