#pragma once

#include <iostream>
using namespace std;

public class AuthUI {
public:
	AuthUI()
	{
	}

	~AuthUI()
	{
	}

	static void Render() {
		int opcion;
		do
		{
			cout << "\n====== MARKET PLACE ======\n";
			cout << "1. Iniciar Sesion\n";
			cout << "2. Registrarse\n";
			cout << "0. Salir\n";
			cout << "Seleccione una opcion: ";
			cin >> opcion;
		} while (opcion < 0 || opcion > 2);

	}

};
