#pragma once
#include <iostream>
#include "AdministradorService.h"
#include "GridView.h"
#include "ComponenteUI.h"
#include "gotoxy.h"
#include "MouseMenu.h"
using namespace std;

class AdministradorUI {

    static string truncate(const string& text, size_t maxLen) {
        if (text.length() <= maxLen) return text;
        return text.substr(0, maxLen - 3) + "...";
    }

    static string padRight(const string& text, size_t width) {
        if (text.length() >= width) return text.substr(0, width);
        return text + string(width - text.length(), ' ');
    }

    static string padLeft(const string& text, size_t width) {
        if (text.length() >= width) return text.substr(0, width);
        return string(width - text.length(), ' ') + text;
    }

    static string fmtPrecio(double precio) {
        int ent = (int)precio;
        int dec = (int)((precio - ent) * 100 + 0.5);
        return to_string(ent) + "." + (dec < 10 ? "0" : "") + to_string(dec);
    }

    static string estadoStr(EstadoPedido e) {
        switch (e) {
        case ENTREGADO: return "Entregado";
        case PENDIENTEDEENTREGA: return "Pendiente de Entrega";
        case CANCELADO: return "Cancelado";
        default: return "Desconocido";
        }
    }

    static void mostrarInfoPagina(int paginaActual, int totalPages, int x, int infoY) {
        gotoXY(x, infoY);
        cout << string(120, ' ');
        gotoXY(x, infoY);
        cout << "Pagina " << (paginaActual + 1) << " de " << totalPages
             << " | Flechas arriba/abajo para navegar | ESC para salir";
    }

    // ========== PRODUCTOS ==========

    static void menuVerProductos(AdministradorService* svc) {
        system("cls");
        cout << "\033[?25l";

        Lista<Producto*>* productos = svc->obtenerTodosLosProductos();

        if (productos == nullptr || productos->esVacia()) {
            gotoXY(60 - 14, 14);
            cout << "No hay productos registrados.";
            cout << "\033[?25h";
            system("pause");
            delete productos;
            return;
        }

        Lista<ComponenteUI<Producto*>*>* cards = new Lista<ComponenteUI<Producto*>*>();
        for (int i = 0; i < productos->longitud(); i++)
            cards->agregaFinal(new ComponenteUI<Producto*>(productos->obtenerPos(i)));

        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
        int ancho = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        int alto  = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

        const int x = 2, y = 3;
        const int cellWidth = 22, cellHeight = 7;
        const int gridAncho = ancho - 4;
        const int gridAlto  = alto - 8;

        GridView<Producto*>* grid = new GridView<Producto*>(
            cards, x, y, gridAncho, gridAlto, cellWidth, cellHeight, 1, 1);

        auto dibujado = [cellWidth](Producto* p, auto posicion) {
            int line = 0;
            posicion(line++); cout << string(cellWidth, '-');
            posicion(line++); cout << truncate(p->getNombre(), cellWidth - 2);
            posicion(line++); cout << "Precio: " << fmtPrecio(p->getPrecio());
            posicion(line++); cout << truncate(p->getCategoria(), cellWidth - 2);
            posicion(line++); cout << "ID: " << p->getId();
            posicion(line++); cout << "Stock: " << p->getStock();
            posicion(line++); cout << string(cellWidth, '-');
        };

        int infoY = y + gridAlto + 1;
        bool salir = false;

        system("cls");
        gotoXY(x, 1);
        cout << "====== VER PRODUCTOS (Grid) ======";
        grid->mostrarGrid(dibujado);
        mostrarInfoPagina(grid->getPaginaActual(), grid->getTotalPages(), x, infoY);

        while (!salir) {
            if (_kbhit()) {
                int tecla = _getch();

                if (tecla == 27 || tecla == 'q' || tecla == 'Q') {
                    salir = true;
                    break;
                }
                if (tecla == 224 || tecla == 0) {
                    int arrow = _getch();
                    if (arrow == 80)      grid->siguientePagina();
                    else if (arrow == 72) grid->paginaAnterior();
                }

                system("cls");
                gotoXY(x, 1);
                cout << "====== VER PRODUCTOS (Grid) ======";
                grid->mostrarGrid(dibujado);
                mostrarInfoPagina(grid->getPaginaActual(), grid->getTotalPages(), x, infoY);
            }
        }

        cout << "\033[?25h";
        delete grid;
        delete cards;
        delete productos;
    }

