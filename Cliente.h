#pragma once
#include "Usuario.h"
#include "Lista.h"
#include "Producto.h"
#include "Pedido.h"
#include "Carrito.h"
class Cliente : public Usuario {
private:
	Lista<Producto*>* productosComprados;
	Lista<Pedido*>* pedidos;
	Carrito* carrito;
public:
	Cliente(Lista<Producto*>* lsProductosComprados, Lista<Pedido*>* lsPedidos, Carrito* carrito) {
		this->productosComprados = lsProductosComprados;
		this->pedidos = lsPedidos;
		this->carrito = carrito;
	}

	~Cliente()
	{
	}

	void listarPedidos() {
		int nCompras = getNumeroCompras();
		cout << "Tus Productos \n";
		cout << "N� " << nCompras << "\n";

		Producto* aux = productosComprados->obtenerInicial();
		for (int i = 0; i < nCompras; i++) {
			if (aux == nullptr) return;
			aux->MostrarProducto();
			aux = productosComprados->obtenerPos(i + 1);
		}

	}

	int getNumeroCompras() {
		return productosComprados->longitud();
	}

};
