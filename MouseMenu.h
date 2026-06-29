#pragma once
#include <windows.h>
#include <iostream>
#include <string>
#include <conio.h>
#include "gotoxy.h"
using namespace std;

static void habilitarMouse() {
    HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
    DWORD modo;
    GetConsoleMode(hIn, &modo);
    modo &= ~ENABLE_QUICK_EDIT_MODE;
    modo |= ENABLE_MOUSE_INPUT;
    SetConsoleMode(hIn, modo);
}

static int menuConMouse(string opciones[], int total, int centroX, int inicioY) {
    habilitarMouse();

    for (int i = 0; i < total; i++) {
        int posX = centroX - (int)(opciones[i].length() / 2);
        gotoXY(posX, inicioY + i);
        cout << opciones[i];
    }

    HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
    FlushConsoleInputBuffer(hIn);
    INPUT_RECORD reg;
    DWORD leido;

    while (true) {
        ReadConsoleInput(hIn, &reg, 1, &leido);

        if (reg.EventType == MOUSE_EVENT) {
            MOUSE_EVENT_RECORD m = reg.Event.MouseEvent;
            if (m.dwEventFlags == 0 && (m.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED)) {
                int y = m.dwMousePosition.Y;
                if (y >= inicioY && y < inicioY + total) {
                    return y - inicioY;
                }
            }
        }
    }
}


static bool leerCampo(string& resultado, bool ocultar = false) {
    resultado = "";
    int ch;
    while (true) {
        ch = _getch();
        if (ch == 27) return false;
        if (ch == '\r') return true;
        if (ch == '\b') {
            if (resultado.length() > 0) {
                resultado.pop_back();
                cout << "\b \b";
            }
        }
        else if (ch == 0 || ch == 224) {
            _getch();                     
        }
        else if (ch >= 32) {             
            resultado += (char)ch;
            cout << (ocultar ? '*' : (char)ch);
        }
    }
}


static bool leerEntero(int& resultado) {
    string texto;
    if (!leerCampo(texto)) return false;
    resultado = atoi(texto.c_str());
    return true;
}


static bool leerDecimal(double& resultado) {
    string texto;
    if (!leerCampo(texto)) return false;
    resultado = atof(texto.c_str());
    return true;
}
