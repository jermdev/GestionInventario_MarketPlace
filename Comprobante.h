#pragma once
#include<iostream>
using namespace std;

enum TipoComprobante {
	BOLETA,
	FACTURA
};
class Comprobante {
private:
	int numCompra;
	string fechaDeCompra;
	double montoTotal;
	TipoComprobante tipoComprobante;
public:
	Comprobante(int numCompra, string fechaDeCompra, double montoTotal, TipoComprobante tipoComprobante)
	{
		this->numCompra = numCompra;
		this->fechaDeCompra = fechaDeCompra;
		this->montoTotal = montoTotal;
		this->tipoComprobante = tipoComprobante;
	}

	~Comprobante()
	{
	}

	void imprimirComprobanteSegunTipo() {
		cout << "\n===============================" << endl;
		if (tipoComprobante == BOLETA) {
			cout << "          BOLETA DE VENTA      " << endl;
		}
		else if (tipoComprobante == FACTURA) {
			cout << "             FACTURA           " << endl;
		}
		cout << "===============================" << endl;
		cout << "Nro Compra: " << numCompra << endl;
		cout << "Fecha: " << fechaDeCompra << endl;
		cout << "Monto Total: S/." << montoTotal << endl;
		cout << "===============================\n" << endl;
	}
};

