#pragma once
#include <iostream>

#include "Auth.h"
#include "IniciarSesion.h"
#include "Registrar.h"
#include "ClienteUI.h"
#include "VendedorUI.h"
#include "AdministradorUI.h"
#include "gotoxy.h"
#include "MouseMenu.h"

using namespace std;



class AuthUI {
private:
    static void imprimirLogo(int x, int y) {
        int recorridos = 0;
        string logo[] = {
"    _                                                              ",
"   / \\    _ __ ___    __ _  ____ ___   _ __                       ",
"  / _ \\  | '_ ` _ \\  / _` ||_  // _ \\ | '_ \\                      ",
" / ___ \\ | | | | | || (_| | / /| (_) || | | |                     ",
"/_/   \\_\\|_| |_| |_| \\__,_|/___|\\___/ |_| |_|                     ",
"",
"                 A M A Z O N                                       "
        };
        for (const string& linea : logo) {
            gotoXY(x, y+recorridos);
            cout << linea << endl;
            recorridos++;
        }
    }
public:

    static void Render(Auth* auth) {

        string opciones[] = {
            "[ Iniciar Sesion ]",
            "[  Registrarse   ]",
            "[     Salir      ]"
        };
        const int totalOpciones = 3;

        while (true) {

            system("cls");
            cout << "\033[?25l";

            int centroX = 40;
            int inicioY = 8;
            int altoLogo = 12;
            int opcionesY = inicioY + altoLogo;

            imprimirLogo(centroX - 12, inicioY - 3);

            int sel = menuConMouse(opciones, totalOpciones, 60, opcionesY);

            cout << "\033[?25h";

            switch (sel) {

            case 0: {
                bool ok = IniciarSesion::Render(auth);

                if (ok && auth->hayUsuarioActivo()) {

                    int tipo = auth->getUsuarioActual()->getTipoId();
                    Usuario* u = auth->getUsuarioActual();

                    if (tipo == 1) {
                        Cliente* c = dynamic_cast<Cliente*>(u);
                        ClienteUI::Render(c);
                    }
                    else if (tipo == 2) {
                        Vendedor* v = dynamic_cast<Vendedor*>(u);
                        VendedorUI::Render(v);
                    }
                    else {
                        Administrador* a = dynamic_cast<Administrador*>(u);
                        AdministradorUI::Render(a);
                    }
                }
                break;
            }

            case 1:
                Registrar::Render(auth);
                break;

            case 2:
                cout << "\nHasta luego.\n";
                return;
            }
        }
    }
};