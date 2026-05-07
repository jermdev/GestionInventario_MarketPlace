#pragma once
#include <iostream>
#include "Usuario.h"
#include "Lista.h"
#include "Producto.h"
#include "Pedido.h"
#include "Carrito.h"
using namespace std;

class Cliente : public Usuario {
private:
    Lista<Producto*>* productosComprados;
    Lista<Pedido*>*   pedidos;
    Carrito*          carrito;

public:
    // Constructor completo (uso directo desde codigo)
    Cliente(Lista<Producto*>* lsProductosComprados,
            Lista<Pedido*>*   lsPedidos,
            Carrito*          carrito)
        : productosComprados(lsProductosComprados),
          pedidos(lsPedidos),
          carrito(carrito)
    {}

    // Constructor por defecto (necesario para deserializacion desde archivo)
    Cliente()
        : productosComprados(new Lista<Producto*>()),
          pedidos(new Lista<Pedido*>()),
          carrito(new Carrito())
    {}

    ~Cliente() {
        delete productosComprados;
        delete pedidos;
        delete carrito;
    }

    string getTipoUsuario() const override { return "Cliente"; }
    int    getTipoId()      const override { return 1; }

    int               getNumeroCompras()         { return productosComprados->longitud(); }
    Lista<Producto*>* getProductosComprados()    { return productosComprados; }
    Lista<Pedido*>*   getPedidos()               { return pedidos; }
    Carrito*          getCarrito()               { return carrito; }

    void listarPedidos() {
        int nCompras = getNumeroCompras();
        cout << "Tus Productos\n";
        cout << "N: " << nCompras << "\n";
        for (int i = 0; i < nCompras; i++) {
            Producto* p = productosComprados->obtenerPos(i);
            if (p == nullptr) return;
            p->MostrarProducto();
        }
    }
};
