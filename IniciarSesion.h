#pragma once
#include <iostream>
#include <string>
#include "Auth.h"
#include "gotoxy.h"
#include "MouseMenu.h"
using namespace std;

class IniciarSesion {
public:
    // Retorna true si el login fue exitoso, false si cancelo o se agotaron los intentos.
    static bool Render(Auth* auth) {
        const int MAX_INTENTOS = 3;
        int intentos = 0;
        string correo, contrasenia;

        const int izqX    = 45;   // borde izquierdo del formulario
        const int inputX  = 59;   // donde empieza el texto ingresado
        const int tituloY = 5;
        const int correoY = 9;
        const int passY   = 12;
        const int errorY  = 16;
        const int hintY   = 19;
        const int anchoInput = 35;

        system("cls");
        cout << "\033[?25h"; // mostrar cursor para escribir

        gotoXY(60 - 14, tituloY);
        cout << "====== INICIAR SESION ======";

        gotoXY(izqX, correoY);
        cout << "Correo      : ";

        gotoXY(izqX, passY);
        cout << "Contrasenia : ";

        gotoXY(60 - 10, hintY);
        cout << "[ ESC para volver ]";

        while (intentos < MAX_INTENTOS) {

            // limpiar campos de entrada
            gotoXY(inputX, correoY);
            for (int i = 0; i < anchoInput; i++) cout << ' ';
            gotoXY(inputX, passY);
            for (int i = 0; i < anchoInput; i++) cout << ' ';

            // leer correo
            gotoXY(inputX, correoY);
            if (!leerCampo(correo)) return false;

            // leer contrasenia
            gotoXY(inputX, passY);
            if (!leerCampo(contrasenia, true)) return false;

            Auth::ResultadoLogin resultado = auth->iniciarSesion(correo, contrasenia);

            if (resultado == Auth::LOGIN_EXITOSO) {
                system("cls");
                gotoXY(60 - 12, 14);
                cout << "Bienvenido, " << auth->getUsuarioActual()->getNombre() << "!";
                return true;
            }

            if (resultado == Auth::CREDENCIALES_INVALIDAS) {
                intentos++;
                int restantes = MAX_INTENTOS - intentos;

                // limpiar linea de error anterior
                gotoXY(0, errorY);
                for (int i = 0; i < 120; i++) cout << ' ';

                if (restantes > 0) {
                    string msg = "Correo o contrasenia incorrectos. Intentos restantes: " + to_string(restantes);
                    gotoXY(60 - (int)msg.length() / 2, errorY);
                    cout << msg;
                } else {
                    string msg = "Demasiados intentos fallidos.";
                    gotoXY(60 - (int)msg.length() / 2, errorY);
                    cout << msg;
                }
            } else {
                string msg = "Error del sistema al iniciar sesion.";
                gotoXY(60 - (int)msg.length() / 2, errorY);
                cout << msg;
                return false;
            }
        }
        return false;
    }
};
