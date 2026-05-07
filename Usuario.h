#pragma once
#include <iostream>
using namespace std;

class Usuario {
private:
	int id;
	string nombre;
	string correo;
	string direccion;
	string contrase�a;
public:
	Usuario() {


	}

	~Usuario()
	{
	}

	void actualizarContraseña(string newContrase�a) { this->contrase�a = contrase�a; }

	int getId() { return this->id; }
	string getNombre() { return this->nombre; }
	string getCorreo() { return this->correo; }
	string getDireccion() { return this->direccion; }
	string getContrase�a() { return this->contrase�a; }

	void setId(int id) { this->id = id; }
	void setNombre(string nombre) { this->nombre = nombre; }
	void setCorreo(string correo) { this->correo = correo; }
	void setDireccion(string direccion) { this->direccion = direccion; }
	void setContrase�a(string contrase�a) { this->contrase�a = contrase�a; }
};
