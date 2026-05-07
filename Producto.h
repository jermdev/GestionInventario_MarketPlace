#pragma once
#include <iostream>
using namespace std;

class Producto {
private:
	string nombre;
	string categoria;
	double precio;
	int id;
	int stock;
public:
	Producto(string nombre, string categoria, double precio, int id, int stock) {
		this->id = id;
		this->categoria = categoria;
		this->nombre = nombre;
		this->precio = precio;
		this->stock = stock;
	}

	~Producto()
	{
	}

	string getNombre() { return nombre; };
	string getCategoria() { return categoria; };
	double getPrecio() { return precio; }
	int getId() { return id; }
	int getStock() { return stock; }


	void setNombre(string nombre) { this->nombre = nombre; }
	void setCategoria(string categoria) { this->categoria = categoria; }
	void setPrecio(double precio) { this->precio = precio; }
	void setStock(int stock) { this->stock = stock; }

	void MostrarProducto() {
		cout << "\n**********************" << endl;
		cout << "Nombre: " << nombre << endl;
		cout << "Categoria: " << categoria << endl;
		cout << "Precio: " << precio << endl;
		cout << "ID: " << id << endl;
		cout << "**********************" << endl;

	}
};
