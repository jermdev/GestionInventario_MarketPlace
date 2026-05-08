#pragma once
#include <iostream>
#include <string>
#include <conio.h>
#include "Auth.h"

using namespace std;

class IniciarSesion {
public:
    // Muestra la pantalla de login. Permite hasta 3 intentos fallidos.
    // Retorna true si el login fue exitoso, false si se agotaron los intentos.
    static bool Render(Auth* auth) {
        const int MAX_INTENTOS = 3;
        int intentos = 0;
        string correo, contrasenia;
        char ch;

        while (intentos < MAX_INTENTOS) {
            cout << "\n====== INICIAR SESION ======\n";
            cout << "Correo: ";
            cin >> correo;
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
			cout << "\n";
            Auth::ResultadoLogin resultado = auth->iniciarSesion(correo, contrasenia);

            if (resultado == Auth::LOGIN_EXITOSO) {
                cout << "Bienvenido, " << auth->getUsuarioActual()->getNombre() << "!\n";
                return true;
            }

            if (resultado == Auth::CREDENCIALES_INVALIDAS) {
                intentos++;
                int restantes = MAX_INTENTOS - intentos;
                if (restantes > 0)
                    cout << "Correo o contrasenia incorrectos. Intentos restantes: " << restantes << "\n";
                else
                    cout << "Demasiados intentos fallidos.\n";
            } else {
                cout << "Error del sistema al iniciar sesion.\n";
                return false;
            }
        }
        return false;
    }
};
