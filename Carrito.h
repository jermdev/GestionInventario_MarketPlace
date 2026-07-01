#pragma once
#include "Lista.h"
#include "Producto.h"
#include "Pila.h"
#include "HashTabla.hpp" 

struct NProductos {
	Producto* producto;
	int cantidad;
};

class Carrito {
private:
	int numeroProductos;
	double precioDelCarrito;
	Lista<NProductos*>* productos;
	HashTabla<int, NProductos*>* tablaCarrito;

public:
	Carrito() {
		this->productos = new Lista<NProductos*>();
		this->tablaCarrito = new HashTabla<int, NProductos*>(128); 
		this->numeroProductos = 0;
		this->precioDelCarrito = 0;
	}

	~Carrito() {
		for (uint i = 0; i < productos->longitud(); i++) {
			delete productos->obtenerPos(i);
		}
		delete productos;
		delete tablaCarrito;
	}

	Lista<NProductos*>* getProductos() { return productos; }
	double getPrecioTotal() { return precioDelCarrito; }

	void vaciarCarrito() {
		for (uint i = 0; i < productos->longitud(); i++) {
			delete productos->obtenerPos(i);
		}
		while (!productos->esVacia()) {
			productos->eliminaInicial();
		}

		delete tablaCarrito;
		tablaCarrito = new HashTabla<int, NProductos*>(128);

		numeroProductos = 0;
		precioDelCarrito = 0;
	}
	//Este metodo no se usa, porque ahora se realizo en la UI
	void listarCarrito() {
		int nProductos = productos->longitud();
		if (nProductos == 0) {
			
			return;
		}

		cout << "\n--- CARRITO DE COMPRAS ---\n";
		cout << "Total de items distintos: " << nProductos << "\n";
		cout << "Monto total acumulado: S/ " << precioDelCarrito << "\n";

		Pila<NProductos*> pilaCarrito;

		for (int i = 0; i < nProductos; i++) {
			pilaCarrito.push(productos->obtenerPos(i));
		}

		while (!pilaCarrito.estaVacia()) {
			NProductos* aux = pilaCarrito.pop();
			cout << "\n[Agregado Recientemente] Cantidad: " << aux->cantidad << "\n";
			aux->producto->MostrarProducto();

		}
		system("pause > nul");
	} 

	void agregarProducto(Producto* producto, int cantidadSolicitada) {
		if (producto->getStock() < cantidadSolicitada) {
			cout << "Stock insuficiente para solicitar " << cantidadSolicitada << " " << producto->getNombre() << endl;
			
			return;
		}

		int id = producto->getId();

		NProductos* existente = tablaCarrito->buscar(id);
		
		if (existente != nullptr) {
			if (producto->getStock() < (existente->cantidad + cantidadSolicitada)) {
				cout << "Stock insuficiente para agregar mas unidades de " << producto->getNombre() << endl;
				return;
			}
			existente->cantidad += cantidadSolicitada;
		}
		else {
			NProductos* solicitudProducto = new NProductos{ producto, cantidadSolicitada };
			productos->agregaInicial(solicitudProducto);
			tablaCarrito->insertar(id, solicitudProducto);
		}

		numeroProductos += cantidadSolicitada;
		precioDelCarrito += (producto->getPrecio() * cantidadSolicitada);
		
	}

	void borrarProducto(int id, int cantidad) {
		NProductos* item = tablaCarrito->buscar(id);

		if (item == nullptr) {
			cout << "Elemento con el id: " << id << " no encontrado en el carrito." << endl;
			return;
		}

		if (cantidad >= item->cantidad) {
			int cantidadEliminada = item->cantidad;

			for (uint i = 0; i < productos->longitud(); i++) {
				if (productos->obtenerPos(i)->producto->getId() == id) {
					productos->eliminaPos(i);
					break;
				}
			}

			tablaCarrito->eliminarValor([id](NProductos* n) { return n->producto->getId() == id; });

			numeroProductos -= cantidadEliminada;
			precioDelCarrito -= (item->producto->getPrecio() * cantidadEliminada);
			delete item;

		
		}
		else {
			item->cantidad -= cantidad;
			numeroProductos -= cantidad;
			precioDelCarrito -= (item->producto->getPrecio() * cantidad);
			cout << "Se redujeron " << cantidad << " unidades de " << item->producto->getNombre() << "." << endl;
		}
	}
};