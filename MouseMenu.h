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

// Muestra las opciones centradas en la consola y espera un clic del mouse.
// centroX: columna central (usa 60 para consola de 120).
// inicioY: fila donde empieza la primera opcion.
// Retorna el indice de la opcion clickeada (0-based).
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

// Lee un campo de texto caracter por caracter.
// Retorna false si el usuario presiona ESC, true si presiona Enter.
// Si ocultar=true muestra '*' en lugar del caracter (para contrasenas).
static bool leerCampo(string& resultado, bool ocultar = false) {
    resultado = "";
    int ch;
    while (true) {
        ch = _getch();
        if (ch == 27) return false;      // ESC -> cancelar
        if (ch == '\r') return true;     // Enter -> confirmar
        if (ch == '\b') {                // Backspace
            if (resultado.length() > 0) {
                resultado.pop_back();
                cout << "\b \b";
            }
        }
        else if (ch == 0 || ch == 224) { // tecla extendida (flechas, etc.)
            _getch();                     // consumir segundo byte
        }
        else if (ch >= 32) {             // caracter imprimible
            resultado += (char)ch;
            cout << (ocultar ? '*' : (char)ch);
        }
    }
}

// Lee un numero entero. Retorna false si el usuario presiona ESC.
static bool leerEntero(int& resultado) {
    string texto;
    if (!leerCampo(texto)) return false;
    resultado = atoi(texto.c_str());
    return true;
}

// Lee un numero decimal. Retorna false si el usuario presiona ESC.
static bool leerDecimal(double& resultado) {
    string texto;
    if (!leerCampo(texto)) return false;
    resultado = atof(texto.c_str());
    return true;
}
