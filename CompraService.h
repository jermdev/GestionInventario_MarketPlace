#pragma once
#include <iostream>
#include <string>
#include "Carrito.h"
#include "ProductoService.h"
#include "PedidoService.h"
#include "Pedido.h"
#include "Pago.h"
using namespace std;

enum EstadoCompra {
	PAGADO,
	PAGOPENDIENTE
};


class CompraService {
private:

	ProductoService* inventario = new ProductoService();
	PedidoService* pedido = new PedidoService();
	int id;

public:

	CompraService(ProductoService*inventario,PedidoService*pedido) {
		this->inventario = inventario;
		this->pedido = pedido;
		this->id = 1000;
	}
	~CompraService() {
		delete inventario;
		delete pedido;
	}

	void generarCompra(Carrito*carrito,MetodoPago metodo,TipoComprobante tipoComp) {
		//recuperamos los productos que estan en el carrito
		Lista<NProductos*>* items = carrito->getProductos();

		if (items->esVacia()) {
			cout << "Carrito Vacio" << endl; return;
		}
		double pesoTotal = 0, montoTotal = 0;

		Lista<Producto*>* productosEscogidos=new Lista<Producto*>();
		for (int i = 0; i < items->longitud(); i++) {

			NProductos* item = items->obtenerPos(i);
			//Se resta y acomoda al nuevo stock
			int nuevoStock = item->producto->getStock() - item->cantidad;
			item->producto->setStock(nuevoStock);

			montoTotal += (item->producto->getPrecio() * item->cantidad);
			//Se puede cambiar de peso
			pesoTotal += (1.50 * item->cantidad);

			for (int j = 0; j < item->cantidad; j++) {
				productosEscogidos->agregaFinal(item->producto);
			}
		}
		inventario->guardarProductos();

		Comprobante* com = new Comprobante(id, "05/02/2026", montoTotal, tipoComp);
		Pago* pag = new Pago(com, metodo, montoTotal);
		pag->realizarPago(metodo, montoTotal);


		Pedido* nuevoPedido = new Pedido(id++, pesoTotal, PENDIENTEDEENVIO, productosEscogidos);
		pedido->registrarPedido(nuevoPedido);

		carrito->vaciarCarrito();

	}

	void cancelarCompra() {
		cout << "Compra Cancelada...." << endl;

	}


};
