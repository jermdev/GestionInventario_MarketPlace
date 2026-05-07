#pragma once
#include <iostream>
using namespace std;

class Usuario {
protected:
	int id;
	string nombre;
	string correo;
	string direccion;
	string contrasenia;
public:
	Usuario() {


	}

	~Usuario()
	{
	}

	void actualizarContrasenia(string newContrasenia) { this->contrasenia = contrasenia; }

	int getId() { return this->id; }
	string getNombre() { return this->nombre; }
	string getCorreo() { return this->correo; }
	string getDireccion() { return this->direccion; }
	string getContrasenia() { return this->contrasenia; }

	void setId(int id) { this->id = id; }
	void setNombre(string nombre) { this->nombre = nombre; }
	void setCorreo(string correo) { this->correo = correo; }
	void setDireccion(string direccion) { this->direccion = direccion; }
	void setContrasenia(string contrasenia) { this->contrasenia = contrasenia; }
};
