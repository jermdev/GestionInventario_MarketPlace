#pragma once
#include <iostream>

#include "Auth.h"
#include "IniciarSesion.h"
#include "Registrar.h"
#include "ClienteUI.h"
#include "VendedorUI.h"
#include "AdministradorUI.h"
#include "gotoxy.h"

using namespace std;



class AuthUI {
private:
    static void imprimirLogo(int x, int y) {
        int recorridos = 0;
        string logo[] = {
        "\033[37m⠀⠀⢀⣀⣀⣀⣀⠀⠀⠀⠀⣀⣀⡀⢀⣀⣀⠀⠀⣀⣀⣀⠀⠀⠀⠀⠀⣀⣀⣀⣀⡀⠀⠀⠀⣀⣀⣀⣀⣀⡀⠀⠀⣀⣀⣀⠀⠀⠀⠀⢀⣀⣀⠀⣀⣀⣀⠀\033[0m",
        "\033[37m⠀⣴⣿⡿⠿⢿⣿⣷⡀⠀⣿⣿⣿⣿⣿⣿⣷⣾⣿⣿⣿⣷⠀⢀⣾⣿⡿⠿⣿⣿⣧⠀⠀⠿⠿⠿⣿⣿⣿⠇⠀⣴⣿⡿⠿⣿⣿⣆⠀⢸⣿⣿⣿⣿⣿⣿⣆⠀\033[0m",
        "\033[37m⠀⠉⢉⣁⣠⣼⣿⣿⡇⠀⣿⣿⡏⠀⠈⣿⣿⡇⠀⢹⣿⣿⠀⠈⠉⣉⣀⣤⣼⣿⣿⠀⠀⠀⠀⣴⣿⣿⠋⠀⢰⣿⣿⠁⠀⠀⢸⣿⣿⡄⢸⣿⣿⠃⠀⢹⣿⣿\033[0m",
        "\033[37m⢀⣾⣿⡿⠛⢹⣿⣿⡇⠀⣿⣿⡇⠀⠀⣿⣿⡇⠀⢸⣿⣿⠀⢠⣾⣿⠟⠛⢹⣿⣿⠀⠀⢀⣾⣿⡿⠁⠀⠀⢸⣿⣿⠀⠀⠀⢀⣿⣿⡇⢸⣿⣿⠀⠀⢸⣿⣿\033[0m",
        "\033[37m⢸⣿⣿⣇⣀⣼⣿⣿⣇⠀⣿⣿⡇⠀⠀⣿⣿⡇⠀⢸⣿⣿⠀⢸⣿⣿⣄⣀⣾⣿⣿⣄⠠⣿⣿⣿⣿⣷⣶⠀⠈⣿⣿⣧⣀⣼⣿⡿⠁⢸⣿⣿⠀⠀⢸⣿⣿⠀\033[0m",
        "\033[37m⠀⠛⠿⠿⠿⠋⠹⠿⠋⠀⠿⠿⠇⠀⠀⠿⠿⠃⠀⠸⠿⠿⠀⠈⠻⠿⠿⠟⠋⠻⠟⠁⠀⠛⠉⠉⣉⠉⠛⠇⠀⠈⠻⠿⠿⠿⠛⠁⠀⠸⠿⠿⠀⠀⠸⠿⠟⠀\033[0m",
        "\033[33m⠀⠀⠀⠀⠀⠀⠀⠀⠐⢶⣄⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠞⠛⠛⠻⣍⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\033[0m",
        "\033[33m⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠻⢿⡒⠦⢤⣀⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣀⣠⠤⣶⡾⠃⢠⡿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\033[0m",
        "\033[33m⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠓⠲⠤⣈⣉⠉⠙⠒⠒⠒⠒⠚⠛⠉⣉⣩⡤⠖⠋⠁⠀⠰⠿⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\033[0m",
        "\033[33m⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠉⠉⠉⠛⠛⠛⠉⠉⠉⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\033[0m"
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
            "Iniciar Sesion",
            "Registrarse",
            "Salir"
        };

        const int totalOpciones = 3;
        int seleccion = 0;

        while (true) {

            system("cls");

            int centroX = 40;
            int inicioY = 8;
            int altoLogo = 12;

            imprimirLogo(centroX - 12, inicioY - 3);

            

            for (int i = 0; i < totalOpciones; i++) {

                gotoXY(centroX - 10, inicioY + i + altoLogo);

                if (i == seleccion)
                    cout << "> " << opciones[i];
                else
                    cout << "  " << opciones[i];
            }

            int tecla = _getch();

            if (tecla == 224) {

                tecla = _getch();

                switch (tecla) {

                case 72: // Flecha arriba
                    seleccion--;
                    if (seleccion < 0)
                        seleccion = totalOpciones - 1;
                    break;

                case 80: // Flecha abajo
                    seleccion++;
                    if (seleccion >= totalOpciones)
                        seleccion = 0;
                    break;
                }
            }
            else if (tecla == 13) { // ENTER

                switch (seleccion) {

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
    }
};