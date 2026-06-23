#pragma once
#include "UserBuilder.h"
#include "Administrador.h"

class AdministradorBuilder : public UsuarioBuilder {
private:
	string cargo;
public:
	AdministradorBuilder () : cargo("")
	{
	}

	~AdministradorBuilder() override {
	}

	void setCargo(string cargo) {
		this->cargo = cargo;
	}

	Usuario* build() override {
		Administrador* a = new Administrador(cargo);

		a->setId(id);
		a->setNombre(nombre);
		a->setCorreo(correo);
		a->setDireccion(direccion);
		a->setContrasenia(contrasenia);
		a->setRol(rol);
		reset();
		return a;
	}
};
