#ifndef __NODOAVL_HPP__
#define __NODOAVL_HPP__

template<class T>
class NodoAVL
{
public:
    T elemento;
    NodoAVL<T>* izq;
    NodoAVL<T>* der;
    int altura;

    NodoAVL() : elemento(), izq(nullptr), der(nullptr), altura(0) {}
    explicit NodoAVL(const T& elem) : elemento(elem), izq(nullptr), der(nullptr), altura(1) {}
};

#endif // __NODOAVL_HPP__