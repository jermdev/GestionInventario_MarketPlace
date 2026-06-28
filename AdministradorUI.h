#pragma once

#include "AdministradorService.h"
#include "gotoxy.h"
#include "MouseMenu.h"
using namespace std;

class AdministradorUI {

    static void menuVerProductos(Administrador* uService) {
        string opciones[] = {
            "[ Mostrar todos los productos ]",
            "[    Filtrar por categoria    ]",
            "[     Ordenar por precio      ]",
            "[           Volver            ]"
        };
        const int total = 4;

        while (true) {
            system("cls");
            cout << "\033[?25l";

            gotoXY(60 - 13, 8);
            cout << "====== VER PRODUCTOS ======";

            int sel = menuConMouse(opciones, total, 60, 11);
            cout << "\033[?25h";

            switch (sel) {
            case 0:
                // mostrar todos los productos - pendiente de implementar
                break;
            case 1:
                // filtrar por categoria - pendiente de implementar
                break;
            case 2:
                // ordenar por precio - pendiente de implementar
                break;
            case 3:
                return;
            }
        }
    }

public:
    static void Render(Administrador* admin) {
        AdministradorService* uService = new AdministradorService();
        uService->inicializarTodosLosProductos();

        string opciones[] = {
            "[   1. Ver Productos   ]",
            "[  2. Editar Productos ]",
            "[ 3. Borrar Productos  ]",
            "[    4. Ver Usuario    ]",
            "[  5. Editar Usuario   ]",
            "[  6. Borrar Usuario   ]",
            "[    7. Ver Pedidos    ]",
            "[  8. Editar Pedidos   ]",
            "[  9. Borrar Pedidos   ]",
            "[   0. Cerrar Sesion   ]"
        };
        const int totalOpciones = 10;

        while (true) {
            system("cls");
            cout << "\033[?25l";

            gotoXY(60 - 16, 6);
            cout << "====== MENU ADMINISTRADOR ======";

            int sel = menuConMouse(opciones, totalOpciones, 60, 9);
            cout << "\033[?25h";

            switch (sel) {
            case 0:
                uService->mostrarTodosLosProductos();
                break;
            case 1:
                break;
            case 2:
                break;
            case 3:
                uService->mostrarTodosLosUsuarios();
                break;
            case 4:
                break;
            case 5:
                break;
            case 6: {
                string correo;
                cout << "Dijite la categoria que quiere buscar: "; cin >> correo;
                uService->verPedidos(correo);
                break;
            }
            case 7:
                break;
            case 8:
                break;
            case 9:
                cout << "Cerrando sesion...\n";
                return;
            }
        }
    }
};
