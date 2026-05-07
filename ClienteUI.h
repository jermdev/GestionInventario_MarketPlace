#pragma once
#include <iostream>
using namespace std;

class ClienteUI {
public:
    static void Render() {
        int opcion;
        do {
            cout << "\n====== MENU CLIENTE ======\n";
            cout << "1. Ver Productos\n";
            cout << "2. Buscar Producto\n";
            cout << "3. Ver Carrito\n";
            cout << "4. Realizar Compra\n";
            cout << "5. Ver Historial de Pedidos\n";
            cout << "0. Cerrar Sesion\n";
            cout << "Seleccione una opcion: ";
            cin >> opcion;

            switch (opcion) {
            case 1: cout << "Ver productos...\n"; break;
            case 2: cout << "Buscar producto...\n"; break;
            case 3: cout << "Ver carrito...\n"; break;
            case 4: cout << "Realizar compra...\n"; break;
            case 5: cout << "Historial de pedidos...\n"; break;
            case 0: break;
            default: cout << "Opcion no valida.\n"; break;
            }
        } while (opcion != 0);
    }
};
