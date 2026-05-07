#pragma once
#include "UserBuilder.h"
#include "Cliente.h"

class ClienteBuilder : public UsuarioBuilder {
public:
    ClienteBuilder() {}
    ~ClienteBuilder() override {}

    // Construye un Cliente con listas vacias.
    // Las colecciones (productos, pedidos) se cargan posteriormente por sus repositorios.
    // build() llama reset() antes de retornar — el builder queda limpio para reuso.
    Usuario* build() override {
        Cliente* c = new Cliente(
            new Lista<Producto*>(),
            new Lista<Pedido*>(),
            new Carrito()
        );
        c->setId(id);
        c->setNombre(nombre);
        c->setCorreo(correo);
        c->setDireccion(direccion);
        c->setContraseniaHash(contraseniaHash);
        reset();
        return c;
    }
};
