#pragma once
#include "Lista.h"
#include "Producto.h"
#include "Pila.h"
struct NProductos {
	Producto* producto;
	int cantidad;
};

class Carrito {
private:
	int numeroProductos;
	Lista<NProductos*>* productos;
	double precioDelCarrito;
public:

	Carrito() {
		this->productos = new Lista<NProductos*>();
		this->numeroProductos = 0;
		this->precioDelCarrito = 0;
	}

	~Carrito() {
		delete productos;
	}


	Lista<NProductos*>* getProductos() { return productos; }

	void vaciarCarrito() {
		while (!productos->esVacia()) {
			productos->eliminaInicial();
		}
		numeroProductos = 0;
		precioDelCarrito = 0;
	}

	void listarCarrito() {
		int nProductos = productos->longitud();
		if (nProductos == 0) {
			cout << "El carrito esta vacio.\n";
			system("pause > nul");
			return;
		}

		cout << "\n--- CARRITO DE COMPRAS ---\n";
		cout << "Total de items distintos: " << nProductos << "\n";

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

		//producto->setStock(producto->getStock() - cantidadSolicitada);
		NProductos* solicitudPodructo = new NProductos{ producto, cantidadSolicitada };

		productos->agregaInicial(solicitudPodructo);
		numeroProductos += cantidadSolicitada;
		cout << "Producto agregado correctamten.";
		
	}

	void borrarProducto(int id, int cantidad) {


		int nProductos = productos->longitud();
		NProductos* aux = productos->obtenerInicial();
		for (int i = 0; i < nProductos; i++) {

			if (aux->producto->getId() == id) {

				if (aux->cantidad == cantidad) {
					productos->eliminaPos(i);
					cout << "Producto eliminado Correctamente.";
				}

				if (aux->cantidad > cantidad) {
					aux->cantidad -= cantidad;
					cout << "Se eliminaron " << cantidad << " " << aux->producto->getNombre();
				}
				if(aux->cantidad<cantidad) {
					cout << "La cantidad de productos a eliminar no valida.";
				}

				return;

			}
			else {
				cout << "Elemento con el id: " << id << " no encontrado.";
				return;
			}
			aux = productos->obtenerPos(i + 1);

		}

		numeroProductos -= cantidad;
		
	}

};