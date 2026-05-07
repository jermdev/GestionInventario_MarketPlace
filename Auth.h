#pragma once
#include "UserRepository.h"
#include "HashUtils.h"
#include "ClienteBuilder.h"
#include "VendedoresBuilder.h"
#include "Usuario.h"
using namespace std;

// Fachada de autenticacion. Coordina repositorio, builders y hashing.
// Auth NO es dueno del repositorio (inyectado por constructor).
// Auth SI es dueno de usuarioActual (lo recibe de buscarPorCorreo y lo destruye).
class Auth {
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

private:
    UsuarioRepository* repo;
    Usuario*           usuarioActual;

public:
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

    ResultadoRegistro registrarCliente(const string& nombre,
                                       const string& correo,
                                       const string& direccion,
                                       const string& contrasenia) {
        if (repo->correoExiste(correo))
            return CORREO_DUPLICADO;

        int    nuevoId = repo->generarNuevoId();
        string hash    = HashUtils::hashear(contrasenia);

        ClienteBuilder b;
        Usuario* u = b.setId(nuevoId)
                      .setNombre(nombre)
                      .setCorreo(correo)
                      .setDireccion(direccion)
                      .setContraseniaHash(hash)
                      .build();

        Cliente* c = dynamic_cast<Cliente*>(u);
        if (c == nullptr) { delete u; return REGISTRO_ERROR; }

        repo->guardarCliente(c);
        delete c;
        return REGISTRO_EXITOSO;
    }

    ResultadoRegistro registrarVendedor(const string& nombre,
                                        const string& correo,
                                        const string& direccion,
                                        const string& contrasenia) {
        if (repo->correoExiste(correo))
            return CORREO_DUPLICADO;

        int    nuevoId = repo->generarNuevoId();
        string hash    = HashUtils::hashear(contrasenia);

        VendedorBuilder b;
        b.setNumeroEstrellas(0);
        Usuario* u = b.setId(nuevoId)
                      .setNombre(nombre)
                      .setCorreo(correo)
                      .setDireccion(direccion)
                      .setContraseniaHash(hash)
                      .build();

        Vendedor* v = dynamic_cast<Vendedor*>(u);
        if (v == nullptr) { delete u; return REGISTRO_ERROR; }

        repo->guardarVendedor(v);
        delete v;
        return REGISTRO_EXITOSO;
    }

    ResultadoLogin iniciarSesion(const string& correo, const string& contrasenia) {
        // Cerrar sesion anterior si la hubiera
        if (usuarioActual != nullptr) {
            delete usuarioActual;
            usuarioActual = nullptr;
        }

        Usuario* u = repo->buscarPorCorreo(correo);
        if (u == nullptr)
            return CREDENCIALES_INVALIDAS;

        string hashIngresado = HashUtils::hashear(contrasenia);
        if (hashIngresado != u->getContraseniaHash()) {
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
    bool     hayUsuarioActivo() const { return usuarioActual != nullptr; }
};
