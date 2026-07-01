#pragma once
#include <iostream>
#include "Comprobante.h"
#include "gotoxy.h"
using namespace std;

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
	Pago(Comprobante*comprobante, MetodoPago metodoPago,double monto)
	{
		this->comprobante = comprobante;
		this->metodoDePago = metodoPago;
		this->monto = monto;
	}

	~Pago()
	{
	}

	void realizarPago(MetodoPago nuevoMetodoPago, double montoAPagar) {
		this->metodoDePago = nuevoMetodoPago;
		this->monto = montoAPagar;

		cout << "\nProcesando pago de S/." << this->monto << "..." << endl;
		cout << "Pago realizado con exito usando: ";
		if (this->metodoDePago == TARJETA) {
			cout << "Tarjeta de Credito/Debito." << endl;
		}
		else {
			cout << "Tarjeta de Regalo." << endl;
		}
		verPago();
	}

	void verPago() {
		cout << "\n--- Detalle del Pago ---" << endl;
		cout << "Metodo: " << (metodoDePago == TARJETA ? "Tarjeta" : "Tarjeta de Regalo") << endl;
		cout << "Monto pagado: S/." << monto << endl;

		if (comprobante != nullptr) {
			cout << "Generando comprobante..." << endl;
			comprobante->imprimirComprobanteSegunTipo();
		}
		else {
			cout << "No hay comprobante asociado a este pago." << endl;
		}
	}
	

};
