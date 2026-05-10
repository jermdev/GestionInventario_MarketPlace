#pragma once
#include <iostream>
#include "Vendedor.h"
#include "UsuarioService.h"
using namespace std;

class VendedorUI {


    static void menuMisProductos(UsuarioService* uService, Vendedor* ven) {
        int opcion,id=0;

        do {
            cout << "\n====== MIS PRODUCTOS ======\n";
            cout << "1. Ver todos mis productos\n";
            cout << "2. Buscar producto\n";
            cout << "3. Eliminar producto\n";
            cout << "0. Volver\n";
            cout << "Seleccione una opcion: ";
            cin >> opcion;

            switch (opcion) {

            case 1:
                uService->mostrarProductosPorVendedor(ven->getId());
                break;

            case 2:
                cout << "Buscando producto...\n";
                break;

            case 3:
                cout << "Ingrese el ID del producto a eliminar: " << endl;
                cin >> id;
                uService->eliminarProductosPorVendedor(ven->getId(), id);
                break;

            case 0:
                break;

            default:
                cout << "Opcion no valida.\n";
            }

        } while (opcion != 0);
    }

    static void menuAgregarProducto(UsuarioService* uService, Vendedor *ven) {
        string nombre, categoria;
        double precio;
        int stock;

        cout << "\n====== AGREGAR PRODUCTO ======\n";

        cin.ignore();

        cout << "Nombre del producto: ";
        getline(cin, nombre);

        cout << "Categoria: ";
        getline(cin, categoria);

        cout << "Precio: ";
        cin >> precio;

        cout << "Stock: ";
        cin >> stock;

        uService->agregarProducto(ven->getId(), nombre, categoria, precio, stock);

        cout << "Producto agregado exitosamente." << endl;

    }
 
    static void menuPedidosRecibidos() {
        int opcion;

        do {
            cout << "\n====== PEDIDOS RECIBIDOS ======\n";
            cout << "1. Ver todos los pedidos\n";
            cout << "2. Ver detalles de pedido\n";
            cout << "3. Confirmar pedido\n";
            cout << "4. Marcar pedido como enviado\n";
            cout << "5. Cancelar pedido\n";
            cout << "0. Volver\n";
            cout << "Seleccione una opcion: ";
            cin >> opcion;

            switch (opcion) {

            case 1:
                cout << "Mostrando pedidos...\n";
                break;

            case 2:
                cout << "Mostrando detalles del pedido...\n";
                break;

            case 3:
                cout << "Confirmando pedido...\n";
                break;

            case 4:
                cout << "Marcando pedido como enviado...\n";
                break;

            case 5:
                cout << "Cancelando pedido...\n";
                break;

            case 0:
                break;

            default:
                cout << "Opcion no valida.\n";
            }

        } while (opcion != 0);
    }


public:
    static void Render(Vendedor* ven) {
        UsuarioService* uService = new UsuarioService();
        uService->inicializarListaProductos();
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

            case 1:
                menuMisProductos(uService, ven);
                break;

            case 2:
                menuAgregarProducto(uService, ven);
                break;

            case 3:
                menuPedidosRecibidos();
                break;

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
