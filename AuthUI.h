#pragma once
#include <iostream>
#include "Auth.h"
#include "IniciarSesion.h"
#include "Registrar.h"
#include "ClienteUI.h"
#include "VendedorUI.h"
using namespace std;

class AuthUI {
public:
    // Punto de entrada principal de la aplicacion.
    // Loop hasta que el usuario elija salir (opcion 0).
    static void Render(Auth* auth) {
        int opcion;
        do {
            cout << "\n============================\n";
            cout << "     MARKET PLACE\n";
            cout << "============================\n";
            cout << "1. Iniciar Sesion\n";
            cout << "2. Registrarse\n";
            cout << "0. Salir\n";
            cout << "Seleccione una opcion: ";
            cin >> opcion;

            switch (opcion) {
            case 1: {
                bool ok = IniciarSesion::Render(auth);
                if (ok && auth->hayUsuarioActivo()) {
                    int tipo = auth->getUsuarioActual()->getId();
                    if (tipo == 1)
                        ClienteUI::Render();
                    else
                        VendedorUI::Render();
                    auth->cerrarSesion();
                }
                break;
            }
            case 2:
                Registrar::Render(auth);
                break;
            case 0:
                cout << "Hasta luego.\n";
                break;
            default:
                cout << "Opcion no valida.\n";
                break;
            }
        } while (opcion != 0);
    }
};
