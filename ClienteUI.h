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

    // ========== MÉTODO PARA CENTRAR EL PRODUCTO EXCLUSIVO DE ESTA UI ==========
    static void mostrarProductoCentrado(Producto* p, int y) {
        string titulo = "--- DETALLES DEL PRODUCTO ---";
        gotoXY(60 - (titulo.length() / 2), y); cout << titulo;

        string strId = "ID: " + to_string(p->getId());
        gotoXY(60 - (strId.length() / 2), y + 2); cout << strId;

        string strNom = "Nombre: " + p->getNombre();
        gotoXY(60 - (strNom.length() / 2), y + 3); cout << strNom;

        string strCat = "Categoria: " + p->getCategoria();
        gotoXY(60 - (strCat.length() / 2), y + 4); cout << strCat;

        // Calculamos el espacio del precio y luego lo imprimimos
        string strPre = "Precio: S/. " + to_string((int)p->getPrecio());
        gotoXY(60 - (strPre.length() / 2), y + 5); cout << "Precio: S/. " << p->getPrecio();

        string strStk = "Stock disponible: " + to_string(p->getStock());
        gotoXY(60 - (strStk.length() / 2), y + 6); cout << strStk;
    }

    // ========== MOSTRAR GRID DE PRODUCTOS CON NAVEGACIÓN ==========

    static void mostrarGridProductos(GridView<Producto*>* gridProductos,ClienteService* uService,const int cellWidth,const int x, const int y,const int gridAlto) {

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

    static void mostrarInfoPagina(int paginaActual, int totalPages, int x, int infoY) {
        gotoXY(x, infoY);
        cout << string(120, ' '); // limpiar línea

        string info = "Pagina " + to_string(paginaActual + 1) + " de " + to_string(totalPages) +
            " | Flechas arriba/abajo para navegar | ESC para salir";
        gotoXY(60 - (info.length() / 2), infoY);
        cout << info;
    }

    // ========== MENÚ VER PRODUCTOS CON NAVEGACIÓN ==========

    static void menuVerProductos(ClienteService* uService) {
        string opciones[] = {
            "[ Mostrar todos los productos ]",
            "[    Filtrar por categoria    ]",
            "[     Ordenar por precio      ]",
            "[            Volver           ]"
        };
        const int total = 4;

        system("cls");
        cout << "\033[?25l"; // ocultar cursor

        string titulo = "====== VER PRODUCTOS ======";
        gotoXY(60 - (titulo.length() / 2), 8);
        cout << titulo;

        int sel = menuConMouse(opciones, total, 60, 11);
        cout << "\033[?25h"; // mostrar cursor

        switch (sel) {
        case 0: // Mostrar todos los productos
        {
            system("cls");
            cout << "\033[?25l"; // ocultar cursor

            Lista<Producto*>* productos = uService->obtenerProductos();

            if (productos == nullptr || productos->esVacia()) {
                string msg = "No hay productos disponibles.";
                gotoXY(60 - (msg.length() / 2), 12);
                cout << msg << "\n";
                gotoXY(60 - 10, 14);
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

            string tituloGrid = "====== VER PRODUCTOS (Grid) ======";
            gotoXY(60 - (tituloGrid.length() / 2), 1);
            cout << tituloGrid;

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
                    gotoXY(60 - (tituloGrid.length() / 2), 1);
                    cout << tituloGrid;

                    // Dibujar grid
                    mostrarGridProductos(gridProductos, uService, cellWidth, x, y, gridAlto);

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
            string descripcionCampo = "Digite la categoria que quiere buscar: ";
            gotoXY(60 - (descripcionCampo.length() / 2), 10);
            cout << descripcionCampo;
            cin >> categoria;
            system("cls");
            uService->filtrarPorCategoria(categoria);
            break;
        }

        case 2: {
            string tipoOrden;
            string descripcionCampo = "Ordenar por mayor o menor precio (ejemplo mayor / menor): ";
            system("cls");

            do {
                gotoXY(60 - (descripcionCampo.length() / 2), 10);
                cout << string(descripcionCampo.length() + 15, ' ');
                gotoXY(60 - (descripcionCampo.length() / 2), 10);
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

    // ========== RESTO DE MENÚS ==========

    static void menuBuscarProducto(Cliente* cli, ClienteService* uService) {
        string opciones[] = {
            "[ Buscar por ID ]",
            "[    Volver     ]"
        };
        const int total = 2;

        while (true) {
            system("cls");
            cout << "\033[?25l";

            string titulo = "====== BUSCAR PRODUCTO ======";
            gotoXY(60 - (titulo.length() / 2), 10);
            cout << titulo;

            int sel = menuConMouse(opciones, total, 60, 13);
            cout << "\033[?25h";

            switch (sel) {
            case 0:
            {
                int id;
                string descripcionCampo = "Digite el id del Producto: ";
                system("cls");
                gotoXY(60 - (descripcionCampo.length() / 2), 10);
                cout << descripcionCampo; cin >> id;

                Producto* p = uService->buscarProductoPorID(id);
                if (p == nullptr) {
                    string msgErr = "Producto no encontrado.";
                    gotoXY(60 - (msgErr.length() / 2), 13);
                    cout << msgErr << "\n";
                    gotoXY(60 - 10, 15);
                    system("pause");
                    break;
                }

                system("cls");

                // Usamos nuestro propio método centrado en lugar de p->MostrarProducto();
                mostrarProductoCentrado(p, 10);

                string resProducto;
                string pregCompra = "Desea comprar este producto? (y/n) :";
                gotoXY(60 - (pregCompra.length() / 2), 18);
                cout << pregCompra; cin >> resProducto;

                if (resProducto == "y") {
                    int cantidad;
                    string pregCant = "Digite cantidad: ";
                    gotoXY(60 - (pregCant.length() / 2), 20);
                    cout << pregCant; cin >> cantidad;
                    if (cantidad <= 0) {
                        string msgVal = "Cantidad no valida.";
                        gotoXY(60 - (msgVal.length() / 2), 22);
                        cout << msgVal;
                        gotoXY(60 - 10, 24);
                        system("pause");
                        break;
                    }
                    cli->getCarrito()->agregarProducto(p, cantidad);
                    string msgEx = "Producto agregado al carrito.";
                    gotoXY(60 - (msgEx.length() / 2), 22);
                    cout << msgEx;
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
            "[      Agregar producto     ]",
            "[     Eliminar producto     ]",
            "[       Vaciar carrito      ]",
            "[           Volver          ]"
        };
        const int total = 5;

        while (true) {
            system("cls");
            cout << "\033[?25l";

            string titulo = "====== CARRITO ======";
            gotoXY(60 - (titulo.length() / 2), 9);
            cout << titulo;

            int sel = menuConMouse(opciones, total, 60, 12);
            cout << "\033[?25h";

            int id, cant;

            switch (sel) {
            case 0: {
                system("cls");
                cout << "\033[?25l";

                Lista<NProductos*>* lista = cli->getCarrito()->getProductos();

                if (lista == nullptr || lista->esVacia()) {
                    string msgVac = "Tu carrito esta vacio.";
                    gotoXY(60 - (msgVac.length() / 2), 10);
                    cout << msgVac << "\n";
                    gotoXY(60 - 10, 12);
                    cout << "\033[?25h";
                    system("pause > 0");
                    break;
                }

                int nProductos = lista->longitud();

                double montoTotal = 0;
                for (int i = 0; i < nProductos; i++) {
                    NProductos* np = lista->obtenerPos(i);
                    montoTotal += np->producto->getPrecio() * np->cantidad;
                }

                Lista<ComponenteUI<NProductos*>*>* cardCarrito = new Lista<ComponenteUI<NProductos*>*>();
                for (int i = 0; i < nProductos; i++) {
                    ComponenteUI<NProductos*>* card = new ComponenteUI<NProductos*>(lista->obtenerPos(i));
                    cardCarrito->agregaFinal(card);
                }

                CONSOLE_SCREEN_BUFFER_INFO csbi;
                GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);

                int ancho = csbi.srWindow.Right - csbi.srWindow.Left + 1;
                int alto = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

                int x = 2;
                int y = 5;                 
                int gridAncho = ancho - 4;
                int gridAlto = alto - 10;
                int cellWidth = 22;
                int cellHeight = 7;

                GridView<NProductos*>* gridCarrito = new GridView<NProductos*>(cardCarrito, x, y, gridAncho, gridAlto, cellWidth, cellHeight, 1, 1);
                //ESTO SE USA PARA ORDENAR LOS PRODUCTO EN ORDEN
                auto dibujado = [cellWidth](NProductos* np, auto posicion) {
                    int line = 0;
                    Producto* p = np->producto;

                    posicion(line++);
                    cout << string(cellWidth, '-');

                    posicion(line++);
                    cout << truncate(p->getNombre(), cellWidth - 2);

                    posicion(line++);
                    cout << "Precio: " << p->getPrecio();

                    posicion(line++);
                    cout << "Cantidad: " << np->cantidad;

                    posicion(line++);
                    cout << "ID: " << p->getId();

                    posicion(line++);
                    cout << string(cellWidth, '-');
                    };
                //ESTO SE USA PARA MOSTRAR EN LA PARTE SUPERIOR
                auto dibujarEncabezado = [&]() {
                    string tituloGrid = "====== CARRITO DE COMPRAS ======";
                    gotoXY(60 - (tituloGrid.length() / 2), 1);
                    cout << tituloGrid;

                    string strItems = "Total de items distintos: " + to_string(nProductos);
                    gotoXY(60 - (strItems.length() / 2), 2);
                    cout << strItems;

                    string strMonto = "Monto total acumulado: S/ " + to_string(montoTotal);
                    gotoXY(60 - (strMonto.length() / 2), 3);
                    cout << strMonto;
                    };

                dibujarEncabezado();
                gridCarrito->mostrarGrid(dibujado);

                int infoY = y + gridAlto + 1;
                mostrarInfoPagina(gridCarrito->getPaginaActual(),
                    gridCarrito->getTotalPages(), x, infoY);
                //ESTO SE USA PARA QUE LAS TECLAS CUMPLAN UNA FUNCION
                bool salir = false;
                while (!salir) {
                    if (_kbhit()) {
                        int tecla = _getch();

                        if (tecla == 27 || tecla == 'q' || tecla == 'Q') {
                            salir = true;
                            break;
                        }

                        if (tecla == 224 || tecla == 0) {
                            int arrowKey = _getch();
                            if (arrowKey == 80) {
                                gridCarrito->siguientePagina();
                            }
                            else if (arrowKey == 72) {
                                gridCarrito->paginaAnterior();
                            }
                        }

                        system("cls");
                        dibujarEncabezado();
                        gridCarrito->mostrarGrid(dibujado);
                        mostrarInfoPagina(gridCarrito->getPaginaActual(),
                            gridCarrito->getTotalPages(), x, infoY);
                    }
                }

                cout << "\033[?25h";
                delete gridCarrito;
                delete cardCarrito;
                break;
            }
            case 1:
            {
                system("cls");
                string promptAdd = "Ingrese el ID del producto a agregar: ";
                gotoXY(60 - (promptAdd.length() / 2), 10);
                cout << promptAdd;
                cin >> id;

                Producto* p = uService->buscarProductoPorID(id);
                if (p != nullptr) {
                    string promptCant = "Ingrese la cantidad: ";
                    gotoXY(60 - (promptCant.length() / 2), 12);
                    cout << promptCant;
                    cin >> cant;
                    cli->getCarrito()->agregarProducto(p, cant);
                    string prodAgr = "Producto agregado exitosamente..";
                    gotoXY(60 - (prodAgr.length() / 2), 14);
                    cout << prodAgr << "\n";
                    gotoXY(60 - 10, 12);
                    system("pause > 0");
                }
                else {
                    string msgNo = "Producto no encontrado en el inventario.";
                    gotoXY(60 - (msgNo.length() / 2), 12);
                    cout << msgNo << "\n";
                }
                gotoXY(60 - 10, 14);
                system("pause > nul");
                break;
            }

            case 2:
            {
                system("cls");
                if (cli->getCarrito()->getProductos()->esVacia()) {
                    string msgVac = "Tu carrito esta vacio. No puedes eliminar.";
                    gotoXY(60 - (msgVac.length() / 2), 10);
                    cout << msgVac << "\n";
                    gotoXY(60 - 10, 12);
                    system("pause > nul");
                    break;
                }
                string promptDel = "Ingrese el ID del producto a eliminar: ";
                gotoXY(60 - (promptDel.length() / 2), 10);
                cout << promptDel;
                cin >> id;
                string promptDelC = "Ingrese la cantidad a eliminar: ";
                gotoXY(60 - (promptDelC.length() / 2), 12);
                cout << promptDelC;
                cin >> cant;
                cli->getCarrito()->borrarProducto(id, cant);
                string prodEl = "Producto eliminado del carrito exitosamente.";
                gotoXY(60 - (prodEl.length() / 2), 14);
                cout << prodEl << "\n";
                gotoXY(60 - 10, 16);
                system("pause > nul");
                break;
            }

            case 3:
            {
                system("cls");
                cli->getCarrito()->vaciarCarrito();
                string msgClean = "Vaciando carrito...";
                gotoXY(60 - (msgClean.length() / 2), 10);
                cout << msgClean << "\n";
                gotoXY(60 - 10, 12);
                system("pause > nul");
                break;
            }

            case 4:
                return;

            default:
                cout << "Opcion no valida.\n";
            }
        }
    }

    static void menuCompra(Cliente* cli, ClienteService* uService) {
        string opciones[] = {
            "[    Confirmar compra     ]",
            "[ Ver resumen (Carrito)   ]",
            "[          Volver         ]"
        };
        const int total = 3;

        while (true) {
            system("cls");
            cout << "\033[?25l";

            string titulo = "====== REALIZAR COMPRA ======";
            gotoXY(60 - (titulo.length() / 2), 10);
            cout << titulo;

            int sel = menuConMouse(opciones, total, 60, 13);
            cout << "\033[?25h";

            int mPago, tComp; char tar,tecla; string numTar, cvv, extra;

            switch (sel) {
            case 0:
            {
                system("cls");
                if (cli->getCarrito()->getProductos()->esVacia()) {
                    string msgVac = "Tu carrito esta vacio. No puedes comprar.";
                    gotoXY(60 - (msgVac.length() / 2), 10);
                    cout << msgVac << "\n";
                    gotoXY(60 - 10, 12);
                    system("pause > 0");
                    break;
                }
                string p1 = "Metodo de Pago (0: Tarjeta Regalo, 1: Tarjeta): ";
                gotoXY(60 - (p1.length() / 2), 6); cout << p1; cin >> mPago;
                string p2 = "Tipo Comprobante (0: Boleta, 1: Factura): ";
                gotoXY(60 - (p2.length() / 2), 8); cout << p2; cin >> tComp;

                if (tComp == 0) {
                    string pDni = "Ingrese su dni: ";
                    gotoXY(60 - (pDni.length() / 2), 10); cout << pDni; 
                    
                  
                    string extra="";
                    while (extra.length() < 8) {
                        tecla = _getch();

                        if (tecla >= '0' && tecla <= '9') {
                            extra += tecla;
                            cout << tecla;
                        }
                    }
                }
                else {
                    string pRuc = "Ingrese su RUC: ";
                    gotoXY(60 - (pRuc.length() / 2), 10); cout << pRuc; cin >> extra;
                    string extra = "";
                    while (extra.length() < 11) {
                        tecla = _getch();

                        if (tecla >= '0' && tecla <= '9') {
                            extra += tecla;
                            cout << tecla;
                        }
                    }
                }
                string pTar = "Ingrese su numero de tarjeta: ";
                gotoXY(60 - (pTar.length() / 2), 12); cout << pTar; cin >> numTar;
                string pCvv = "Ingrese su CVV: ";
                gotoXY(60 - (pCvv.length() / 2), 14); cout << pCvv;

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
                uService->realizarCompraProductos(cli->getId(), cli, static_cast<MetodoPago>(mPago), static_cast<TipoComprobante>(tComp), extra);
                string msgOk = "Producto comprado correctamente. Revisar Pedidos.";
                gotoXY(60 - (msgOk.length() / 2), 17); cout << msgOk;
                gotoXY(60 - 10, 19);
                system("pause > 0");
                break;
            }

            case 1:
                system("cls");
                cli->getCarrito()->listarCarrito();
                system("pause > 0");
                break;

            case 2:
                return;
            }
        }
    }

    static void menuHistorialPedidos(Cliente* cli, ClienteService* uService) {
        string opciones[] = {
            "[ Ver todos los pedidos ]",
            "[         Volver        ]"
        };
        const int total = 2;

        while (true) {
            system("cls");
            cout << "\033[?25l";

            string titulo = "====== HISTORIAL DE PEDIDOS ======";
            gotoXY(60 - (titulo.length() / 2), 10);
            cout << titulo;

            int sel = menuConMouse(opciones, total, 60, 13);
            cout << "\033[?25h";

            switch (sel) {
            case 0:
                system("cls");
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

            string titulo = "====== MENU CLIENTE ======";
            gotoXY(60 - (titulo.length() / 2), 5);
            cout << titulo;

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
                menuCompra(cli, uService);
                break;
            case 4:
                menuHistorialPedidos(cli, uService);
                break;
            case 5:
                cout << "\033[?25h";
                delete uService;
                return;
            }
        }

        delete uService;
    }
};