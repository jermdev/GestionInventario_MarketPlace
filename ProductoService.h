#pragma once
#include<iostream>
#include<vector>
#include"Lista.h"
#include"GestorDeArchivos.h"
#include"Producto.h"
using namespace std;

class ProductoService {
private:
	
	Lista<Producto*>* productos;
	GestorDeArchivos* gestorArchivo;

public:
	ProductoService() {
		this->productos = new Lista<Producto*>();
		this->gestorArchivo = new GestorDeArchivos();
	}
	~ProductoService() {
		delete productos;
		delete gestorArchivo;
	}

	void agregarProducto(Producto* p, int IDVendedor) {

		productos->agregaFinal(p);
		guardarProductos();
	}
	template<class T>

	Producto* buscarProductoPorId(int indice, T condicion) {

		if (indice == productos->longitud())return nullptr;

		auto productoEncontrado = productos->obtenerPos(indice);

		if (condicion(productoEncontrado)) {
			return productoEncontrado;
		}

		return buscarProductoPorId(indice + 1, condicion);
	}

	template<class T>
	Lista<Producto*>* obtenerPorductosPorCondicion(int indice, T condicion) {

		if (indice == productos->longitud()) {
			return new Lista<Producto*>();
		}

		Lista<Producto*>* resultado = obtenerPorductosPorCondicion(indice + 1, condicion);


		Producto* p = productos->obtenerPos(indice);
		if (condicion(p)) {
			resultado->agregaInicial(p);
		}

		return resultado;
	}

	void listarProductoPorId(int id) {
		Producto* productoVerificado = buscarProductoPorId(0, [id](Producto* p) {
			return p->getId() == id;
			});
		if (productoVerificado != nullptr && productoVerificado->getStock() > 0) {
			productoVerificado->MostrarProducto();
		}
		else {
			cout << "Producto con id: " << id << " no encontrado" << endl;
		}
	}

	void mostrarInventario() {
		for (uint i = 0; i < productos->longitud(); i++) {
			Producto* p = productos->obtenerPos(i);
			if (p->getStock() > 0) {
				p->MostrarProducto();
			}
		}
	}

	void eliminarProducto(int id) {


		for (int i = 0; i < productos->longitud(); i++) {

			auto productoEliminar = productos->obtenerPos(i);

			if (productoEliminar->getId() == id) {
				productos->eliminaPos(i);
				guardarProductos();
				cout << "Producto eliminado...."<<endl;
				break;
			}
		}
	}

	void guardarProductos() {

		vector < vector<string>>lineas;
		
		for (int i = 0; i < productos->longitud(); i++) {
			Producto* p = productos->obtenerPos(i);
			vector<string>linea;
			linea.push_back(to_string(p->getIdVendedor())); // agregamos el id del vendedor al inicio
			linea.push_back(p->getNombre());
			linea.push_back(p->getCategoria());
			linea.push_back(to_string(p->getPrecio()));
			linea.push_back(to_string(p->getId()));
			linea.push_back(to_string(p->getStock()));
		lineas.push_back(linea);
		}
		gestorArchivo->guardarLineas("productos.txt", lineas, ';');

	}

	void iniciaizarProductos() {

		vector<vector<string>> lineas =
			gestorArchivo->leerLineasString("productos.txt", ';');

		for (int i = 0; i < lineas.size(); i++) {

			// Ahora son 6 campos
			if (lineas[i].size() >= 6) {

				int idVendedor = stoi(lineas[i][0]);
				string nombre = lineas[i][1];
				string categoria = lineas[i][2];
				double precio = stod(lineas[i][3]);
				int idProducto = stoi(lineas[i][4]);
				int stock = stoi(lineas[i][5]);

				// Inicializar incluyendo idVendedor
				Producto* p = new Producto(
					nombre,
					categoria,
					precio,
					idProducto,
					idVendedor,
					stock
				);

				productos->agregaFinal(p);
			}
		}

	}

	void inicializarProductosPorIdVendedor(int idVendedorBuscado) {

		vector<vector<string>> lineas =
			gestorArchivo->leerLineasString("productos.txt", ';');

		for (int i = 0; i < lineas.size(); i++) {

			if (lineas[i].size() >= 6) {

				int idVendedor = stoi(lineas[i][0]);

				// Verificar si pertenece al vendedor buscado
				if (idVendedor == idVendedorBuscado) {

					string nombre = lineas[i][1];
					string categoria = lineas[i][2];
					double precio = stod(lineas[i][3]);
					int idProducto = stoi(lineas[i][4]);
					int stock = stoi(lineas[i][5]);

					Producto* p = new Producto(nombre, categoria, precio, idProducto, idVendedor, stock);

					productos->agregaFinal(p);
				}
			}
		}
	}


};