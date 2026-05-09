#pragma once
#include "Cliente.h"
#include "UsuarioService.h"
#include <iostream>
using namespace std;

class ClienteUI {


    static void menuVerProductos() {
        int opcion;

        do {
            cout << "\n====== VER PRODUCTOS ======\n";
            cout << "1. Mostrar todos los productos\n";
            cout << "2. Filtrar por categoria\n";
            cout << "3. Ordenar por precio\n";
            cout << "4. Ver detalles de producto\n";
            cout << "0. Volver\n";
            cout << "Seleccione una opcion: ";
            cin >> opcion;

            switch (opcion) {
            case 1:
                cout << "Mostrando todos los productos...\n";
                break;

            case 2:
                cout << "Filtrando productos por categoria...\n";
                break;

            case 3:
                cout << "Ordenando productos por precio...\n";
                break;

            case 4:
                cout << "Mostrando detalles del producto...\n";
                break;

            case 0:
                break;

            default:
                cout << "Opcion no valida.\n";
            }

        } while (opcion != 0);
    }

    static void menuBuscarProducto() {
        int opcion;

        do {
            cout << "\n====== BUSCAR PRODUCTO ======\n";
            cout << "1. Buscar por nombre\n";
            cout << "2. Buscar por categoria\n";
            cout << "3. Buscar por rango de precio\n";
            cout << "0. Volver\n";
            cout << "Seleccione una opcion: ";
            cin >> opcion;

            switch (opcion) {
            case 1:
                cout << "Buscando producto por nombre...\n";
                break;

            case 2:
                cout << "Buscando producto por categoria...\n";
                break;

            case 3:
                cout << "Buscando producto por rango de precio...\n";
                break;

            case 0:
                break;

            default:
                cout << "Opcion no valida.\n";
            }

        } while (opcion != 0);
    }

    static void menuCarrito() {
        int opcion;

        do {
            cout << "\n====== CARRITO ======\n";
            cout << "1. Ver productos del carrito\n";
            cout << "2. Agregar producto\n";
            cout << "3. Eliminar producto\n";
            cout << "4. Vaciar carrito\n";
            cout << "0. Volver\n";
            cout << "Seleccione una opcion: ";
            cin >> opcion;

            switch (opcion) {
            case 1:
                cout << "Mostrando carrito...\n";
                break;

            case 2:
                cout << "Agregando producto al carrito...\n";
                break;

            case 3:
                cout << "Eliminando producto del carrito...\n";
                break;

            case 4:
                cout << "Vaciando carrito...\n";
                break;

            case 0:
                break;

            default:
                cout << "Opcion no valida.\n";
            }

        } while (opcion != 0);
    }

    static void menuCompra() {
        int opcion;

        do {
            cout << "\n====== REALIZAR COMPRA ======\n";
            cout << "1. Confirmar compra\n";
            cout << "2. Seleccionar metodo de pago\n";
            cout << "3. Ver resumen de compra\n";
            cout << "0. Volver\n";
            cout << "Seleccione una opcion: ";
            cin >> opcion;

            switch (opcion) {
            case 1:
                cout << "Compra confirmada...\n";
                break;

            case 2:
                cout << "Seleccionando metodo de pago...\n";
                break;

            case 3:
                cout << "Mostrando resumen de compra...\n";
                break;

            case 0:
                break;

            default:
                cout << "Opcion no valida.\n";
            }

        } while (opcion != 0);
    }

    static void menuHistorialPedidos() {
        int opcion;

        do {
            cout << "\n====== HISTORIAL DE PEDIDOS ======\n";
            cout << "1. Ver todos los pedidos\n";
            cout << "2. Ver detalles de pedido\n";
            cout << "3. Buscar pedido por fecha\n";
            cout << "0. Volver\n";
            cout << "Seleccione una opcion: ";
            cin >> opcion;

            switch (opcion) {
            case 1:
                cout << "Mostrando historial de pedidos...\n";
                break;

            case 2:
                cout << "Mostrando detalles del pedido...\n";
                break;

            case 3:
                cout << "Buscando pedido por fecha...\n";
                break;

            case 0:
                break;

            default:
                cout << "Opcion no valida.\n";
            }

        } while (opcion != 0);
    }
    public:
    static void Render(Cliente* cli) {
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

            case 1:
                menuVerProductos();
                break;

            case 2:
                menuBuscarProducto();
                break;

            case 3:
                menuCarrito();
                break;

            case 4:
                menuCompra();
                break;

            case 5:
                menuHistorialPedidos();
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