    static void menuAgregarProducto(AdministradorService* svc) {
        system("cls");
        cout << "\033[?25h";

        const int izqX = 45, inputX = 60;

        gotoXY(60 - 15, 4);
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

        gotoXY(inputX, 8);  if (!leerCampo(nombre))     return;
        gotoXY(inputX, 10); if (!leerCampo(categoria))  return;
        gotoXY(inputX, 12); if (!leerDecimal(precio))   return;
        gotoXY(inputX, 14); if (!leerEntero(stock))     return;

        svc->agregarProductoGlobal(nombre, categoria, precio, stock);

        system("cls");
        gotoXY(60 - 15, 14);
        cout << "Producto agregado exitosamente.";
        system("pause");
    }

    static void menuEditarProducto(AdministradorService* svc) {
        system("cls");
        cout << "\033[?25h";

        const int izqX = 45, inputX = 60;

        gotoXY(60 - 14, 4);
        cout << "====== EDITAR PRODUCTO ======";

        gotoXY(izqX, 8);
        cout << "ID Producto : ";
        gotoXY(izqX, 10);
        cout << "Nuevo Nombre: ";
        gotoXY(izqX, 12);
        cout << "Categoria   : ";
        gotoXY(izqX, 14);
        cout << "Precio      : ";
        gotoXY(izqX, 16);
        cout << "Stock       : ";

        gotoXY(60 - 10, 21);
        cout << "[ ESC para cancelar ]";

        int id = 0, stock = 0;
        string nombre, categoria;
        double precio = 0;

        gotoXY(inputX, 8);
        if (!leerEntero(id)) return;
        gotoXY(inputX, 10);
        if (!leerCampo(nombre)) return;
        gotoXY(inputX, 12);
        if (!leerCampo(categoria)) return;
        gotoXY(inputX, 14);
        if (!leerDecimal(precio)) return;
        gotoXY(inputX, 16);
        if (!leerEntero(stock)) return;

        svc->editarProductoGlobal(id, nombre, categoria, precio, stock);

        system("cls");
        gotoXY(60 - 11, 14);
        cout << "Producto actualizado.";
        system("pause");
    }

    static void menuBorrarProducto(AdministradorService* svc) {
        system("cls");
        cout << "\033[?25h";

        const int izqX = 45, inputX = 60;

        gotoXY(60 - 14, 6);
        cout << "====== BORRAR PRODUCTO ======";

        gotoXY(izqX, 10);
        cout << "ID Producto : ";

        gotoXY(60 - 10, 15);
        cout << "[ ESC para cancelar ]";

        int id = 0;
        gotoXY(inputX, 10);
        if (!leerEntero(id)) return;

        svc->eliminarProductoGlobal(id);

        system("cls");
        gotoXY(60 - 9, 14);
        cout << "Producto eliminado.";
        system("pause");
    }

    // ========== USUARIOS ==========

    static void menuVerUsuarios(AdministradorService* svc) {
        system("cls");
        cout << "\033[?25l";

        Lista<Usuario*>* usuarios = svc->obtenerUsuarios();
        int n = usuarios ? usuarios->longitud() : 0;

        const int x = 22;

        gotoXY(60 - 11, 1);
        cout << "====== USUARIOS ======";

        gotoXY(x, 3);
        cout << padRight("ID", 5) << " | "
             << padRight("Nombre", 20) << " | "
             << padRight("Correo", 28) << " | "
             << "Tipo";

        gotoXY(x, 4);
        cout << string(72, '-');

        for (int i = 0; i < n; i++) {
            Usuario* u = usuarios->obtenerPos(i);
            gotoXY(x, 5 + i);
            cout << padLeft(to_string(u->getId()), 5)             << " | "
                 << padRight(truncate(u->getNombre(), 20), 20)    << " | "
                 << padRight(truncate(u->getCorreo(), 28), 28)    << " | "
                 << u->getTipoUsuario();
        }

        if (n == 0) {
            gotoXY(60 - 13, 5);
            cout << "No hay usuarios registrados.";
        }

        cout << "\033[?25h";
        gotoXY(x, n + 7);
        cout << flush;
        system("pause");
        delete usuarios;
    }

