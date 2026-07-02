#ifndef __ARBOLAVL_HPP__
#define __ARBOLAVL_HPP__

#include "NodoAVL.hpp"


template<class T, class Comparar = int(*)(T, T)>
class ArbolAVL
{
private:
    NodoAVL<T>* raiz;
    void(*procesar)(T);            // callback para procesar elementos (T)

    Comparar comparar;

    T* _buscar(NodoAVL<T>* nodo, T e)
    {
        if (nodo == nullptr) return nullptr;

        int rel = comparar(e, nodo->elemento);

        if (rel == 0)
        {
            return &(nodo->elemento);
        }
        else if (rel < 0)
        {
            return _buscar(nodo->izq, e);
        }
        else
        {
            return _buscar(nodo->der, e);
        }
    }

    //Operaciones privadas
    int _altura(NodoAVL<T>* nodo)
    {
        if (nodo == nullptr) return 0;
        return nodo->altura;
    }

    void _rotarDerecha(NodoAVL<T>*& nodo)
    {
        NodoAVL<T>* p = nodo->izq;
        nodo->izq = p->der;
        p->der = nodo;

        int hizq = _altura(nodo->izq);
        int hder = _altura(nodo->der);
        nodo->altura = 1 + ((hizq > hder) ? hizq : hder);

        hizq = _altura(p->izq);
        hder = _altura(p->der);
        p->altura = 1 + ((hizq > hder) ? hizq : hder);

        nodo = p;
    }

    void _rotarIzquierda(NodoAVL<T>*& nodo)
    {
        NodoAVL<T>* p = nodo->der;
        nodo->der = p->izq;
        p->izq = nodo;

        int hizq = _altura(nodo->izq);
        int hder = _altura(nodo->der);
        nodo->altura = 1 + ((hizq > hder) ? hizq : hder);

        hizq = _altura(p->izq);
        hder = _altura(p->der);
        p->altura = 1 + ((hizq > hder) ? hizq : hder);

        nodo = p;
    }

    bool _actualizar(NodoAVL<T>* nodo, T e)
    {
        // Busca el nodo cuya clave coincide (segun 'comparar') y reemplaza su elemento
        if (nodo == nullptr) return false;
        int rel = comparar(e, nodo->elemento);
        if (rel == 0)
        {
            nodo->elemento = e;
            return true;
        }
        else if (rel < 0)
        {
            return _actualizar(nodo->izq, e);
        }
        else
        {
            return _actualizar(nodo->der, e);
        }
    }

    bool _eliminar(NodoAVL<T>*& nodo, T e) {
        if (nodo == nullptr) return false;

        bool eliminado = false;
        int rel = comparar(e, nodo->elemento);

        if (rel < 0)
        {
            eliminado = _eliminar(nodo->izq, e);
        }
        else if (rel > 0)
        {
            eliminado = _eliminar(nodo->der, e);
        }
        else // rel == 0, encontrado
        {
            eliminado = true;
            if (nodo->izq == nullptr || nodo->der == nullptr)
            {
                NodoAVL<T>* hijo = (nodo->izq != nullptr) ? nodo->izq : nodo->der;
                NodoAVL<T>* viejo = nodo;
                nodo = hijo; // puede quedar nullptr
                delete viejo;
            }
            else
            {
                // Reemplazar por el sucesor (mínimo del subárbol derecho)
                NodoAVL<T>* sucesor = _minimo(nodo->der);
                nodo->elemento = sucesor->elemento;
                _eliminar(nodo->der, sucesor->elemento);
            }
        }

        if (nodo != nullptr)
            _balanceo(nodo);

        return eliminado;
    }

    NodoAVL<T>* _minimo(NodoAVL<T>* nodo) {
        // Nodo más a la izquierda (el menor) de un subárbol
        while (nodo->izq != nullptr)
            nodo = nodo->izq;
        return nodo;
    }

    void _balanceo(NodoAVL<T>*& nodo)
    {
        int hizq = _altura(nodo->izq);
        int hder = _altura(nodo->der);
        int fb = hder - hizq;

        if (fb > 1)
        { //rotar a la izq
            int hhizq = _altura(nodo->der->izq);
            int hhder = _altura(nodo->der->der);
            if (hhizq > hhder) {
                _rotarDerecha(nodo->der);
            }
            _rotarIzquierda(nodo);
        }
        if (fb < -1)
        { //rotar a la der
            int hhizq = _altura(nodo->izq->izq);
            int hhder = _altura(nodo->izq->der);
            if (hhizq < hhder) {
                _rotarIzquierda(nodo->izq);
            }
            _rotarDerecha(nodo);
        }
        hizq = _altura(nodo->izq);
        hder = _altura(nodo->der);
        nodo->altura = 1 + ((hizq > hder) ? hizq : hder);
    }

    void _obtenerElementos(NodoAVL<T>* nodo, std::vector<T>& resultado)
    {
        if (nodo == nullptr) return;
        _obtenerElementos(nodo->izq, resultado);
        resultado.push_back(nodo->elemento);
        _obtenerElementos(nodo->der, resultado);
    }

    bool _insertar(NodoAVL<T>*& nodo, T e)
    {
        if (nodo == nullptr)
        {
            nodo = new NodoAVL<T>(e);
            nodo->altura = 1;
            return true;
        }
        else if (comparar(e, nodo->elemento) == 0)
            return false;
        else if (comparar(e, nodo->elemento) < 0)
        {
            _insertar(nodo->izq, e);
        }
        else // > 0
        {
            _insertar(nodo->der, e);
        }
        _balanceo(nodo);
        return true;
    }

    void _inOrden(NodoAVL<T>* nodo)
    {
        if (nodo == nullptr) return;
        _inOrden(nodo->izq);
        if (procesar) procesar(nodo->elemento);
        _inOrden(nodo->der);
    }

    void _inOrdenH(NodoAVL<T>* nodo)
    {
        if (nodo == nullptr) return;
        _inOrdenH(nodo->izq);
        // solo llamar al callback de alturas si fue provisto (evita C2664)
        _inOrdenH(nodo->der);
    }

public:
    // Constructor principal: callback para elementos y comparador; procesarAltura es opcional (puedes pasar nullptr)
    ArbolAVL(void(*nuevaFuncion)(T), Comparar nuevoCriterio)
    {
        this->procesar = nuevaFuncion;
        this->comparar = nuevoCriterio;
        this->raiz = nullptr;
    }

    bool Insertar(T e)
    {
        return _insertar(raiz, e);
    }
    T* Buscar(T e)
    {
        return _buscar(raiz, e);
    }

    bool Eliminar(T e)
    {
        return _eliminar(raiz, e);
    }

    std::vector<T> ObtenerElementos()
    {
        std::vector<T> resultado;
        _obtenerElementos(raiz, resultado);
        return resultado;
    }

    bool Actualizar(T e)
    {
        return _actualizar(raiz, e);
    }
    void inOrden()
    {
        _inOrden(raiz);
    }
    void inOrdenH()
    {
        _inOrdenH(raiz);
    }
};

#endif // __ARBOLAVL_HPP__