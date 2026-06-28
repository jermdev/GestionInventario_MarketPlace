#pragma once
#include <iostream>
#include "ClienteService.h"
#include "ComponenteUI.h"
#include "GridView.h"
#include "Producto.h"
#include "gotoxy.h"
#include "MouseMenu.h"

using namespace std;

class ClienteUI {

    // ========== HELPERS para formato de texto ==========

    static string truncate(const string& text, size_t maxLen) {
        if (text.length() <= maxLen) return text;
        return text.substr(0, maxLen - 3) + "...";
    }

    static string padRight(const string& text, size_t width) {
        if (text.length() >= width) return text.substr(0, width);
        return text + string(width - text.length(), ' ');
    }

    static void clearLine(int x, int y, int width) {
        gotoXY(x, y);
        cout << string(width, ' ');
        gotoXY(x, y);
    }

    // ========== MOSTRAR GRID DE PRODUCTOS CON NAVEGACIÓN ==========

    static void mostrarGridProductos(GridView<Producto*>* gridProductos, 
                                      ClienteService* uService,
                                      const int cellWidth,
                                      const int x, const int y,
                                      const int gridAlto) {

        auto dibujado = [cellWidth](Producto* p, auto posicion) {
            int line = 0;

            posicion(line++);
            cout << string(cellWidth, '-');

            posicion(line++);
            cout << truncate(p->getNombre(), cellWidth - 2);

            posicion(line++);
            cout << "Precio: " << p->getPrecio();

            posicion(line++);
            cout << truncate(p->getCategoria(), cellWidth - 2);

            posicion(line++);
            cout << "ID: " << p->getId();

            posicion(line++);
            cout << string(cellWidth, '-');
        };

        gridProductos->mostrarGrid(dibujado);
    }

    static void mostrarInfoPagina(int paginaActual, int totalPages, 
                                   int x, int infoY) {
        gotoXY(x, infoY);
        cout << string(120, ' '); // limpiar línea
        gotoXY(x, infoY);
        cout << "Pagina " << (paginaActual + 1) << " de " << totalPages 
             << " | Flechas arriba/abajo para navegar | ESC para salir";
    }

    // ========== MENÚ VER PRODUCTOS CON NAVEGACIÓN ==========

