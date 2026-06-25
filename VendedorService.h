#pragma once
#include "Producto.h"
#include "Pila.h"
#include "ProductoService.h"

//utilidades
#include "idAleatorio.h"

class VendedorService {
private:
	ProductoService* productoService;

public:
	VendedorService() {
		this->productoService = new ProductoService();
	}

	~VendedorService() {

	}

	// Para el vendedor
	void inicializarListaProductos() {
		productoService->iniciaizarProductos();
	}

	void agregarProducto(int idVendedor, string nombre, string categoria, double precio, int stock) {
		int id = idAleatorioPorDigitos(6);
		Producto* nuevoProducto = new Producto(nombre, categoria, precio, id, idVendedor, stock);
		productoService->agregarProducto(nuevoProducto, idVendedor);

	}
	void mostrarProductosPorVendedor(int idVendedor) {
		Lista<Producto*>* productos = productoService->obtenerPorductosPorCondicion(0, [idVendedor](Producto* p) { return p->getIdVendedor() == idVendedor; });

		Pila<Producto*> pilaProductos;

		int index = productos->longitud() + 1;
		for (int i = 0; i < productos->longitud(); i++) {
			pilaProductos.push(productos->obtenerPos(i));

			cout << "\n--- Tus productos (Mas recientes primero) ---" << endl;
			while (!pilaProductos.estaVacia()) {
				cout << "\nN°" << --index << endl;
				pilaProductos.pop()->MostrarProducto();
			}
		}
	}
	void eliminarProductosPorVendedor(int idVendedor, int idProducto) {
		// Buscamos directamente el producto validando que exista y le pertenezca a este vendedor
		Producto* p = productoService->buscarProductoPorId(0, [idVendedor, idProducto](Producto* prod) {
			return prod->getId() == idProducto && prod->getIdVendedor() == idVendedor;
			});

		if (p != nullptr) {
			// Si lo encuentra, lo elimina
			productoService->eliminarProducto(idProducto);
			cout << "Producto eliminado exitosamente." << endl;
		}
		else {
			// Si no lo encuentra, o si el producto es de otro vendedor
			cout << "Error: No se encontro el producto o no te pertenece." << endl;
		}
	}
	Lista<Producto*>* obtenerListaProductosVendedor(int idVendedor) {
		return productoService->obtenerPorductosPorCondicion(0, [idVendedor](Producto* p) {
			return p->getIdVendedor() == idVendedor;
			});
	}
};
