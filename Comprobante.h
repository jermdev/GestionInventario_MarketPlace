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
	string extra;

public:
	Comprobante(int numCompra, string fechaDeCompra, double montoTotal, TipoComprobante tipoComprobante,string extra)
	{
		this->numCompra = numCompra;
		this->fechaDeCompra = fechaDeCompra;
		this->montoTotal = montoTotal;
		this->tipoComprobante = tipoComprobante;
		this->extra = extra;
	}

	~Comprobante()
	{
	}

	void imprimirComprobanteSegunTipo() {
		cout << "\n===============================" << endl;

		double igv = montoTotal * 0.18;
		double totalConIgv = montoTotal + igv;

		if (tipoComprobante == BOLETA) {
			cout << "          BOLETA DE VENTA      " << endl;
			cout << "===============================" << endl;
			cout << "Nro Dni: " << extra << endl;
			cout << "Nro Compra: " << numCompra << endl;
			cout << "Fecha: " << fechaDeCompra << endl;
			cout << "Monto (sin IGV): S/." << montoTotal << endl;
			cout << "IGV (18%): S/." << igv << endl;
			cout << "Monto Total (con IGV): S/." << totalConIgv << endl;
			cout << "===============================\n" << endl;
		}
		else if (tipoComprobante == FACTURA) {
			cout << "             FACTURA           " << endl;
			cout << "===============================" << endl;
			cout << "Nro RUC: " << extra << endl;
			cout << "Nro Compra: " << numCompra << endl;
			cout << "Fecha: " << fechaDeCompra << endl;
			cout << "Monto (sin IGV): S/." << montoTotal << endl;
			cout << "IGV (18%): S/." << igv << endl;
			cout << "Monto Total (con IGV): S/." << totalConIgv << endl;
			cout << "===============================\n" << endl;
		}
	
	}
};

