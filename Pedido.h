#pragma once
#include<iostream>
#include"Lista.h"
#include"Producto.h"
#include"Carrito.h" // para NProductos
using namespace std;

enum EstadoPedido {
	ENTREGADO,
	PENDIENTEDEENTREGA,
	CANCELADO,
	INDEFINIDO
};

class Pedido
{
private:

	int idPedido;
	int idCliente;
	double peso;
	EstadoPedido estadopedido;
	Lista<NProductos*>* productosComprados; // ahora guarda NProductos (producto + cantidad)
	string fechaEntrega;

public:
	// Nuevo constructor que recibe Lista<NProductos*>*
	Pedido(int idPedido, int idCliente,double peso, EstadoPedido estadopedido, Lista<NProductos*>* productosComprados, string fechaEntrega) {
		this->idPedido = idPedido;
		this->idCliente = idCliente;
		this->peso = peso;
		this->estadopedido = estadopedido;
		this->productosComprados = productosComprados;
		this->fechaEntrega = fechaEntrega;
	}

	Pedido(int id) {
		this->idPedido = id;
		this->idCliente = 0;
		this->peso = 0;
		this->estadopedido = INDEFINIDO;
		this->productosComprados = nullptr;
		this->fechaEntrega = "";
	}

	Pedido() {
		this->idPedido = 0;
		this->idCliente = 0;
		this->peso = 0;
		this->estadopedido = INDEFINIDO;
		this->productosComprados = nullptr;
		this->fechaEntrega = "";
	}

	~Pedido()
	{
		// Liberar la lista de NProductos; no se asume propiedad de Producto*
		delete productosComprados;
	}
	int getIdPedido() { return idPedido; }
	int getIdCliente() { return idCliente; }

	EstadoPedido getEstadoPedido() { return estadopedido; }

	double getPeso() { return peso; }
	Lista<NProductos*>* getProductosComprados() { return productosComprados; }
	
	string getFechaEntrega() {
		return fechaEntrega;
	}
	void setEstadoPedido(EstadoPedido nuevoEstado) {
		this->estadopedido = nuevoEstado;
	}

	void setFechaEntrega(string fecha) { this->fechaEntrega = fecha; }
	void setPeso(double peso) { this->peso = peso; }

	void mostrarDetallePedido() {
		cout << "\n===== DETALLE DEL PEDIDO =====" << endl;
		cout << "ID Pedido: " << idPedido << endl;
		cout << "Peso: " << peso << " kg" << endl;

		cout << "Estado: ";
		switch (estadopedido) {
		case ENTREGADO: cout << "Entregado"; break;
		case PENDIENTEDEENTREGA: cout << "Pendiente de Entrega"; break;
		case CANCELADO: cout << "Cancelado"; break;
		}
		cout << endl;

		int cantidadItems = productosComprados->longitud();
		cout << "Cantidad de lineas de pedido: " << cantidadItems << endl;
		for (int i = 0; i < cantidadItems; ++i) {
			NProductos* np = productosComprados->obtenerPos(i);
			if (np == nullptr) continue;
			cout << " - Producto ID: " << (np->producto ? np->producto->getId() : 0)
				<< " Nombre: " << (np->producto ? np->producto->getNombre() : "desconocido")
				<< " Cantidad: " << np->cantidad << endl;
		}

		cout << "Fecha Entrega: " << fechaEntrega << endl;
		cout << "==============================" << endl;
	}
};

