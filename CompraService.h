#pragma once
#include <iostream>
#include <string>
#include "Carrito.h"
#include "ProductoService.h"
#include "PedidoService.h"
#include "Pedido.h"
#include "Pago.h"
#include "fechas.h"
using namespace std;

enum EstadoCompra {
	PAGADO,
	PAGOPENDIENTE
};


class CompraService {
private:

	ProductoService* inventario;
	PedidoService* pedido;


public:

	// Ahora recibe también el id del vendedor cuyo inventario se está usando
	CompraService(ProductoService* inventario, PedidoService* pedido) {
		this->inventario = inventario;
		this->pedido = pedido;
	}
	~CompraService() {
		// No eliminar inventario ni pedido aquí: son inyectados y su ciclo de vida lo debe controlar el creador.
	}

	void generarCompra(Cliente* cli, MetodoPago metodo, TipoComprobante tipoComp) {
		// recuperamos los productos que estan en el carrito
		Lista<NProductos*>* items = cli->getCarrito()->getProductos();

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
		Lista<NProductos*>* productosEscogidos = new Lista<NProductos*>();

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

			// Copiar la línea NProductos al pedido (nueva instancia)
			NProductos* np = new NProductos{ prodInventario, item->cantidad };
			productosEscogidos->agregaFinal(np);
		}

		// Persistir cambios: ahora guardarProductos requiere el id del vendedor
		inventario->guardarProductos();
		string fechaActual = obtenerFechaActual();
		Comprobante* com = new Comprobante(cli->getId(), fechaActual, montoTotal, tipoComp);
		Pago* pag = new Pago(com, metodo, montoTotal);
		pag->realizarPago(metodo, montoTotal);

		// generar fechaEntrega y pasarla al pedido
		string fechaEntrega = generarFechaEntrega();
		Pedido* nuevoPedido = new Pedido(cli->getId(), pesoTotal, PENDIENTEDEENTREGA, productosEscogidos, fechaEntrega);
		pedido->registrarPedido(nuevoPedido);
		pedido->registrarPedido(nuevoPedido); // registrar una vez suficiente; si hay lógica extra ajustar

		cli->getCarrito()->vaciarCarrito();
	}

	void cancelarCompra() {
		cout << "Compra Cancelada...." << endl;
	}

};