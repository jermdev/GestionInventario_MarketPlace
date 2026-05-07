#pragma once
#include <iostream>
using namespace std;

class VendedorUI {
public:
    static void Render() {
        int opcion;
        do {
            cout << "\n====== MENU VENDEDOR ======\n";
            cout << "1. Ver mis productos\n";
            cout << "2. Agregar producto\n";
            cout << "3. Ver pedidos recibidos\n";
            cout << "0. Cerrar Sesion\n";
            cout << "Seleccione una opcion: ";
            cin >> opcion;

            switch (opcion) {
            case 1: cout << "Ver productos...\n"; break;
            case 2: cout << "Agregar producto...\n"; break;
            case 3: cout << "Ver pedidos...\n"; break;
            case 0: break;
            default: cout << "Opcion no valida.\n"; break;
            }
        } while (opcion != 0);
    }
};
