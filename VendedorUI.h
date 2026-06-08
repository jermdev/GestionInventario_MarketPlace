#pragma once
#include <iostream>
#include "Vendedor.h"
#include "VendedorService.h"
#include "Cola.h"
using namespace std;

class VendedorUI {

    static void menuMisProductos(VendedorService* uService, Vendedor* ven) {
        system("cls");
        int opcion, id = 0;

        do {
            cout << "\n====== MIS PRODUCTOS ======\n";
            cout << "1. Ver todos mis productos\n";
            cout << "2. Eliminar producto\n";
            cout << "3. Ver productos\n";
            cout << "0. Volver\n";
            cout << "Seleccione una opcion: ";
            cin >> opcion;

            switch (opcion) {

            case 1:
                uService->mostrarProductosPorVendedor(ven->getId());
                break;


            case 2:
                cout << "Ingrese el ID del producto a eliminar: " << endl;
                cin >> id;
                uService->eliminarProductosPorVendedor(ven->getId(), id);
                break;

            case 3: {
                Lista<Producto*>* misProductos = uService->obtenerListaProductosVendedor(ven->getId());

                if (misProductos->esVacia()) {
                    cout << "\nNo tienes productos registrados para mostrar.\n";
                    break;
                }
                Cola<Producto*> colaProductos;
                for (int i = 0; i < misProductos->longitud(); i++) {
                    colaProductos.enqueue(misProductos->obtenerPos(i));
                }

                cout << "\n--- Mostrando productos en Cola (FIFO) ---\n";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                while (!colaProductos.esVacia()) {
                    Producto* p = colaProductos.dequeue();
                    p->MostrarProducto();

                    if (!colaProductos.esVacia()) {
                        cout << "\nPresiona ENTER para desencolar y ver el siguiente producto...";
                        cin.get();
                    }
                    else {
                        cout << "\n�Has llegado al final de la cola!\n";
                    }
                }
                break;
            }

            case 0:
                system("cls");

                break;

            default:
                cout << "Opcion no valida.\n";
            }

        } while (opcion != 0);
    }
    
    static void menuAgregarProducto(VendedorService* uService, Vendedor *ven) {
        system("cls");
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

        system("cls");
        cout << "Producto agregado exitosamente." << endl;

    }
 


public:
    static void Render(Vendedor* ven) {
        VendedorService* uService = new VendedorService();
        uService->inicializarListaProductos();
        int opcion;
        do {
            cout << "\n====== MENU VENDEDOR ======\n";
            cout << "1. Ver mis productos\n";
            cout << "2. Agregar producto\n";
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
