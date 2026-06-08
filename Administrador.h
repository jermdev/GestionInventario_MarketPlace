#pragma once
#include "Usuario.h"

class Administrador : public Usuario {
private:
	string cargo;
	

public:
	Administrador(string cargo) {
		this->cargo = cargo;
	}

	Administrador() {
		this->cargo = "cargo no asignado";
	}

	~Administrador()
	{
	}
	
	string getTipoUsuario() const override { return "Administrador"; }
	int getTipoId() const override { return 3; }

	void setCargo(string cargo) { this->cargo = cargo; }
	string getCargo() { return this->cargo; }
};
