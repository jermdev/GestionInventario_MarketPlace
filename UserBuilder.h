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
    string contrasenia;
    ROL rol;

public:
    UsuarioBuilder()
        : id(0), nombre(""), correo(""), direccion(""), contrasenia(""), rol(ROL::NONE)
    {}

    virtual ~UsuarioBuilder() {}

    void setId(int id) {
        this->id = id;
    }

    void setNombre(const string& nombre) {
        this->nombre = nombre;
    }

    void setCorreo(const string& correo) {
        this->correo = correo;
    }

    void setDireccion(const string& direccion) {
        this->direccion = direccion;
    }

    void setContrasenia(const string& contrasenia) {
        this->contrasenia = contrasenia;
    }

    void setRol(const ROL& rol) {
        this->rol = rol;
    }
    // Limpia todos los campos acumulados para reusar el builder.
    void reset() {
        id = 0;
        nombre = correo = direccion = contrasenia = "";
        rol = ROL::NONE;
    }

    // Construye el objeto Usuario concreto. El CALLER es dueno del puntero devuelto.
    virtual Usuario* build() = 0;
};
