#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "GestorDeArchivos.h"
#include "Pedido.h"
#include "Lista.h"
#include "fechas.h"

using namespace std;

class PedidoService {
private:
    Lista<Pedido*>* pedidos;
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
        nuevaFila.push_back(to_string(static_cast<int>(pedido->getEstadoPedido())));
        // fechaEntrega
        nuevaFila.push_back(pedido->getFechaEntrega());

        // cantidad de líneas (número de NProductos distintos)
        int lineasProductos = pedido->getProductosComprados()->longitud();
        nuevaFila.push_back(to_string(lineasProductos));

        // pares id:cantidad separados por comas
        string pares = "";
        for (int i = 0; i < lineasProductos; ++i) {
            NProductos* np = pedido->getProductosComprados()->obtenerPos(i);
            if (np == nullptr) continue;
            if (!pares.empty()) pares += ",";
            pares += to_string(np->producto ? np->producto->getId() : 0);
            pares += ":";
            pares += to_string(np->cantidad);
        }
        nuevaFila.push_back(pares);

        dataExistente.push_back(nuevaFila);
        gestor.guardarLineas(ARCHIVO_PEDIDOS, dataExistente, DELIMITADOR);
    }


    EstadoPedido estadoPedido(string fechaEntrega, EstadoPedido estadoActual) {
        string hoy = obtenerFechaActual();
        if (estadoActual == EstadoPedido::CANCELADO) return estadoActual;

        if (hoy >= fechaEntrega) {
            estadoActual = EstadoPedido::ENTREGADO;
        }
        else if (hoy < fechaEntrega) {
            estadoActual = EstadoPedido::PENDIENTEDEENTREGA;
        }

        return estadoActual;
    }

    void setPedidos(Lista<Pedido*>* pedidos) { this->pedidos = pedidos; }

    // Reconstruye pedidos historicos: usa ProductoService para obtener Producto* por id y preserva cantidad
    template<class T>
    Lista<Pedido*>* obtenerPedidosPorCondicion(int indice,T condicion) {
        if (indice == pedidos->longitud()) {
            return new Lista<Pedido*>();
        }

        Lista<Pedido*>* resultado = obtenerPedidosPorCondicion(indice + 1, condicion);


        Pedido* p = pedidos->obtenerPos(indice);
        if (condicion(p)) {
            resultado->agregaInicial(p);
        }

        return resultado;
    }

    Lista<Pedido*>* obtenerPedidosHistoricos() {

        Lista<Pedido*>* listaPedidos = new Lista<Pedido*>();

        if (!gestor.archivoExiste(ARCHIVO_PEDIDOS)) {
            return listaPedidos;
        }

        // Inicializar productos existentes
        ProductoService productoService;
        productoService.iniciaizarProductos();

        vector<vector<string>> data =
            gestor.leerLineasString(ARCHIVO_PEDIDOS, DELIMITADOR);

        for (int i = 0; i < data.size(); i++) {

            if (data[i].size() >= 6) {

                int idPedido = stoi(data[i][0]);

                double peso = stod(data[i][1]);

                EstadoPedido estado =
                    static_cast<EstadoPedido>(stoi(data[i][2]));
                estado = estadoPedido(obtenerFechaActual(), estado);

                string fechaEntrega = data[i][3];

                string productosTexto = data[i][5];

                Lista<NProductos*>* productosDelPedido =
                    new Lista<NProductos*>();

                string temp = "";

                // Recorrer:
                // 121176:1,121177:2

                for (int j = 0; j <= productosTexto.size(); j++) {

                    if (j == productosTexto.size() ||
                        productosTexto[j] == ',') {

                        int pos = temp.find(':');

                        if (pos != string::npos) {

                            int idProducto =
                                stoi(temp.substr(0, pos));

                            int cantidad =
                                stoi(temp.substr(pos + 1));

                            // Buscar producto real
                            Producto* productoEncontrado =
                                productoService.buscarProductoPorId(
                                    0,
                                    [idProducto](Producto* p) {
                                        return p->getId() == idProducto;
                                    }
                                );

                            // Si existe
                            if (productoEncontrado != nullptr) {

                                NProductos* np = new NProductos();

                                np->producto = productoEncontrado;
                                np->cantidad = cantidad;

                                productosDelPedido->agregaFinal(np);
                            }
                        }

                        temp = "";
                    }
                    else {
                        temp += productosTexto[j];
                    }
                }

                Pedido* pedido = new Pedido(
                    idPedido,
                    peso,
                    estado,
                    productosDelPedido,
                    fechaEntrega
                );

                listaPedidos->agregaFinal(pedido);
            }
        }

        return listaPedidos;
    }
};