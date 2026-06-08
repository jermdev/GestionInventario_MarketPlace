#pragma once
#include "Usuario.h"
string rolToString(ROL rol) {
	switch (rol)
	{
	case ROL::CLIENTE:
		return "cliente";
	case ROL::VENDEDOR:
		return "vendedor";
	case ROL::ADMINISTRADOR:
		return "administrador";
	case ROL::NONE:
		return "none";
	default:
		break;
	}
}

ROL stringToRol(string rol) {
	if (rol == "cliente") return ROL::CLIENTE;
	else if (rol == "vendedor") return ROL::VENDEDOR;
	else if (rol == "administrador") return ROL::ADMINISTRADOR;
	else if (rol == "none") return ROL::NONE;
}