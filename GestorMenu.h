#pragma once
#include <iostream>
using namespace std;

class GestorMenus {
public:

    // ===== MENÚ PRINCIPAL =====
    static int menuPrincipal() {
        int opcion;
        cout << "\n====== MARKET PLACE ======\n";
        cout << "1. Iniciar Sesion\n";
        cout << "2. Registrarse\n";
        cout << "0. Salir\n";
        cout << "Seleccione una opcion: ";
        cin >> opcion;
        return opcion;
    }

    // ===== MENÚ CLIENTE =====
    static int menuCliente() {
        int opcion;
        cout << "\n====== MENU CLIENTE ======\n";
        cout << "1. Ver Productos\n";
        cout << "2. Buscar Producto\n";
        cout << "3. Ver Carrito\n";
        cout << "4. Realizar Compra\n";
        cout << "5. Ver Historial de Pedidos\n";
        cout << "0. Cerrar Sesion\n";
        cout << "Seleccione una opcion: ";
        cin >> opcion;
        return opcion;
    }

    // ===== MENÚ VENDEDOR =====
    static int menuVendedor() {
        int opcion;
        cout << "\n====== MENU VENDEDOR ======\n";
        cout << "1. Registrar Producto\n";
        cout << "2. Eliminar Producto\n";
        cout << "3. Actualizar Producto\n";
        cout << "4. Ver Mis Productos\n";
        cout << "5. Ver Pedidos\n";
        cout << "0. Cerrar Sesion\n";
        cout << "Seleccione una opcion: ";
        cin >> opcion;
        return opcion;
    }

    // ===== MENÚ PRODUCTOS =====
    static int menuProductos() {
        int opcion;
        cout << "\n====== PRODUCTOS ======\n";
        cout << "1. Listar Productos\n";
        cout << "2. Filtrar por precio\n";
        cout << "3. Agregar al carrito\n";
        cout << "0. Volver\n";
        cout << "Seleccione una opcion: ";
        cin >> opcion;
        return opcion;
    }

    // ===== MENÚ CARRITO =====
    static int menuCarrito() {
        int opcion;
        cout << "\n====== CARRITO ======\n";
        cout << "1. Ver productos\n";
        cout << "2. Eliminar producto\n";
        cout << "3. Vaciar carrito\n";
        cout << "4. Proceder al pago\n";
        cout << "0. Volver\n";
        cout << "Seleccione una opcion: ";
        cin >> opcion;
        return opcion;
    }

    // ===== MENÚ PAGO =====
    static int menuPago() {
        int opcion;
        cout << "\n====== METODO DE PAGO ======\n";
        cout << "1. Tarjeta\n";
        cout << "2. Tarjeta de regalo\n";
        cout << "0. Cancelar\n";
        cout << "Seleccione una opcion: ";
        cin >> opcion;
        return opcion;
    }

    // ===== MENÚ AUTH =====
    static int menuAuth() {
        int opcion;
        cout << "\n====== AUTENTICACION ======\n";
        cout << "1. Login\n";
        cout << "2. Registro Cliente\n";
        cout << "3. Registro Vendedor\n";
        cout << "0. Volver\n";
        cout << "Seleccione una opcion: ";
        cin >> opcion;
        return opcion;
    }

    // ===== MENÚ AUTH =====
    static int menuRegister() {
        int opcion;
        string correo;
        string contraseña;
        cout << "\n====== Registro ======\n";
        cout << "Correo: \n"; cin >> correo;
        cout << "Contraseña\n"; cin >> contraseña;
        cin >> opcion;
        return opcion;
    }

    // ===== MENÚ PEDIDOS =====
    static int menuPedidos() {
        int opcion;
        cout << "\n====== PEDIDOS ======\n";
        cout << "1. Ver pedidos pendientes\n";
        cout << "2. Ver pedidos enviados\n";
        cout << "3. Ver pedidos entregados\n";
        cout << "0. Volver\n";
        cout << "Seleccione una opcion: ";
        cin >> opcion;
        return opcion;
    }
};