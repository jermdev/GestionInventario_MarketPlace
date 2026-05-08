#pragma once
#include<iostream>
#include"Lista.h"
#include"Producto.h"

enum EstadoPedido {
	ENTREGADO,
	PENDIENTEDEENVIO,
	PENDIENTEDEENTREGA,
	CANCELADO
};

class Pedido
{
private:

	int idPedido;
	double peso;
	EstadoPedido estadopedido;
	Lista<Producto*>* productosComprados;

public:
	Pedido(int idPedido,double peso, EstadoPedido estadopedido, Lista<Producto*>* productosComprados)
	{
		this->idPedido = idPedido;
		this->peso = peso;
		this->estadopedido = estadopedido;
		this->productosComprados = productosComprados;
	}

	~Pedido()
	{
	}
	int getIdPedido() { return idPedido; }

	EstadoPedido getEstadoPedido() { return estadopedido; }

	double getPeso() { return peso; }
	Lista<Producto*>* getProductosComprados() { return productosComprados; }

	void setEstadoPedido(EstadoPedido nuevoEstado) {
		this->estadopedido = nuevoEstado;
	}

	void mostrarDetallePedido() {
		cout << "\n===== DETALLE DEL PEDIDO =====" << endl;
		cout << "ID Pedido: " << idPedido << endl;
		cout << "Peso: " << peso << " kg" << endl;

		cout << "Estado: ";
		switch (estadopedido) {
		case ENTREGADO: cout << "Entregado"; break;
		case PENDIENTEDEENVIO: cout << "Pendiente de Envio"; break;
		case PENDIENTEDEENTREGA: cout << "Pendiente de Entrega"; break;
		case CANCELADO: cout << "Cancelado"; break;
		}
		cout << endl;

		cout << "Cantidad de productos: " << productosComprados->longitud() << endl;
		cout << "==============================" << endl;
	}
};

