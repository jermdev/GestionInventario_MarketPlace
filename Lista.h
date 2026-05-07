#pragma once
#include "Nodo.h"
#include <functional>
typedef unsigned int uint;
using namespace std;

template <typename T, T NADA = 0>
class Lista {
    struct Nodo;
    typedef function<int(T, T)> Comp;

    Nodo* ini;
    uint    lon; // n�mero de elementos en la lista

    Comp    comparar; // lambda de criterio de comparaci�n

public:
    Lista() : ini(nullptr), lon(0), comparar([](T a, T b) {return a - b; }) {};
    Lista(Comp comparar) : ini(nullptr), lon(0), comparar(comparar) {}
    ~Lista();

    uint    longitud();

    bool    esVacia();

    void    agregaInicial(T elem);
    void    agregarPos(T elem, uint pos);
    void    agregaFinal(T elem);

    void    modificarInicial(T elem);
    void    modificarPos(T elem, uint pos);
    void    modificarFinal(T elem);

    void    eliminaInicial();
    void    eliminaPos(uint pos);
    void    eliminaFinal();

    T       obtenerInicial();
    T       obtenerPos(uint pos);
    T       obtenerFinal();

    T       buscar(T elem);
};

template <typename T, T NADA>
struct Lista<T, NADA>::Nodo {
    T       elem;
    Nodo* sig; // puntero apunta al siguiente nodo

    Nodo(T elem = NADA, Nodo* sig = nullptr) : elem(elem), sig(sig) {}
};

template <typename T, T NADA>
Lista<T, NADA>::~Lista() {
    while (ini != nullptr) {
        Nodo* aux = ini;
        ini = ini->sig;
        delete aux;
    }
}

template <typename T, T NADA>
uint Lista<T, NADA>::longitud() {
    return lon;
}

template <typename T, T NADA>
bool Lista<T, NADA>::esVacia() {
    return lon == 0;
}

template <typename T, T NADA>
void Lista<T, NADA>::agregaInicial(T elem) {
    Nodo* nuevo = new Nodo(elem, ini); //constructor, env�o elemento y dirreci�n de memoria del ini
    if (nuevo != nullptr) {
        ini = nuevo;
        lon++;
    }
}

template <typename T, T NADA>
void Lista<T, NADA>::agregarPos(T elem, uint pos) {
    if (pos > lon) return;
    if (pos == 0) {
        agregaInicial(elem);
    }
    else {
        Nodo* aux = ini;
        for (int i = 1; i < pos; i++) {
            aux = aux->sig;
        }
        Nodo* nuevo = new Nodo(elem, aux->sig);
        if (nuevo != nullptr) {
            aux->sig = nuevo;
            lon++;
        }
    }
}
template <typename T, T NADA>
void Lista<T, NADA>::agregaFinal(T elem) {
    agregarPos(elem, lon); // ;)
}

template <typename T, T NADA>
void Lista<T, NADA>::modificarInicial(T elem) {
    if (lon > 0) {
        ini->elem = elem;
    }
}
template <typename T, T NADA>
void Lista<T, NADA>::modificarPos(T elem, uint pos) {
    if (pos >= 0 && pos < lon) {
        Nodo* aux = ini; //variable tipo nodo con el valor del nodo ini
        for (int i = 0; i < pos; i++) {
            aux = aux->sig; //asigno la direcci�n de memoria del siguiente nodo
        }
        aux->elem = elem;
    }
}
template <typename T, T NADA>
void Lista<T, NADA>::modificarFinal(T elem) {
    modificarPos(elem, lon - 1);
}

template <typename T, T NADA>
void Lista<T, NADA>::eliminaInicial() {
    if (lon > 0) {
        Nodo* aux = ini;
        ini = ini->sig;
        delete aux;
        lon--;
    }
}

//Probando...
template <typename T, T NADA>
void Lista<T, NADA>::eliminaPos(uint pos) {

    //Revisar esto
    if (pos > lon) return;
    if (pos == 0)
        eliminaInicial();

    else {
        Nodo* aux = ini;
        Nodo* aux2 = ini;
        for (int i = 0; i != pos; i++)
        {
            aux2 = aux;
            aux = aux->sig;
        }
        aux2 = aux->sig;
        aux->sig = nullptr;
        delete aux;
        lon--;
    }
}

template <typename T, T NADA>
void Lista<T, NADA>::eliminaFinal() {
    eliminaPos(lon - 1);
}

template <typename T, T NADA>
T Lista<T, NADA>::obtenerInicial() {
    return obtenerPos(0); //return ini->elem;
}
template <typename T, T NADA>
T Lista<T, NADA>::obtenerPos(uint pos) {
    if (pos >= 0 && pos < lon) {
        Nodo* aux = ini;
        for (int i = 0; i < pos; i++) {
            aux = aux->sig;
        }
        return aux->elem;
    }
    else {
        return NADA;
    }
}
template <typename T, T NADA>
T Lista<T, NADA>::obtenerFinal() {
    return obtenerPos(lon - 1);
}

template <typename T, T NADA>
T Lista<T, NADA>::buscar(T elem) {
    Nodo* aux = ini;
    while (aux != nullptr) {
        if (comparar(aux->elem, elem) == 0) {
            return aux->elem;
        }
        aux = aux->sig;
    }
    return NADA;
}

