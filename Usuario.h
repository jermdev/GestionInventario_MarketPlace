#pragma once
#include <string>
using namespace std;

enum class ROL {
    CLIENTE,
    VENDEDOR,
    ADMINISTRADOR,
    NONE
};

class Usuario {
protected:
    int    id;
    string nombre;
    string correo;
    string direccion;
    string contrasenia;
    ROL rol;
public:
    Usuario()
        : id(0), nombre(""), correo(""), direccion(""), contrasenia(""), rol(ROL::NONE)
    {}

    virtual ~Usuario() {}

    // Metodos virtuales puros — hacen a Usuario abstracto.
    // Cada subclase declara su tipo y un ID numerico para serializar/discriminar.
    virtual string getTipoUsuario() const = 0;
    virtual int getTipoId() const = 0;
    virtual void mostrarUsuario() const = 0;

    // Getters
    int getId() { return id; }
    string getNombre( ){ return nombre; }
    string getCorreo(){ return correo; }
    string getDireccion() { return direccion; }
    string getContrasenia() { return contrasenia; }
    ROL getRol() { return rol; }
    // Setters
    void setId(int id) { this->id = id; }
    void setNombre(string nombre) { this->nombre = nombre; }
    void setCorreo(string correo) { this->correo = correo; }
    void setDireccion(string direccion) { this->direccion = direccion; }
    void setContrasenia(string contrasenia) { this->contrasenia = contrasenia; }
    void setRol(ROL rol) { this->rol = rol; }

    void actualizarContrasenia(const string contrasenia) {
        this->contrasenia = contrasenia;
    }
};