    static void menuEditarUsuario(AdministradorService* svc) {
        system("cls");
        cout << "\033[?25h";

        const int izqX = 45, inputX = 60;

        gotoXY(60 - 13, 4);
        cout << "====== EDITAR USUARIO ======";

        gotoXY(izqX, 8);  cout << "Correo      : ";
        gotoXY(izqX, 10); cout << "Nuevo Nombre: ";
        gotoXY(izqX, 12); cout << "Direccion   : ";
        gotoXY(izqX, 14); cout << "Contrasena  : ";

        gotoXY(60 - 10, 19);
        cout << "[ ESC para cancelar ]";

        string correo;
        gotoXY(inputX, 8);
        if (!leerCampo(correo)) return;

        Usuario* u = svc->obtenerUsuarioParaEditar(correo);
        if (u == nullptr) {
            system("cls");
            gotoXY(60 - 11, 14);
            cout << "Usuario no encontrado.";
            system("pause");
            return;
        }

        string nNombre, nDir, nPass;

        gotoXY(inputX, 10); if (!leerCampo(nNombre))      return;
        gotoXY(inputX, 12); if (!leerCampo(nDir))         return;
        gotoXY(inputX, 14); if (!leerCampo(nPass, true))  return;

        u->setNombre(nNombre);
        u->setDireccion(nDir);
        u->setContrasenia(nPass);
        svc->modificarUsuario(u);

        system("cls");
        gotoXY(60 - 11, 14);
        cout << "Usuario actualizado.";
        system("pause");
    }

    static void menuBorrarUsuario(AdministradorService* svc) {
        system("cls");
        cout << "\033[?25h";

        const int izqX = 45, inputX = 60;

        gotoXY(60 - 13, 6);
        cout << "====== BORRAR USUARIO ======";

        gotoXY(izqX, 10);
        cout << "Correo      : ";

        gotoXY(60 - 10, 15);
        cout << "[ ESC para cancelar ]";

        string correo;
        gotoXY(inputX, 10);
        if (!leerCampo(correo)) return;

        svc->eliminarUsuario(correo);

        system("cls");
        gotoXY(60 - 9, 14);
        cout << "Usuario eliminado.";
        system("pause");
    }

    // ========== PEDIDOS ==========

    static void menuVerPedidos(AdministradorService* svc) {
        system("cls");
        cout << "\033[?25l";

        Lista<Pedido*>* pedidos = svc->obtenerPedidos();
        int n = pedidos ? pedidos->longitud() : 0;

        const int x = 25;

        gotoXY(60 - 10, 1);
        cout << "====== PEDIDOS ======";

        gotoXY(x, 3);
        cout << padRight("ID Ped", 7) << " | "
             << padRight("Cliente", 9) << " | "
             << padRight("Estado", 22) << " | "
             << padRight("Fecha", 15) << " | "
             << "Items";

        gotoXY(x, 4);
        cout << string(70, '-');

        for (int i = 0; i < n; i++) {
            Pedido* p = pedidos->obtenerPos(i);
            int items = p->getProductosComprados() ? p->getProductosComprados()->longitud() : 0;
            gotoXY(x, 5 + i);
            cout << padLeft(to_string(p->getIdPedido()), 7)          << " | "
                 << padLeft(to_string(p->getIdCliente()), 9)          << " | "
                 << padRight(estadoStr(p->getEstadoPedido()), 22)     << " | "
                 << padRight(p->getFechaEntrega(), 15)                << " | "
                 << padLeft(to_string(items), 5);
        }

        if (n == 0) {
            gotoXY(60 - 13, 5);
            cout << "No hay pedidos registrados.";
        }

        cout << "\033[?25h";
        gotoXY(x, n + 7);
        cout << flush;
        system("pause");
    }

public:
    static void Render(Administrador* admin) {
        AdministradorService* uService = new AdministradorService();
        uService->inicializarTodosLosProductos();

        string opciones[] = {
            "[  Ver Productos   ]",
            "[ Agregar Producto ]",
            "[ Editar Producto  ]",
            "[ Borrar Producto  ]",
            "[  Ver Usuarios    ]",
            "[ Editar Usuario   ]",
            "[ Borrar Usuario   ]",
            "[   Ver Pedidos    ]",
            "[ Editar Pedidos   ]",
            "[ Borrar Pedidos   ]",
            "[  Cerrar Sesion   ]"
        };
        const int totalOpciones = 11;

        while (true) {
            system("cls");
            cout << "\033[?25l";

            gotoXY(60 - 16, 4);
            cout << "====== Bienvenido, "<< admin->getNombre() << " ======";

            int sel = menuConMouse(opciones, totalOpciones, 60, 7);
            cout << "\033[?25h";

            switch (sel) {
            case 0:  menuVerProductos(uService);break;
            case 1:  menuAgregarProducto(uService);break;
            case 2:  menuEditarProducto(uService); break;
            case 3:  menuBorrarProducto(uService); break;
            case 4:  menuVerUsuarios(uService); break;
            case 5:  menuEditarUsuario(uService); break;
            case 6:  menuBorrarUsuario(uService); break;
            case 7:  menuVerPedidos(uService); break;
            case 8:
            case 9:
                system("cls");
                gotoXY(60 - 10, 14);
                cout << "Opcion en desarrollo.";
                system("pause");
                break;
            case 10:
                delete uService;
                return;
            }
        }
    }
};
