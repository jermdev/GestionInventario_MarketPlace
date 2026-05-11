#pragma once
#include "Cliente.h"
#include "UsuarioService.h"
#include <iostream>
using namespace std;

class ClienteUI {


    static void menuVerProductos(UsuarioService* uService) {
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
               
                uService->mostrarTodosLosProductos();
                break;

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

    static void menuBuscarProducto(Cliente* cli,UsuarioService* uService) {
        int opcion;

        do {
            cout << "\n====== BUSCAR PRODUCTO ======\n";
            cout << "1. Digite el Id del Producto:\n";
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
                break;

            default:
                cout << "Opcion no valida.\n";
            }

        } while (opcion != 0);
    }

    static void menuCarrito(Cliente* cli, UsuarioService* uService) {
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

    static void menuCompra(Cliente* cli, UsuarioService* uService) {
        int opcion, mPago, tComp;
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

                uService->realizarCompraProductos(cli->getId(),cli,static_cast<MetodoPago>(mPago),static_cast<TipoComprobante>(tComp) );
                break;
            case 2:
                cli->getCarrito()->listarCarrito();
                break;
            case 0:
                break;
            default:
                cout << "Opcion no valida.\n";
            }
        } while (opcion != 0);
    }

    static void menuHistorialPedidos(Cliente* cli, UsuarioService* uService) {
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
                uService->listarPedidos();
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
        UsuarioService* uService = new UsuarioService();
        uService->inicializarListaProductos();
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
