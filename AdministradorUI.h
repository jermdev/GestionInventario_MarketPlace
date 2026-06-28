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
    static void Render(Administrador* admin) {
        AdministradorService* uService = new AdministradorService();
        uService->inicializarTodosLosProductos();

        int opcion;
        do {
            cout << "\n====== MENU ADMINISTRADOR ======\n";
            cout << "1. Ver Productos\n";
            cout << "2. Editar Productos\n";
            cout << "3. Borrar Productos\n";
            cout << "4. Ver Usuario\n";
            cout << "5. Editar Usuario\n";
            cout << "6. Borrar Usuario\n";
            cout << "7. Ver Pedidos\n";
            cout << "8. Editar Pedidos\n";
            cout << "9. Borrar Pedidos\n";
            cout << "0. Cerrar Sesion\n";
            cout << "Seleccione una opcion: ";
            cin >> opcion;

            switch (opcion) {

            case 1:
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
