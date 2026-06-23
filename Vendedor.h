#pragma once
#include <iostream>
#include "Usuario.h"
#include "Lista.h"
#include "Producto.h"
using namespace std;

class Vendedor : public Usuario {
private:
    Lista<Producto*>* productosEnVenta;
    int               numeroEstrellas;

public:
    // Constructor completo
    Vendedor(Lista<Producto*>* productosEnVenta, int numeroEstrellas)
        : productosEnVenta(productosEnVenta), numeroEstrellas(numeroEstrellas)
    {}

    // Constructor por defecto (necesario para deserializacion desde archivo)
    Vendedor()
        : productosEnVenta(new Lista<Producto*>()), numeroEstrellas(0)
    {}

    ~Vendedor() {
        delete productosEnVenta;
    }

    string getTipoUsuario() const override { return "Vendedor"; }
    int    getTipoId()      const override { return 2; }

    int               getNumeroEstrellas()              const { return numeroEstrellas; }
    void              setNumeroEstrellas(int estrellas)       { this->numeroEstrellas = estrellas; }
    Lista<Producto*>* getProductosEnVenta()                   { return productosEnVenta; }

    void listarProductos() {
        int nProductos = productosEnVenta->longitud();
        cout << "Tus Productos de Venta\n";
        cout << "N: " << nProductos << "\n";
        for (int i = 0; i < nProductos; i++) {
            Producto* p = productosEnVenta->obtenerPos(i);
            if (p == nullptr) return;
            p->MostrarProducto();
        }
    }

    void mostrarUsuario() const override {
        cout << "ID:" << this->id << endl;
        cout << "Nombre Completo: " << this->nombre << endl;
        cout << "Tipo de usuario: " << getTipoUsuario() << endl;
        cout << "Correo: " << this->correo;
        cout << "Direccion: " << this->direccion;
    }
};
