#pragma once
#include "UserBuilder.h"
#include "Vendedor.h"


class VendedorBuilder : public UsuarioBuilder {
private:
    int numeroEstrellas;

public:
    VendedorBuilder() : numeroEstrellas(0) {}
    ~VendedorBuilder() override {}

    VendedorBuilder& setNumeroEstrellas(int estrellas) {
        this->numeroEstrellas = estrellas;
        return *this;
    }

    // Construye un Vendedor con lista de productos vacia.
    // build() llama reset() antes de retornar — el builder queda limpio para reuso.
    Usuario* build() override {
        Vendedor* v = new Vendedor(
            new Lista<Producto*>(),
            numeroEstrellas
        );
        v->setId(id);
        v->setNombre(nombre);
        v->setCorreo(correo);
        v->setDireccion(direccion);
        v->setContrasenia(contrasenia);
        reset();
        numeroEstrellas = 0;
        return v;
    }
};
