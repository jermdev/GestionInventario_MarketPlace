#pragma once

#include "AdministradorService.h"
using namespace std;

class AdministradorUI {


    static void menuVerProductos(Administrador* uService) {
        int opcion;
        system("cls");
        do {
            system("cls");

            cout << "\n====== VER PRODUCTOS ======\n";
            cout << "1. Mostrar todos los productos\n";
            cout << "2. Filtrar por categoria\n";
            cout << "3. Ordenar por precio\n";
            cout << "0. Volver\n";
            cout << "Seleccione una opcion: ";
            cin >> opcion;

            

        } while (opcion != 0);
    }


    

    

public:
    static void Render(Cliente* cli) {
        AdministradorService* uService = new AdministradorService();
        uService->inicializarTodosLosProductos();

        
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

           

            case 0:
                cout << "Cerrando sesion...\n";
                break;

            default:
                cout << "Opcion no valida.\n";
                break;
            }

        } while (opcion != 0);
    }
};
