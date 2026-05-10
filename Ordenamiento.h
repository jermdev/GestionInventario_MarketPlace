#pragma once
#include "Lista.h"
#include "Producto.h"
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
