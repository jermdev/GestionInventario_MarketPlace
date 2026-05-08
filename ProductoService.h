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

	void agregarProducto(Producto* p) {

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

	void listarProductoPorId(int id) {
		Producto* productoVerificado = buscarProductoPorId(0, [id](Producto* p) {
			return p->getId() == id;
			});
		if (productoVerificado != nullptr) {
			productoVerificado->MostrarProducto();
		}
		else {
			cout << "Producto con id: " << id << " no encontrado" << endl;
		}
	}

	void mostrarInventario() {
		for (uint i = 0; i < productos->longitud(); i++) {
			productos->obtenerPos(i)->MostrarProducto();
		}
	}

	void guardarProductos() {

		vector < vector<string>>lineas;
		
		for (int i = 0; i < productos->longitud(); i++) {
			Producto* p = productos->obtenerPos(i);
			vector<string>linea;
			linea.push_back(p->getNombre());
			linea.push_back(p->getCategoria());
			linea.push_back(to_string(p->getPrecio()));
			linea.push_back(to_string(p->getId()));
			linea.push_back(to_string(p->getStock()));
		lineas.push_back(linea);
		}
		gestorArchivo->guardarLineas("productos.txt", lineas, ';');

	}

	void obtenerProductos() {

		vector<vector<string>>lineas = gestorArchivo->leerLineasString("productos.txt",';');

		for (int i = 0; i < lineas.size(); i++) {
			if (lineas[i].size() >= 5) {

				string nombre = lineas[i][0];
				string categoria = lineas[i][1];
				double precio = stod(lineas[i][2]);
				int id = stoi(lineas[i][3]);
				int stock = stoi(lineas[i][4]);

				Producto* p = new Producto(nombre, categoria, precio, id, stock);
				productos->agregaFinal(p);

			}

		}


	}


};