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

	CompraService(ProductoService*inventario,PedidoService*pedido, int idUsuario) {
		this->inventario = inventario;
		this->pedido = pedido;
		this->id = idUsuario;
	}
	~CompraService() {
		delete inventario;
		delete pedido;
	}

	void generarCompra(Carrito*carrito,MetodoPago metodo,TipoComprobante tipoComp) {
		Lista<NProductos*>* items = carrito->getProductos();

		if (items->esVacia()) {
			cout << "Carrito Vacio" << endl; return;
		}

		// Validar stock antes de aplicar cambios
		for (int i = 0; i < items->longitud(); i++) {
			NProductos* item = items->obtenerPos(i);
			int idProducto = item->producto->getId();

			Producto* prodInventario = inventario->buscarProductoPorId(0, [idProducto](Producto* p) {
				return p != nullptr && p->getId() == idProducto;
				});

			if (prodInventario == nullptr) {
				cout << "Producto con id " << idProducto << " no existe en inventario. Compra cancelada." << endl;
				return;
			}

			if (prodInventario->getStock() < item->cantidad) {
				cout << "Stock insuficiente para " << prodInventario->getNombre()
					<< ". Disponible: " << prodInventario->getStock()
					<< ", Solicitado: " << item->cantidad << ". Compra cancelada." << endl;
				return;
			}
		}

		// Aplicar cambios al inventario (objeto dentro de ProductoService) y preparar pedido
		double pesoTotal = 0, montoTotal = 0;
		Lista<Producto*>* productosEscogidos = new Lista<Producto*>();

		for (int i = 0; i < items->longitud(); i++) {

			NProductos* item = items->obtenerPos(i);
			int idProducto = item->producto->getId();

			Producto* prodInventario = inventario->buscarProductoPorId(0, [idProducto](Producto* p) {
				return p != nullptr && p->getId() == idProducto;
				});

			int nuevoStock = prodInventario->getStock() - item->cantidad;
			prodInventario->setStock(nuevoStock);

			// Si el producto del carrito es otra instancia, sincronizar su stock también
			item->producto->setStock(nuevoStock);

			montoTotal += (prodInventario->getPrecio() * item->cantidad);
			pesoTotal += (1.50 * item->cantidad);

			for (int j = 0; j < item->cantidad; j++) {
				productosEscogidos->agregaFinal(prodInventario);
			}
		}

		// Persistir cambios: ahora guardarProductos requiere el id del vendedor
		inventario->guardarProductos();

		Comprobante* com = new Comprobante(id, "05/02/2026", montoTotal, tipoComp);
		Pago* pag = new Pago(com, metodo, montoTotal);
		pag->realizarPago(metodo, montoTotal);

		Pedido* nuevoPedido = new Pedido(id, pesoTotal, PENDIENTEDEENVIO, productosEscogidos);
		pedido->registrarPedido(nuevoPedido);

		carrito->vaciarCarrito();
	}

	void cancelarCompra() {
		cout << "Compra Cancelada...." << endl;

	}


};
