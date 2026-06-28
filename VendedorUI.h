#pragma once
#include <iostream>
#include "Vendedor.h"
#include "VendedorService.h"
#include "Cola.h"
#include "gotoxy.h"
#include "MouseMenu.h"
using namespace std;

class VendedorUI {

    static void menuMisProductos(VendedorService* uService, Vendedor* ven) {
        string opciones[] = {
            "[ Ver todos mis productos ]",
            "[   Eliminar producto     ]",
            "[    Ver productos        ]",
            "[        Volver           ]"
        };
        const int total = 4;

        while (true) {
            system("cls");
            cout << "\033[?25l";

            gotoXY(60 - 13, 8);
            cout << "====== MIS PRODUCTOS ======";

            int sel = menuConMouse(opciones, total, 60, 11);
            cout << "\033[?25h";

            switch (sel) {
            case 0:
                system("cls");
                uService->mostrarProductosPorVendedor(ven->getId());

                break;

            case 1: {
                system("cls");
                int id = 0;
                cout << "Ingrese el ID del producto a eliminar: " << endl;
                cin >> id;
                uService->eliminarProductosPorVendedor(ven->getId(), id);
                break;
            }

            case 2: {
                system("cls");
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
                while (cin.get() != '\n') {}

                while (!colaProductos.esVacia()) {
                    Producto* p = colaProductos.dequeue();
                    p->MostrarProducto();

                    if (!colaProductos.esVacia()) {
                        cout << "\nPresiona ENTER para desencolar y ver el siguiente producto...";
                        cin.get();
                    }
                    else {
                        cout << "\n¡Has llegado al final de la cola!\n";
                        system("pause>0");
                    }

                }
                break;
            }

            case 3:
                return;
            }
        }
    }

    static void menuAgregarProducto(VendedorService* uService, Vendedor* ven) {
        system("cls");
        cout << "\033[?25h"; // mostrar cursor para escribir

        const int izqX  = 45;
        const int inputX = 59;

        gotoXY(60 - 14, 4);
        cout << "====== AGREGAR PRODUCTO ======";

        gotoXY(izqX, 8);  cout << "Nombre      : ";
        gotoXY(izqX, 10); cout << "Categoria   : ";
        gotoXY(izqX, 12); cout << "Precio      : ";
        gotoXY(izqX, 14); cout << "Stock       : ";

        gotoXY(60 - 10, 19);
        cout << "[ ESC para cancelar ]";

        string nombre, categoria;
        double precio = 0;
        int stock = 0;

        gotoXY(inputX, 8);
        if (!leerCampo(nombre)) return;

        gotoXY(inputX, 10);
        if (!leerCampo(categoria)) return;

        gotoXY(inputX, 12);
        if (!leerDecimal(precio)) return;

        gotoXY(inputX, 14);
        if (!leerEntero(stock)) return;

        uService->agregarProducto(ven->getId(), nombre, categoria, precio, stock);

        system("cls");
        gotoXY(60 - 17, 14);
        cout << "Producto agregado exitosamente.";
        system("pause");
    }

public:
    static void Render(Vendedor* ven) {
        VendedorService* uService = new VendedorService();
        uService->inicializarListaProductos();

        string opciones[] = {
            "[ Ver mis productos ]",
            "[ Agregar producto  ]",
            "[ Cerrar Sesion     ]"
        };
        const int totalOpciones = 3;

        while (true) {
            system("cls");
            cout << "\033[?25l";

            gotoXY(60 - 14, 10);
            cout << "====== MENU VENDEDOR ======";

            int sel = menuConMouse(opciones, totalOpciones, 60, 13);
            cout << "\033[?25h";

            switch (sel) {
            case 0:
                menuMisProductos(uService, ven);
                break;

            case 1:
                menuAgregarProducto(uService, ven);
                break;

            case 2:
                cout << "Cerrando sesion...\n";
                return;
            }
        }
    }
};
