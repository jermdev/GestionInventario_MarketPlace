#pragma once
#include "Usuario.h"
#include "Producto.h"
#include "Lista.h"

class Vendedor : public Usuario {
private:
	Lista<Producto*>* productosEnVenta;
	int numeroEstrellas;

public:
	Vendedor(Lista<Producto*>* prodcutosEnVenta, int numeroEstrellas) : Usuario() {

		this->productosEnVenta = prodcutosEnVenta;
		this->numeroEstrellas = numeroEstrellas;
	}

	~Vendedor() {
	}

	void listarPedidos() {
		int nProductos = productosEnVenta->longitud();
		cout << "Tus Productos de Venta \n";
		cout << "N° " << nProductos << "\n";

		Producto* aux = productosEnVenta->obtenerInicial();
		for (int i = 0; i < nProductos; i++) {
			aux->MostrarProducto();
			aux = productosEnVenta->obtenerPos(i);
		}

	}
};
