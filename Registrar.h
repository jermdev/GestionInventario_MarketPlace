#pragma once
#include <iostream>
#include <string>
#include <limits>
#include "Auth.h"
using namespace std;

class Registrar {
public:
    static void Render(Auth* auth) {
        cout << "\n====== REGISTRO ======\n";
        cout << "Tipo de cuenta:\n";
        cout << "  1. Cliente\n";
        cout << "  2. Vendedor\n";
        cout << "  3. Administrador\n";
        cout << "  0. Cancelar\n";
        cout << "Seleccione: ";

        int tipo;
        cin >> tipo;

        if (tipo == 0) return;
        if (tipo != 1 && tipo != 2 && tipo!=3) {
            cout << "Opcion no valida.\n";
            return;
        }

        // Limpiar el '\n' que quedo en el buffer despues de cin >> tipo
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        string nombre, correo, direccion, contrasenia,cargo;
		char ch;

        cout << "Nombre completo: ";
        getline(cin, nombre);

        cout << "Correo electronico: ";
        cin >> correo;

        cout << "Direccion: ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(cin, direccion);

        cout << "Contrasenia: ";
        while (true) {
            ch = _getch();

            if (ch == '\r') {
                break;
            }
            else if (ch == '\b') {
                if (contrasenia.length() > 0) {
                    contrasenia.pop_back();
                    cout << "\b \b";
                }
            }
            else {
                contrasenia += ch;
                cout << "*";
            }
        }
        if (tipo == 3) {
            cout << "\nCargo: ";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            getline(cin, cargo);
        }

        cout << "\n";
        Auth::ResultadoRegistro resultado;
        if (tipo == 1)
            resultado = auth->registrarCliente(nombre, correo, direccion, contrasenia);
        else if (tipo == 2)
            resultado = auth->registrarVendedor(nombre, correo, direccion, contrasenia);
        else
            resultado = auth->registrarAdministrador(nombre, correo, direccion, contrasenia, cargo);

        switch (resultado) {
        case Auth::REGISTRO_EXITOSO:
            cout << "Registro exitoso. Ya puede iniciar sesion.\n";
            break;
        case Auth::CORREO_DUPLICADO:
            cout << "El correo ya esta registrado. Use otro correo.\n";
            break;
        case Auth::REGISTRO_ERROR:
        default:
            cout << "Error al registrar. Intente de nuevo.\n";
            break;
        }
    }
};
