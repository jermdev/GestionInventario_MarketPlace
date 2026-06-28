#pragma once
#include "Lista.h"
#include "ComponenteUI.h"
#include <iostream>
#include <windows.h>
#include "gotoxy.h"

template <typename T>
class GridView {
private:
    int filas = 1;
    int columnas = 1;
    int selecionado = -1;
    Lista<ComponenteUI<T>*>* elementos = nullptr;
    int x = 0; int y = 0; int alto = 0; int ancho = 0;
    int anchoItem = 1;
    int altoItem = 1;
    int espacioX = 1;
    int espacioY = 1;
    int paginaActual = 0;

    void calculateGridGeometry() {
        if (anchoItem <= 0) anchoItem = 1;
        if (altoItem <= 0) altoItem = 1;

        int denomX = anchoItem + espacioX;
        columnas = (denomX > 0 && (ancho / denomX) > 0) ? (ancho / denomX) : 1;

        int denomY = altoItem + espacioY;
        filas = (denomY > 0 && ((alto - 2) / denomY) > 0) ? ((alto - 2) / denomY) : 1;
    }

    // Validar que la posición esté dentro del buffer (sin algorithm)
    bool isPositionValid(int px, int py) {
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
        if (h == INVALID_HANDLE_VALUE) return false;
        if (!GetConsoleScreenBufferInfo(h, &csbi)) return false;

        int maxX = csbi.srWindow.Right - csbi.srWindow.Left;
        int maxY = csbi.srWindow.Bottom - csbi.srWindow.Top;
        return (px >= 0 && px <= maxX && py >= 0 && py <= maxY);
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
        espacioY(espacioY),
        paginaActual(0)
    {
        calculateGridGeometry();
    }

    ~GridView() { }

    // Calcular total de páginas sin algorithm (usar ternario)
    int getTotalPages() {
        if (!elementos || elementos->longitud() == 0) return 1;
        int totalCellsPerPage = filas * columnas;
        if (totalCellsPerPage <= 0) totalCellsPerPage = 1;
        // Equivalente a: (n + perPage - 1) / perPage
        return (elementos->longitud() + totalCellsPerPage - 1) / totalCellsPerPage;
    }

    void siguientePagina() {
        int totalPages = getTotalPages();
        if (paginaActual < totalPages - 1) paginaActual++;
    }

    void paginaAnterior() {
        if (paginaActual > 0) paginaActual--;
    }

    int getPaginaActual() { return paginaActual; }
    void setPaginaActual(int p) { paginaActual = p; }

    template <typename C>
    void mostrarGrid(C card) {
        if (!elementos) return;
        int n = elementos->longitud();
        if (n <= 0) return;

        calculateGridGeometry();

        int totalCellsPerPage = filas * columnas;
        if (totalCellsPerPage <= 0) return;

        int startIdx = paginaActual * totalCellsPerPage;
        // Sin std::min: usar ternario
        int endIdx = (startIdx + totalCellsPerPage < n) ? (startIdx + totalCellsPerPage) : n;

        for (int i = startIdx; i < endIdx; ++i) {
            int indexInPage = i - startIdx;
            int fila = indexInPage / columnas;
            int columna = indexInPage % columnas;

            int posX = x + columna * (anchoItem + espacioX);
            int posY = y + fila * (altoItem + espacioY);

            if (!isPositionValid(posX, posY)) continue;

            ComponenteUI<T>* e = elementos->obtenerPos(i);
            if (!e) continue;

            // callable con validación de bounds
            auto posicionFn = [this, posX, posY](int lineOffset) {
                int checkY = posY + lineOffset;
                if (isPositionValid(posX, checkY)) {
                    std::cout.flush();
                    gotoXY(posX, checkY);
                }
            };

            e->template render<C>(card, posicionFn);
        }
    }
};