    static void menuVerProductos(ClienteService* uService) {
        string opciones[] = {
            "[ Mostrar todos los productos ]",
            "[    Filtrar por categoria    ]",
            "[     Ordenar por precio      ]",
            "[           Volver            ]"
        };
        const int total = 4;

        
        system("cls");
        cout << "\033[?25l"; // ocultar cursor

        gotoXY(60 - 13, 8);
        cout << "====== VER PRODUCTOS ======";

        int sel = menuConMouse(opciones, total, 60, 11);
        cout << "\033[?25h"; // mostrar cursor

        switch (sel) {
        case 0: // Mostrar todos los productos
        {
            system("cls");
            cout << "\033[?25l"; // ocultar cursor

            Lista<Producto*>* productos = uService->obtenerProductos();

            if (productos == nullptr || productos->esVacia()) {
                cout << "No hay productos disponibles.\n";
                system("pause");
                break;
            }

            Lista<ComponenteUI<Producto*>*>* cardProductos = 
                new Lista<ComponenteUI<Producto*>*>();

            for (int i = 0; i < productos->longitud(); i++) {
                Producto* p = productos->obtenerPos(i);
                ComponenteUI<Producto*>* card = new ComponenteUI<Producto*>(p);
                cardProductos->agregaFinal(card);
            }

            CONSOLE_SCREEN_BUFFER_INFO csbi;
            GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);

            int ancho = csbi.srWindow.Right - csbi.srWindow.Left + 1;
            int alto = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

            int x = 2;
            int y = 3;
            int gridAncho = ancho - 4;
            int gridAlto = alto - 8;

            int cellWidth = 22;
            int cellHeight = 7;

            GridView<Producto*>* gridProductos = new GridView<Producto*>(
                cardProductos, x, y, gridAncho, gridAlto, cellWidth, cellHeight, 1, 1);

            // ========== BUCLE DE NAVEGACIÓN ==========
            bool salir = false;

            
            system("cls");
            
            gotoXY(x, 1);
            cout << "====== VER PRODUCTOS (Grid) ======";


            // Dibujar grid
            mostrarGridProductos(gridProductos, uService, cellWidth, x, y, gridAlto);

            int infoY = y + gridAlto + 1;
            mostrarInfoPagina(gridProductos->getPaginaActual(),
                              gridProductos->getTotalPages(), x, infoY);

            while (!salir) {
                if (_kbhit()) {
                    int tecla = _getch();

                    // Tecla ESC (27) para salir
                    if (tecla == 27) {
                        salir = true;
                        break;
                    }

                    // Tecla 'q' o 'Q' para salir
                    if (tecla == 'q' || tecla == 'Q') {
                        salir = true;
                        break;
                    }

                    // Flechas especiales (tecla 224 = código especial)
                    if (tecla == 224 || tecla == 0) {
                        int arrowKey = _getch();

                        // Flecha abajo (80): siguiente página
                        if (arrowKey == 80) {
                            gridProductos->siguientePagina();
                        }
                        // Flecha arriba (72): página anterior
                        else if (arrowKey == 72) {
                            gridProductos->paginaAnterior();
                        }
                    }

                    system("cls");
                    gotoXY(x, 1);
                    cout << "====== VER PRODUCTOS (Grid) ======";

                    // Dibujar grid
                    mostrarGridProductos(gridProductos, uService, cellWidth, x, y, gridAlto);
;
                    mostrarInfoPagina(gridProductos->getPaginaActual(),
                        gridProductos->getTotalPages(), x, infoY);
                    
                }
            }
            cout << "\033[?25h"; // mostrar cursor
            delete gridProductos;
            delete cardProductos;
            break;
            }

            case 1: {
                system("cls");
                string categoria;
                string descripcionCampo = "Dijite la categoria que quiere buscar: ";
                gotoXY(descripcionCampo.length() - 20, 8);
                cout << descripcionCampo;
                cin >> categoria;
                uService->filtrarPorCategoria(categoria);
                break;
            }

            case 2: {
                string tipoOrden;
                string descripcionCampo = "Ordenar por mayor o menor precio (ejemplo mayor / menor): ";
                system("cls");
                gotoXY(descripcionCampo.length() - 50, 8);

                do {
                    cout << descripcionCampo;
                    cin >> tipoOrden;
                } while (tipoOrden != "mayor" && tipoOrden != "menor");

                system("cls");
                uService->mostrarListaProductosOrdenadaPorPrecio(tipoOrden);
                break;
            }

            case 3:
                return;
            
        }
    }

    // ========== RESTO DE MENÚS (sin cambios) ==========

    static void menuBuscarProducto(Cliente* cli, ClienteService* uService) {
        string opciones[] = {
            "[ Buscar por ID ]",
            "[    Volver     ]"
        };
        const int total = 2;

        while (true) {
            system("cls");
            cout << "\033[?25l";

            gotoXY(60 - 14, 10);
            cout << "====== BUSCAR PRODUCTO ======";

            int sel = menuConMouse(opciones, total, 60, 13);
            cout << "\033[?25h";

            switch (sel) {
            case 0:
            {
                int id;
                string descripcionCampo = "Dijite el id del Producto: ";
                system("cls");
                gotoXY(descripcionCampo.length() - 20, 8);
                cout << descripcionCampo; cin >> id;

                Producto* p = uService->buscarProductoPorID(id);
                if (p == nullptr) {
                    cout << "\nProducto no encontrado.\n";
                    system("pause");
                    break;
                }

                string resProducto;
                p->MostrarProducto();
                cout << "Desea comprar este producto? (y/n) :"; cin >> resProducto;

                if (resProducto == "y") {
                    int cantidad;
                    cout << "Digite cantidad: "; cin >> cantidad;
                    if (cantidad <= 0) {
                        cout << "Cantidad no valida.";
                        system("pause");
                        break;
                    }
                    cli->getCarrito()->agregarProducto(p, cantidad);
                    cout << "Producto agregado al carrito.";
                }
                break;
            }

            case 1:
                return;
            }
        }
    }

    static void menuCarrito(Cliente* cli, ClienteService* uService) {
        string opciones[] = {
            "[ Ver productos del carrito ]",
            "[     Agregar producto      ]",
            "[    Eliminar producto      ]",
            "[      Vaciar carrito       ]",
            "[          Volver           ]"
        };
        const int total = 5;

        while (true) {
            system("cls");
            cout << "\033[?25l";

            gotoXY(60 - 10, 9);
            cout << "====== CARRITO ======";

            int sel = menuConMouse(opciones, total, 60, 12);
            cout << "\033[?25h";

            int id, cant;

            switch (sel) {
            case 0:
                system("cls");
                gotoXY(60, 10);
                cli->getCarrito()->listarCarrito();
                break;

            case 1:
                system("cls");
                cout << "Ingrese el ID del producto a agregar: ";
                cin >> id;
                {
                    Producto* p = uService->buscarProductoPorID(id);
                    if (p != nullptr) {
                        cout << "Ingrese la cantidad: ";
                        cin >> cant;
                        cli->getCarrito()->agregarProducto(p, cant);
                    }
                    else {
                        cout << "Producto no encontrado en el inventario.\n";
                    }
                    system("pause > nul");
                }
                break;

            case 2:
                system("cls");
                if (cli->getCarrito()->getProductos()->esVacia()) {
                    cout << "Tu carrito esta vacio. No puedes eliminar.\n";
                    break;
                }
                cout << "Ingrese el ID del producto a eliminar: ";
                cin >> id;
                cout << "Ingrese la cantidad a eliminar: ";
                cin >> cant;
                cli->getCarrito()->borrarProducto(id, cant);
                system("pause > nul");
                break;

            case 3:
                system("cls");
                cli->getCarrito()->vaciarCarrito();
                cout << "Vaciando carrito...\n";
                break;

            case 4:
                return;

            default:
                cout << "Opcion no valida.\n";
            }
        }
    }

public:
    static void Render(Cliente* cli) {
        ClienteService* uService = new ClienteService();
        uService->inicializarListaProductos();
        uService->InicializarPedidosPorIdCliente(cli->getId());

        string opciones[] = {
            "[ Ver Productos ]",
            "[ Buscar Producto ]",
            "[ Ver Carrito ]",
            "[ Realizar Compra ]",
            "[ Ver Historial ]",
            "[ Cerrar Sesion ]"
        };
        const int total = 6;

        while (true) {
            system("cls");
            cout << "\033[?25l";

            gotoXY(60 - 10, 5);
            cout << "====== MENU CLIENTE ======";

            int sel = menuConMouse(opciones, total, 60, 9);
            cout << "\033[?25h";

            switch (sel) {
            case 0:
                menuVerProductos(uService);
                break;
            case 1:
                menuBuscarProducto(cli, uService);
                break;
            case 2:
                menuCarrito(cli, uService);
                break;
            case 3:
                // menuCompra(cli, uService);
                break;
            case 4:
                // menuHistorialPedidos(cli, uService);
                break;
            case 5:
                cout << "\033[?25h";
                return;
            }
        }

        delete uService;
    }
};
