#pragma once
#include <iostream>
using namespace std;

class ClienteUI
{
public:
	ClienteUI()
	{
	}

	~ClienteUI()
	{
	}

	static void Render() {
		int opcion;
		do {
			int opcion;
			cout << "\n====== MENU CLIENTE ======\n";
			cout << "1. Ver Productos\n";
			cout << "2. Buscar Producto\n";
			cout << "3. Ver Carrito\n";
			cout << "4. Realizar Compra\n";
			cout << "5. Ver Historial de Pedidos\n";
			cout << "0. Cerrar Sesion\n";
			cout << "Seleccione una opcion: ";
			cin >> opcion;
		} while (true);
	}
};
