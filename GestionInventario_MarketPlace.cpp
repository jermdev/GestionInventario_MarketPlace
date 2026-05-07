#include "pch.h"
#include "GestorMenu.h"
#include "AuthUI.h"

using namespace System;

void main() {
    /*int opcion;

    do {
        opcion = GestorMenus::menuPrincipal();

        switch (opcion) {
        case 1:
            cout << "Login...\n";
            break;
        case 2:
            cout << "Registro...\n";
            break;
        case 0:
            cout << "Saliendo...\n";
            break;
        }

    } while (opcion != 0);*/
    AuthUI::Render();

}
