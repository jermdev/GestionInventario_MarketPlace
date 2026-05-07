#pragma once
#include "Comprobante.h"
enum MetodoPago {
	TARJETADEREGALO,
	TARJETA
};

class Pago {
private:
	Comprobante* comprobante;
	MetodoPago metodoDePago;
	double monto;
public:
	Pago()
	{
	}

	~Pago()
	{
	}


};
