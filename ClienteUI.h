#pragma once
#include <iostream>
#include "ClienteService.h"
#include "ComponenteUI.h"
#include "GridView.h"
#include "Producto.h"
using namespace std;

class ClienteUI {


    static void menuVerProductos(ClienteService* uService) {
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

            switch (opcion) {
            case 1:
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

                GetConsoleScreenBufferInfo(
                    GetStdHandle(STD_OUTPUT_HANDLE),
                    &csbi
                );

                int ancho =
                    csbi.srWindow.Right -
                    csbi.srWindow.Left + 1;

                int alto =
                    csbi.srWindow.Bottom -
                    csbi.srWindow.Top + 1;

                int x = (ancho / 15);
                int y = (alto / 7);

                GridView<Producto*> *gridProductos = new GridView<Producto*>(cardProductos,x,y, ancho, alto, 21, 6);
                auto dibujado = [](Producto* p, auto posicion) {

                    int aumentFila = 0;

                    posicion(0);
                    cout << "-------------------" << endl;

                    posicion(1);

                    string nombre = p->getNombre();
                    string auxNombre = "";

                    if (nombre.length() > 11) {
                        auxNombre = nombre.substr(11);   // resto
                        nombre = nombre.substr(0, 11);   // primera parte
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
                    };
                gridProductos->mostrarGrid(dibujado);
                system("pause");
                break;
            }

            case 2: {
                string categoria;
                cout << "Dijite la categoria que quiere buscar: "; cin >> categoria;
                uService->filtrarPorCategoria(categoria);
                break;
            }

            case 3: {
                    string tipoOrden;
                do
                {
                    cout << "Ordenar por mayor o menor precio (ejemplo mayor / menor): "; cin >> tipoOrden;

                } while (tipoOrden!= "mayor" && tipoOrden != "menor");
                
                uService->mostrarListaProductosOrdenadaPorPrecio(tipoOrden);
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

    static void menuBuscarProducto(Cliente* cli,ClienteService* uService) {
        int opcion;
        system("cls");
        do {
            cout << "\n====== BUSCAR PRODUCTO ======\n";
            cout << "1. Buscar por ID:\n";
            cout << "0. Volver\n";
            cout << "Seleccione una opcion: ";
            cin >> opcion;

            switch (opcion) {
            case 1: 
            {   
                int id;
                cout << "Dijite el id del Producto."; cin >> id;

                Producto* p = uService->buscarProductoPorID(id);
                if (p == nullptr) {
                    cout << "\nProducto no encontrado.\n";
                    system("pause");
                    break;
                }
                string resProducto;
                p->MostrarProducto();
                cout << "Desea comprar este producto? (y/n) :"; cin >> resProducto;

                if (resProducto == "n") {
                    break;
                }

                if (resProducto == "y") {
                    int cantidad;
                    cout << "Digite cantidad: "; cin >> cantidad;
                    if (cantidad <= 0) {
                        cout << "Cantidad no valida.";
                        system("pause");
                        break;
                    }
                        cli->getCarrito()->agregarProducto(p, cantidad);
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

    static void menuCarrito(Cliente* cli, ClienteService* uService) {
        system("cls");
        int opcion, id, cant;
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
                cli->getCarrito()->listarCarrito();
                break;
            case 2:
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
            case 3:
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
            case 4:
                cli->getCarrito()->vaciarCarrito();
                cout << "Vaciando carrito...\n";
                break;
            case 0:
                system("cls");
                break;
            default:
                cout << "Opcion no valida.\n";
            }
        } while (opcion != 0);
    }

    static void menuCompra(Cliente* cli, ClienteService* uService) {
        system("cls");
        int opcion, mPago, tComp; char tar; string numTar,cvv;
        do {
            cout << "\n====== REALIZAR COMPRA ======\n";
            cout << "1. Confirmar compra\n";
            cout << "2. Ver resumen de compra (Carrito)\n";
            cout << "0. Volver\n";
            cout << "Seleccione una opcion: ";
            cin >> opcion;

            switch (opcion) {
            case 1:
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
                //FALTA IMPLEMENTAR QUE APAREZCA LA TARJETA
                uService->realizarCompraProductos(cli->getId(),cli,static_cast<MetodoPago>(mPago),static_cast<TipoComprobante>(tComp) );
                break;
            case 2:
                cli->getCarrito()->listarCarrito();
                break;
            case 0:
                system("cls");
                break;
            default:
                cout << "Opcion no valida.\n";
            }
        } while (opcion != 0);
    }

    static void menuHistorialPedidos(Cliente* cli, ClienteService* uService) {
        int opcion;
        system("cls");
        do {
            cout << "\n====== HISTORIAL DE PEDIDOS ======\n";
            cout << "1. Ver todos los pedidos\n";
            cout << "0. Volver\n";
            cout << "Seleccione una opcion: ";
            cin >> opcion;

            switch (opcion) {
            case 1:
                uService->listarPedidos(cli->getId());
                break;

            case 0:
                system("cls");
                break;

            default:
                cout << "Opcion no valida.\n";
            }

        } while (opcion != 0);
    }
    public:
    static void Render(Cliente* cli) {
        ClienteService* uService = new ClienteService();
        uService->inicializarListaProductos();
        uService->InicializarPedidosPorIdCliente(cli->getId());
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
                menuVerProductos(uService);
                break;

            case 2:
                
                menuBuscarProducto(cli,uService);
                break;

            case 3:
                
                menuCarrito(cli, uService);
                break;

            case 4:
                
                menuCompra(cli,uService);
                break;

            case 5:
                
                menuHistorialPedidos(cli, uService);
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
