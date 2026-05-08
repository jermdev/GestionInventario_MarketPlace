#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "GestorDeArchivos.h"
#include "Pedido.h"
#include "Lista.h"

using namespace std;

class PedidoService {
private:
    GestorDeArchivos gestor;
    const string ARCHIVO_PEDIDOS = "pedidos.txt";
    const char DELIMITADOR = '|';

public:
    PedidoService() {}

    void registrarPedido(Pedido* pedido) {
        vector<vector<string>> dataExistente;

        if (gestor.archivoExiste(ARCHIVO_PEDIDOS)) {
            dataExistente = gestor.leerLineasString(ARCHIVO_PEDIDOS, DELIMITADOR);
        }

        vector<string> nuevaFila;
        nuevaFila.push_back(to_string(pedido->getIdPedido()));
        nuevaFila.push_back(to_string(pedido->getPeso()));
        nuevaFila.push_back(to_string(pedido->getEstadoPedido()));

        int cantidadProductos = pedido->getProductosComprados()->longitud();
        nuevaFila.push_back(to_string(cantidadProductos));

        dataExistente.push_back(nuevaFila);
        gestor.guardarLineas(ARCHIVO_PEDIDOS, dataExistente, DELIMITADOR);
    }

    Lista<Pedido*>* obtenerPedidosHistoricos() {
        Lista<Pedido*>* listaPedidos = new Lista<Pedido*>();

        if (!gestor.archivoExiste(ARCHIVO_PEDIDOS)) {
            return listaPedidos;
        }

        vector<vector<string>> data = gestor.leerLineasString(ARCHIVO_PEDIDOS, DELIMITADOR);

        for (int i = 0; i < data.size(); i++) {
            if (data[i].size() >= 3) {

                int id = stoi(data[i][0]);
                double peso = stod(data[i][1]);
                EstadoPedido estado = static_cast<EstadoPedido>(stoi(data[i][2]));

                Lista<Producto*>* productosDelPedido = new Lista<Producto*>();

                Pedido* p = new Pedido(id, peso, estado, productosDelPedido);

                listaPedidos->agregaFinal(p);
            }
        }

        return listaPedidos;
    }
};