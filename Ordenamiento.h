#pragma once
#include <iostream>
#include "Lista.h"
#include "Producto.h"

using namespace std;

void insertionSortListaPorPrecio(Lista<Producto*>* list, bool asc) {
    int n = list->longitud();
    for (int i = 1; i < n; ++i) {
        Producto* key = list->obtenerPos(i);
        int j = i - 1;

        // desplazar elementos mayores (o menores si desc) hacia la derecha
        while (j >= 0) {
            Producto* current = list->obtenerPos(j);
            if (current == nullptr || key == nullptr) break;

            bool condition;
            if (asc) condition = (current->getPrecio() > key->getPrecio());
            else      condition = (current->getPrecio() < key->getPrecio());

            if (condition) {
                // mover current a la posición j+1
                list->modificarPos(current, j + 1);
                --j;
            }
            else {
                break;
            }
        }
        // insertar key en la posición j+1
        list->modificarPos(key, j + 1);
    }
}

int particion(Producto** A, int p, int r) {
    double x = A[r]->getPrecio(); // El pivote ahora es el precio del producto
    int i = p - 1;
    for (int j = p; j < r; j++) {
        if (A[j]->getPrecio() <= x) {
            i++;
            swap(A[i], A[j]);
        }
    }
    swap(A[i + 1], A[r]);
    return i + 1;
}

void quicksort(Producto** A, int p, int r) {
    int q;
    if (p < r) {
        q = particion(A, p, r);
        quicksort(A, p, q - 1);
        quicksort(A, q + 1, r);
    }
}

void Merge(Producto** A1, Producto** A2, Producto** A, int n) {
    int i = 0, j = 0, k = 0;
    int mitad = n / 2;
    while (i < mitad && j < n - mitad) {
        // Comparamos los Nombres alfabéticamente
        if (A1[i]->getNombre() < A2[j]->getNombre()) {
            A[k] = A1[i];
            i++; k++;
        }
        else {
            A[k] = A2[j];
            j++; k++;
        }
    }
    while (i < mitad) { A[k] = A1[i]; i++; k++; }
    while (j < n - mitad) { A[k] = A2[j]; j++; k++; }
}

void mergeSort(Producto** A, int n) {
    if (n > 1) {
        int mitad = n / 2;
        Producto** A1 = new Producto * [mitad];
        Producto** A2 = new Producto * [n - mitad];

        for (int i = 0; i < mitad; i++) A1[i] = A[i];
        for (int i = mitad; i < n; i++) A2[i - mitad] = A[i];

        mergeSort(A1, mitad);
        mergeSort(A2, n - mitad);
        Merge(A1, A2, A, n);

        // Evita fugas de memoria borrando los arreglos temporales
        delete[] A1;
        delete[] A2;
    }
}