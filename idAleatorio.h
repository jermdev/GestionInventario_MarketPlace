#pragma once
#include <ctime>
#include <cstdlib> 

int idAleatorioPorDigitos(int numDigitos) {
    if (numDigitos <= 0) return 0;

    static bool inicializado = false;
    if (!inicializado) {
        srand(time(nullptr));
        inicializado = true;
    }

    int minimo = 1;
    for (int i = 1; i < numDigitos; i++) {
        minimo *= 10;
    }

    int maximo = minimo * 10 - 1;

    return minimo + rand() % (maximo - minimo + 1);
}