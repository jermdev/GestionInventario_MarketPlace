#pragma once
#include <iostream>
#include <string>
#include "Auth.h"
#include "gotoxy.h"
#include "MouseMenu.h"
using namespace std;

class Registrar {
public:
    static void Render(Auth* auth) {
        system("cls");
        cout << "\033[?25l"; // ocultar cursor mientras se muestra el menu

        gotoXY(60 - 10, 7);
        cout << "====== REGISTRO ======";

        gotoXY(60 - 9, 9);
        cout << "Tipo de cuenta:";

        string tiposOpciones[] = {
            "[ Cliente        ]",
            "[ Vendedor       ]",
            "[ Administrador  ]",
            "[ Cancelar       ]"
        };
        const int totalTipos = 4;

        int sel = menuConMouse(tiposOpciones, totalTipos, 60, 11);

        if (sel == 3) return;

        int tipo = sel + 1; // 0=Cliente(1), 1=Vendedor(2), 2=Administrador(3)

        // --- formulario de datos ---
        system("cls");
        cout << "\033[?25h"; // mostrar cursor para escribir

        string tipoNombre = (tipo == 1) ? "Cliente" : (tipo == 2) ? "Vendedor" : "Administrador";

        gotoXY(60 - 10, 3);
        cout << "====== REGISTRO ======";

        gotoXY(60 - 8, 5);
        cout << "Cuenta: " << tipoNombre;

        const int izqX   = 45;
        const int inputX  = 59;

        gotoXY(izqX, 8);  cout << "Nombre      : ";
        gotoXY(izqX, 10); cout << "Correo      : ";
        gotoXY(izqX, 12); cout << "Direccion   : ";
        gotoXY(izqX, 14); cout << "Contrasenia : ";
        if (tipo == 3) {
            gotoXY(izqX, 16); cout << "Cargo       : ";
        }

        gotoXY(60 - 10, 21);
        cout << "[ ESC para cancelar ]";

        string nombre, correo, direccion, contrasenia, cargo;

        gotoXY(inputX, 8);
        if (!leerCampo(nombre)) return;

        gotoXY(inputX, 10);
        if (!leerCampo(correo)) return;

        gotoXY(inputX, 12);
        if (!leerCampo(direccion)) return;

        gotoXY(inputX, 14);
        if (!leerCampo(contrasenia, true)) return;

        if (tipo == 3) {
            gotoXY(inputX, 16);
            if (!leerCampo(cargo)) return;
        }

        Auth::ResultadoRegistro resultado;
        if (tipo == 1)
            resultado = auth->registrarCliente(nombre, correo, direccion, contrasenia);
        else if (tipo == 2)
            resultado = auth->registrarVendedor(nombre, correo, direccion, contrasenia);
        else
            resultado = auth->registrarAdministrador(nombre, correo, direccion, contrasenia, cargo);

        system("cls");
        string msg;
        switch (resultado) {
        case Auth::REGISTRO_EXITOSO:
            msg = "Registro exitoso. Ya puede iniciar sesion.";
            break;
        case Auth::CORREO_DUPLICADO:
            msg = "El correo ya esta registrado. Use otro correo.";
            break;
        case Auth::REGISTRO_ERROR:
        default:
            msg = "Error al registrar. Intente de nuevo.";
            break;
        }
        gotoXY(60 - (int)msg.length() / 2, 14);
        cout << msg << "\n\n";
        system("pause");
    }
};
