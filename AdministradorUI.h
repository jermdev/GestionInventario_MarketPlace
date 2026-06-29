#pragma once

#include "AdministradorService.h"
#include "gotoxy.h"
#include "MouseMenu.h"
using namespace std;

class AdministradorUI {


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
            int sel = menuConMouse(opciones, totalOpciones, 60, 9);


            switch (sel) {
            case 0:
                system("cls");
                uService->mostrarTodosLosProductos();
                break;

            case 2: { // Editar Productos
                int id, stock; string nNombre, nCat; double nPrecio;
                cout << "Ingrese el ID del producto a editar: "; cin >> id;
                cin.ignore();
                cout << "Nuevo Nombre: "; getline(cin, nNombre);
                cout << "Nueva Categoria: "; getline(cin, nCat);
                cout << "Nuevo Precio: "; cin >> nPrecio;
                cout << "Nuevo Stock: "; cin >> stock;
                uService->editarProductoGlobal(id, nNombre, nCat, nPrecio, stock);
                system("pause");
                break;
            }

            case 3: { // Borrar Productos
                int id;
                cout << "Ingrese el ID del producto a borrar: "; cin >> id;
                uService->eliminarProductoGlobal(id);
                system("pause");
                break;
            }

            case 4: // Ver Usuarios
                uService->mostrarTodosLosUsuarios();
                break;

            case 5: { // Editar Usuario
                string correo;
                cout << "Ingrese el correo del usuario a editar: "; cin >> correo;

                Usuario* u = uService->obtenerUsuarioParaEditar(correo);
                if (u != nullptr) {
                    string nNombre, nDir, nPass;
                    cin.ignore();
                    cout << "Nuevo Nombre: "; getline(cin, nNombre);
                    cout << "Nueva Direccion: "; getline(cin, nDir);
                    cout << "Nueva Contrasena: "; cin >> nPass;

                    u->setNombre(nNombre);
                    u->setDireccion(nDir);
                    u->setContrasenia(nPass);

                    uService->modificarUsuario(u);
                }
                else {
                    cout << u8"Usuario no encontrado.\n";
                }
                system("pause");
                break;
            }

            case 6: { // Borrar Usuario
                string correo;
                cout << "Ingrese el correo del usuario a borrar: "; cin >> correo;
                uService->eliminarUsuario(correo);
                system("pause");
                break;
            }

            case 7: { // Ver Pedidos
                string correo;
                cout << "Digite el correo del usuario que quiere buscar: "; cin >> correo;
                uService->verPedidos(correo);
                system("pause");
                break;
            }

            case 8:
                cout << u8"Opcion de editar pedidos (En desarrollo)...\n";
                system("pause");
                break;

            case 9:
                cout << u8"Opcion de borrar pedidos (En desarrollo)...\n";
                system("pause");
                break;

            default:
                cout << "Opcion no valida.\n";
                break;
            }
            
        }
    }
};
