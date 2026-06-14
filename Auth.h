#pragma once
#include <string>
#include "UserRepository.h"
#include "ClienteBuilder.h"
#include "VendedoresBuilder.h"
#include "AdministradorBuilder.h"
#include "Usuario.h"
using namespace std;

// Fachada de autenticacion. Coordina repositorio, builders y hashing.
// Auth NO es dueno del repositorio (inyectado por constructor).
// Auth SI es dueno de usuarioActual (lo recibe de buscarPorCorreo y lo destruye).
class Auth {
 
private:
    UsuarioRepository* repo;
    Usuario* usuarioActual;

public:

    enum ResultadoRegistro {
        REGISTRO_EXITOSO,
        CORREO_DUPLICADO,
        REGISTRO_ERROR
    };

    enum ResultadoLogin {
        LOGIN_EXITOSO,
        CREDENCIALES_INVALIDAS,
        LOGIN_ERROR
    };

    explicit Auth(UsuarioRepository* repo)
        : repo(repo), usuarioActual(nullptr)
    {}

    ~Auth() {
        if (usuarioActual != nullptr) {
            delete usuarioActual;
            usuarioActual = nullptr;
        }
        // repo NO se destruye aqui — el caller (main) es dueno del repo
    }

    ResultadoRegistro registrarCliente(string nombre,string correo,string direccion,string contrasenia) {
        if (repo->correoExiste(correo))
            return CORREO_DUPLICADO;

        int    nuevoId = repo->generarNuevoId();

        ClienteBuilder b;
        Usuario* u = nullptr;
        b.setId(nuevoId);
        b.setNombre(nombre);
        b.setCorreo(correo);
        b.setDireccion(direccion);
        b.setContrasenia(contrasenia);
        b.setRol(ROL::CLIENTE);
        u = b.build();

        Cliente* c = dynamic_cast<Cliente*>(u);
        if (c == nullptr) { delete u; return REGISTRO_ERROR; }

        repo->guardarCliente(c);
        delete c;
        return REGISTRO_EXITOSO;
    }

    ResultadoRegistro registrarVendedor(string nombre,string correo,string direccion,string contrasenia) {
        if (repo->correoExiste(correo))
            return CORREO_DUPLICADO;

        int nuevoId = repo->generarNuevoId();

        VendedorBuilder b;
        b.setNumeroEstrellas(0);
        Usuario* u = nullptr;
        b.setId(nuevoId);
        b.setNombre(nombre);
        b.setCorreo(correo);
        b.setDireccion(direccion);
        b.setContrasenia(contrasenia);
        b.setRol(ROL::VENDEDOR);
        u = b.build();

        Vendedor* v = dynamic_cast<Vendedor*>(u);
        if (v == nullptr) { delete u; return REGISTRO_ERROR; }

        repo->guardarVendedor(v);
        delete v;
        return REGISTRO_EXITOSO;
    }

    ResultadoRegistro registrarAdministrador(string nombre, string correo, string direccion, string contrasenia, string cargo) {
        if (repo->correoExiste(correo))
            return CORREO_DUPLICADO;

        int nuevoId = repo->generarNuevoId();

        AdministradorBuilder b;
        Usuario* u = nullptr;
        b.setCargo(cargo);
        b.setId(nuevoId);
        b.setNombre(nombre);
        b.setCorreo(correo);
        b.setDireccion(direccion);
        b.setContrasenia(contrasenia);
        b.setRol(ROL::ADMINISTRADOR);
        u = b.build();

        Administrador* v = dynamic_cast<Administrador*>(u);
        if (v == nullptr) { delete u; return REGISTRO_ERROR; }

        repo->guardarAdministrador(v);
        delete v;
        return REGISTRO_EXITOSO;
    }

    ResultadoLogin iniciarSesion(const string& correo, const string& contrasenia) {
        // Cerrar sesion anterior si la hubiera
        if (usuarioActual != nullptr) {
            delete usuarioActual;
            usuarioActual = nullptr;
        }

        Usuario* u = repo->buscarPorCondicion([correo](Usuario* u) {return u->getCorreo() == correo; });
        if (u == nullptr)
            return CREDENCIALES_INVALIDAS;

        //string hashIngresado = HashUtils::hashear(contrasenia);
        if (contrasenia != u->getContrasenia()) {
            delete u;
            return CREDENCIALES_INVALIDAS;
        }

        usuarioActual = u; // Auth toma ownership del puntero
        return LOGIN_EXITOSO;
    }

    void cerrarSesion() {
        if (usuarioActual != nullptr) {
            delete usuarioActual;
            usuarioActual = nullptr;
        }
    }

    Usuario* getUsuarioActual() const { return usuarioActual; }
    bool hayUsuarioActivo() const { return usuarioActual != nullptr; }
};
