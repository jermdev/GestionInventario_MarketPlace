#ifndef __HASHTABLA_HPP__
#define __HASHTABLA_HPP__
#include <iostream>
#include <string>
#include <sstream>
#include "HashEntidad.hpp"
#include "Lista.h"

using namespace std;

template <typename K, typename V>
class HashTabla {
private:
	Lista<HashEntidad<K, V>>* tabla;

	int numElementos;
	int TABLE_SIZE;

public:
	HashTabla(int TABLE_SIZE = 128) {
		this->TABLE_SIZE = TABLE_SIZE;
		tabla = new Lista<HashEntidad<K, V>>[TABLE_SIZE];
		numElementos = 0;
	}

	~HashTabla() {
		delete[] tabla;
	}


	int funcionHash(K key) {

		stringstream ss;
		ss << key;
		string texto = ss.str();

		int sumaMatematica = 0;
		for (char letra : texto) {
			sumaMatematica += (int)letra;
		}

		return sumaMatematica % TABLE_SIZE;
	}

	void insertar(K key, V value) {
		int indice = funcionHash(key);
		HashEntidad<K, V> nuevaEntidad(key, value);

		int cantidadEnLista = tabla[indice].longitud();

		if (cantidadEnLista == 0) {

			tabla[indice].agregaInicial(nuevaEntidad);
		}
		else {

			int mitad = cantidadEnLista / 2;
			tabla[indice].agregarPos(nuevaEntidad, mitad);
		}
		numElementos++;
	}

	int size() {
		return TABLE_SIZE;
	}

	int sizeactual() {
		return numElementos;
	}

	V buscar(K key) {
		int indice = funcionHash(key);
		int cantidadEnLista = tabla[indice].longitud();

		for (int i = 0; i < cantidadEnLista; i++) {
			HashEntidad<K, V> actual = tabla[indice].obtenerPos(i);
			if (actual.getKey() == key) {
				return actual.getValue();
			}
		}
		return V();
	}
	
	template <typename Condicion>
	void eliminarValor(Condicion cond) {
		for (int i = 0; i < TABLE_SIZE; i++) {
			if (!tabla[i].esVacia()) {
				for (int j = 0; j < tabla[i].longitud(); j++) {
					HashEntidad<K, V> actual = tabla[i].obtenerPos(j);
					if (cond(actual.getValue())) {

						tabla[i].eliminaPos(j);
						numElementos--;
					
						return;
					}
				}
			}
		}
	}
	template<typename F>
	void mostrar(F condicion) {
		for (int i = 0; i < TABLE_SIZE; i++) {
			if (!tabla[i].esVacia()) {
				for (int j = 0; j < tabla[i].longitud(); j++) {
					HashEntidad<K, V> actual = tabla[i].obtenerPos(j);
					condicion(actual.getKey(),actual.getValue());
				}
			}
		}
	}
};

#endif // !__HASHTABLA_HPP__