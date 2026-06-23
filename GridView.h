#pragma once
#include "Lista.h"
#include "ComponenteUI.h"
#include <windows.h>
#include <iostream>
template <typename T>
class GridView {
private:
	int filas, columnas, selecionado;
	Lista<ComponenteUI<T>*>* elementos;
    int x; int y; int alto; int ancho;
    int anchoItem;
    int altoItem;
    int espacioX;
    int espacioY;

    static void gotoXY(int x, int y) {
        COORD coord;
        coord.X = x;
        coord.Y = y;

        SetConsoleCursorPosition(
            GetStdHandle(STD_OUTPUT_HANDLE),
            coord
        );
    }

     void calculateGridGeometry() {
        if (anchoItem <= 0) anchoItem = 1;
        if (altoItem <= 0) altoItem = 1;

        int denomX = anchoItem + espacioX;
        columnas = (denomX > 0 && (ancho / denomX) > 0) ? (ancho / denomX) : 1;

        int denomY = altoItem + espacioY;
        filas = (denomY > 0 && (alto / denomY) > 0) ? (alto / denomY) : 1;
    }
public:
	GridView(
        Lista<ComponenteUI<T>*>* elementos,
        int x,
        int y,
        int ancho,
        int alto,
        int anchoItem,
        int altoItem,
        int espacioX = 2,
        int espacioY = 1) :
        elementos(elementos),
        x(x),
        y(y),
        ancho(ancho),
        alto(alto),
        anchoItem(anchoItem),
        altoItem(altoItem),
        espacioX(espacioX),
        espacioY(espacioY)
    {
    }

	~GridView() {

	}

    template <typename C>
	void mostrarGrid(C card) {

        if (!elementos) return;
        int n = elementos->longitud();
        if (n <= 0) return;

        // Recalcular en cada render por si cambió tamaño/params
        calculateGridGeometry();

        for (int i = 0; i < n; ++i) {
            int fila = i / columnas;
            int columna = i % columnas;

            int posX = x + columna * (anchoItem + espacioX);
            int posY = y + fila * (altoItem + espacioY);

            ComponenteUI<T>* e = elementos->obtenerPos(i);
            if (!e) continue;

            // callable que mueve el cursor al inicio de la "celda"
            auto posicionFn = [posX, posY](int fila) { gotoXY(posX, posY+fila); };

            // llamar al renderer del componente pasando la función de posicion
            e->template render<C>(card, posicionFn);
        }
        
	}
};