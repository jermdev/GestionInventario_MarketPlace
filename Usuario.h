#pragma once
#include <string>
using namespace std;

class Usuario {
protected:
    int    id;
    string nombre;
    string correo;
    string direccion;
    string contraseniaHash;

public:
    Usuario()
        : id(0), nombre(""), correo(""), direccion(""), contraseniaHash("")
    {}

    virtual ~Usuario() {}

    // Metodos virtuales puros — hacen a Usuario abstracto.
    // Cada subclase declara su tipo y un ID numerico para serializar/discriminar.
    virtual string getTipoUsuario() const = 0;
    virtual int    getTipoId()      const = 0;

    // Getters
    int    getId()              const { return id; }
    string getNombre()          const { return nombre; }
    string getCorreo()          const { return correo; }
    string getDireccion()       const { return direccion; }
    string getContraseniaHash() const { return contraseniaHash; }

    // Setters
    void setId(int id)                         { this->id = id; }
    void setNombre(const string& nombre)       { this->nombre = nombre; }
    void setCorreo(const string& correo)       { this->correo = correo; }
    void setDireccion(const string& direccion) { this->direccion = direccion; }
    void setContraseniaHash(const string& hash){ this->contraseniaHash = hash; }

    void actualizarContrasenia(const string& nuevoHash) {
        this->contraseniaHash = nuevoHash;
    }
};
