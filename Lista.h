#pragma once
#include <functional>

typedef unsigned int uint;
using namespace std;

template <typename T>
class Lista {
private:
    struct Nodo {
        T elem;
        Nodo* sig;
        Nodo(T elem, Nodo* sig = nullptr) : elem(elem), sig(sig) {}
    };

    typedef function<int(T, T)> Comp;

    Nodo* ini;
    uint lon;
    Comp comparar;

public:
    Lista() : ini(nullptr), lon(0) {};
    Lista(Comp comparar) : ini(nullptr), lon(0), comparar(comparar) {}

    ~Lista() {
        Nodo* aux = ini;
        while (aux != nullptr) {
            ini = ini->sig;
            delete aux;
            aux = ini;
        }
    }

    uint longitud();
    bool esVacia();
    void agregaInicial(T elem);
    void agregarPos(T elem, uint pos);
    void agregaFinal(T elem);
    void modificarInicial(T elem);
    void modificarPos(T elem, uint pos);
    void modificarFinal(T elem);
    void eliminaInicial();
    void eliminaPos(uint pos);
    void eliminaFinal();
    T obtenerInicial();
    T obtenerPos(uint pos);
    T obtenerFinal();
    T buscar(T elem);
};

template <typename T>
uint Lista<T>::longitud() {
    return lon;
}

template <typename T>
bool Lista<T>::esVacia() {
    return lon == 0;
}

template <typename T>
void Lista<T>::agregaInicial(T elem) {
    Nodo* nuevo = new Nodo(elem, ini);
    if (nuevo != nullptr) {
        ini = nuevo;
        lon++;
    }
}

template <typename T>
void Lista<T>::agregarPos(T elem, uint pos) {
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

template <typename T>
void Lista<T>::agregaFinal(T elem) {
    agregarPos(elem, lon);
}

template <typename T>
void Lista<T>::modificarInicial(T elem) {
    if (lon > 0) {
        ini->elem = elem;
    }
}

template <typename T>
void Lista<T>::modificarPos(T elem, uint pos) {
    if (pos >= 0 && pos < lon) {
        Nodo* aux = ini;
        for (int i = 0; i < pos; i++) {
            aux = aux->sig;
        }
        aux->elem = elem;
    }
}

template <typename T>
void Lista<T>::modificarFinal(T elem) {
    modificarPos(elem, lon - 1);
}

template <typename T>
void Lista<T>::eliminaInicial() {
    if (lon > 0) {
        Nodo* aux = ini;
        ini = ini->sig;
        delete aux;
        lon--;
    }
}

template <typename T>
void Lista<T>::eliminaPos(uint pos) {
    if (pos >= lon) return;
    if (pos == 0)
        eliminaInicial();
    else {
        Nodo* aux = ini;
        Nodo* aux2 = ini;
        for (int i = 0; i != pos; i++) {
            aux2 = aux;
            aux = aux->sig;
        }
        aux2->sig = aux->sig;
        aux->sig = nullptr;
        delete aux;
        lon--;
    }
}

template <typename T>
void Lista<T>::eliminaFinal() {
    if (lon > 0) eliminaPos(lon - 1);
}

template <typename T>
T Lista<T>::obtenerInicial() {
    return obtenerPos(0);
}

template <typename T>
T Lista<T>::obtenerPos(uint pos) {
    if (pos >= 0 && pos < lon) {
        Nodo* aux = ini;
        for (int i = 0; i < pos; i++) {
            aux = aux->sig;
        }
        return aux->elem;
    }
    else {
        return T();
    }
}

template <typename T>
T Lista<T>::obtenerFinal() {
    return obtenerPos(lon - 1);
}

template <typename T>
T Lista<T>::buscar(T elem) {
    Nodo* aux = ini;
    while (aux != nullptr) {

        if (comparar && comparar(aux->elem, elem) == 0) {
            return aux->elem;
        }
        aux = aux->sig;
    }
    return T();
}