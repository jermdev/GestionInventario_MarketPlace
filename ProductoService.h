#pragma once
#include <iostream>
#include <vector>
#include "Lista.h"
#include "GestorDeArchivos.h"
#include "Producto.h"
#include "HashTabla.hpp"

using namespace std;

class ProductoService {
private:
	Lista<Producto*>* productos;
	GestorDeArchivos* gestorArchivo;
	HashTabla<int, Producto*>* tablaProductos;//Se usa para eliminar

public:
	ProductoService() {
		this->productos = new Lista<Producto*>();
		this->gestorArchivo = new GestorDeArchivos();
		this->tablaProductos = new HashTabla<int, Producto*>(128);
	}
	~ProductoService() {
		delete productos;
		delete gestorArchivo;
		delete tablaProductos; 
	}

	void agregarProducto(Producto* p, int IDVendedor) {
		productos->agregaFinal(p);
		guardarProductos();
	}

	template<class T>
	Producto* buscarProductoPorId(int indice, T condicion) {
		if (indice == productos->longitud()) return nullptr;

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

	//Ver en que lado funciona
	void mostrarInventario() {
		for (int i = 0; i < productos->longitud(); i++) {
			Producto* p = productos->obtenerPos(i);
			if (p->getStock() > 0) {
				p->MostrarProducto();
			}
		}		
	}

	void eliminarProducto(int id) {
		
		tablaProductos->eliminarValor([id](Producto* p) { return p->getId() == id; });
		
		for (int i = 0; i < productos->longitud(); i++) {
			auto productoEliminar = productos->obtenerPos(i);

			if (productoEliminar->getId() == id) {
				productos->eliminaPos(i);          
				delete productoEliminar;           
				guardarProductos();               
				cout << "Producto eliminado...." << endl;
				break;
			}
		}
	}

	void guardarProductos() {
		vector < vector<string>> lineas;
		for (int i = 0; i < productos->longitud(); i++) {
			Producto* p = productos->obtenerPos(i);
			vector<string> linea;
			linea.push_back(to_string(p->getIdVendedor()));
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
		vector<vector<string>> lineas = gestorArchivo->leerLineasString("productos.txt", ';');

		for (int i = 0; i < lineas.size(); i++) {
			if (lineas[i].size() >= 6) {

				string sIdVendedor = lineas[i][0];
				string sNombre = lineas[i][1];
				string sCategoria = lineas[i][2];
				string sPrecio = lineas[i][3];
				string sIdProducto = lineas[i][4];
				string sStock = lineas[i][5];

				auto is_number = [](const string& s) {
					if (s.empty()) return false;
					char* end = nullptr;
					strtod(s.c_str(), &end);
					return end != s.c_str() && *end == '\0';
					};

				if (!is_number(sPrecio) || !is_number(sIdProducto) || !is_number(sStock)) {
					cerr << "Linea productos.txt malformada en la linea " << i + 1 << " -> se omite." << endl;
					continue;
				}

				int idVendedor = stoi(sIdVendedor);
				string nombre = sNombre;
				string categoria = sCategoria;
				double precio = stod(sPrecio);
				int idProducto = stoi(sIdProducto);
				int stock = stoi(sStock);

				Producto* p = new Producto(nombre, categoria, precio, idProducto, idVendedor, stock);

				productos->agregaFinal(p);
				tablaProductos->insertar(idProducto, p);
			}
		}
	}

	void inicializarProductosPorIdVendedor(int idVendedorBuscado) {
		vector<vector<string>> lineas = gestorArchivo->leerLineasString("productos.txt", ';');

		for (int i = 0; i < lineas.size(); i++) {
			if (lineas[i].size() >= 6) {
				int idVendedor = stoi(lineas[i][0]);

				if (idVendedor == idVendedorBuscado) {
					string nombre = lineas[i][1];
					string categoria = lineas[i][2];
					double precio = stod(lineas[i][3]);
					int idProducto = stoi(lineas[i][4]);
					int stock = stoi(lineas[i][5]);

					Producto* p = new Producto(nombre, categoria, precio, idProducto, idVendedor, stock);

					productos->agregaFinal(p);
					tablaProductos->insertar(idProducto, p);
				}
			}
		}
	}
};