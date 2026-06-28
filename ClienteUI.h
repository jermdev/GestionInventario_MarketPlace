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

    static void menuVerProductos(ClienteService* uService) {
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
            {
                system("cls");

                Lista<Producto*>* productos = uService->obtenerProductos();

                Lista<ComponenteUI<Producto*>*>* cardProductos = new Lista<ComponenteUI<Producto*>*>();
                for (int i = 0; i < productos->longitud(); i++) {
                    Producto* p = productos->obtenerPos(i);
                    ComponenteUI<Producto*>* card = new ComponenteUI<Producto*>(p);
                    cardProductos->agregaInicial(card);
                }

                CONSOLE_SCREEN_BUFFER_INFO csbi;
                GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);

                int ancho = csbi.srWindow.Right - csbi.srWindow.Left + 1;
                int alto  = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

                int x = (ancho / 15);
                int y = (alto / 7);

                GridView<Producto*>* gridProductos = new GridView<Producto*>(cardProductos, x, y, ancho, alto, 21, 6);
                auto dibujado = [](Producto* p, auto posicion) {

                    int aumentFila = 0;

                    posicion(0);
                    cout << "-------------------" << endl;

                    posicion(1);

                    string nombre = p->getNombre();
                    string auxNombre = "";

                    if (nombre.length() > 11) {
                        auxNombre = nombre.substr(11);
                        nombre = nombre.substr(0, 11);
                    }

                    cout << "Nombre: " << nombre << endl;

                    if (auxNombre.length() > 0) {
                        posicion(2);
                        cout << auxNombre << endl;
                        aumentFila++;
                    }

                    posicion(2 + aumentFila);
                    cout << "Precio: " << p->getPrecio() << endl;

                    string categoria = p->getCategoria();
                    string auxCategoria = "";

                    if (categoria.length() > 11) {
                        auxCategoria = categoria.substr(11);
                        categoria = categoria.substr(0, 11);
                    }

                    posicion(3 + aumentFila);
                    cout << "Categoria: " << categoria << endl;

                    if (auxCategoria.length() > 0) {
                        posicion(4 + aumentFila);
                        cout << auxCategoria << endl;
                        aumentFila++;
                    }
                    int id = p->getId();
                    posicion(4 + aumentFila);
                    cout << "ID: " << id << endl;
                    };
                gridProductos->mostrarGrid(dibujado);
                system("pause");
                break;
            }

            case 1: {
                system("cls");
                string categoria;
                string descripcionCampo = "Dijite la categoria que quiere buscar: ";
                gotoXY(descripcionCampo.length() - 20, 8);
                cout << descripcionCampo; cin >> categoria;
                uService->filtrarPorCategoria(categoria);
                break;
            }

            case 2: {
                string tipoOrden;
                string descripcionCampo = "Ordenar por mayor o menor precio (ejemplo mayor / menor): ";
                system("cls");
                gotoXY(descripcionCampo.length() - 50, 8);

                do {
                    cout << descripcionCampo; cin >> tipoOrden;
                } while (tipoOrden != "mayor" && tipoOrden != "menor");
                system("cls");
                uService->mostrarListaProductosOrdenadaPorPrecio(tipoOrden);
                break;
            }

            case 3:
                return;
            }
        }
    }

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
                cout << "Ingrese el ID del producto a agregar: ";
                cin >> id;
                {
                    Producto* p = uService->buscarProductoPorID(id);
                    if (p != nullptr) {
                        cout << "Ingrese la cantidad: ";
                        cin >> cant;
                        cli->getCarrito()->agregarProducto(p, cant);
                        cout << "Producto agregado al carrito con exito.\n";
                    }
                    else {
                        cout << "Producto no encontrado en el inventario.\n";
                    }
                }
                break;

            case 2:
                if (cli->getCarrito()->getProductos()->esVacia()) {
                    cout << "Tu carrito esta vacio. No puedes eliminar.\n";
                    break;
                }
                cout << "Ingrese el ID del producto a eliminar: ";
                cin >> id;
                cout << "Ingrese la cantidad a eliminar: ";
                cin >> cant;
                cli->getCarrito()->borrarProducto(id, cant);
                break;

            case 3:
                cli->getCarrito()->vaciarCarrito();
                cout << "Vaciando carrito...\n";
                break;

            case 4:
                return;
            }
        }
    }

    static void menuCompra(Cliente* cli, ClienteService* uService) {
        string opciones[] = {
            "[    Confirmar compra     ]",
            "[ Ver resumen (Carrito)   ]",
            "[         Volver          ]"
        };
        const int total = 3;

        while (true) {
            system("cls");
            cout << "\033[?25l";

            gotoXY(60 - 14, 10);
            cout << "====== REALIZAR COMPRA ======";

            int sel = menuConMouse(opciones, total, 60, 13);
            cout << "\033[?25h";

            int mPago, tComp; char tar; string numTar, cvv;

            switch (sel) {
            case 0:
                if (cli->getCarrito()->getProductos()->esVacia()) {
                    cout << "Tu carrito esta vacio. No puedes comprar.\n";
                    break;
                }
                cout << "Metodo de Pago (0: Tarjeta Regalo, 1: Tarjeta): ";
                cin >> mPago;
                cout << "Tipo Comprobante (0: Boleta, 1: Factura): ";
                cin >> tComp;
                cout << "Ingrese su numero de tarjeta: ";
                cin >> numTar;
                cout << "Ingrese su CVV: ";
                while (true) {
                    tar = _getch();
                    if (tar == '\r') {
                        break;
                    }
                    else if (tar == '\b') {
                        if (cvv.length() > 0) {
                            cvv.pop_back();
                            cout << "\b \b";
                        }
                    }
                    else {
                        cvv += tar;
                        cout << "*";
                    }
                }
                uService->realizarCompraProductos(cli->getId(), cli, static_cast<MetodoPago>(mPago), static_cast<TipoComprobante>(tComp));
                break;

            case 1:
                cli->getCarrito()->listarCarrito();
                break;

            case 2:
                return;
            }
        }
    }

    static void menuHistorialPedidos(Cliente* cli, ClienteService* uService) {
        string opciones[] = {
            "[ Ver todos los pedidos ]",
            "[        Volver         ]"
        };
        const int total = 2;

        while (true) {
            system("cls");
            cout << "\033[?25l";

            gotoXY(60 - 16, 10);
            cout << "====== HISTORIAL DE PEDIDOS ======";

            int sel = menuConMouse(opciones, total, 60, 13);
            cout << "\033[?25h";

            switch (sel) {
            case 0:
                uService->listarPedidos(cli->getId());
                system("pause>0");
                break;

            case 1:
                return;
            }
        }
    }

public:
    static void Render(Cliente* cli) {
        ClienteService* uService = new ClienteService();
        uService->inicializarListaProductos();
        uService->InicializarPedidosPorIdCliente(cli->getId());

        string opciones[] = {
            "[    Ver Productos         ]",
            "[    Buscar Producto       ]",
            "[    Ver Carrito           ]",
            "[    Realizar Compra       ]",
            "[ Ver Historial de Pedidos ]",
            "[    Cerrar Sesion         ]"
        };
        const int totalOpciones = 6;

        while (true) {
            system("cls");
            cout << "\033[?25l";

            gotoXY(60 - 13, 8);
            cout << "====== Hola, " << cli->getNombre() <<  " ======";

            int sel = menuConMouse(opciones, totalOpciones, 60, 11);
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
                menuCompra(cli, uService);
                break;

            case 4:
                menuHistorialPedidos(cli, uService);
                break;

            case 5:
                cout << "Cerrando sesion...\n";
                return;
            }
        }
    }
};
