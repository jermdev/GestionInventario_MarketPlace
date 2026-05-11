#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "GestorDeArchivos.h"
#include "Pedido.h"
#include "Lista.h"
#include "fechas.h"
#include "ProductoService.h" // necesario para reconstruir productos

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

        // Validación
        if (pedido == nullptr) return;

        vector<vector<string>> dataExistente;

        // Leer archivo si existe
        if (gestor.archivoExiste(ARCHIVO_PEDIDOS)) {
            dataExistente =
                gestor.leerLineasString(
                    ARCHIVO_PEDIDOS,
                    DELIMITADOR
                );
        }

        vector<string> nuevaFila;

        // Formato elegido:
        // idPedido | idUsuario | peso | estado | fechaEntrega | lineasProductos | pares id:cantidad

        nuevaFila.push_back(to_string(pedido->getIdPedido()));
        nuevaFila.push_back(to_string(pedido->getIdCliente()));

        nuevaFila.push_back(
            to_string(pedido->getPeso())
        );

        nuevaFila.push_back(
            to_string(
                static_cast<int>(
                    pedido->getEstadoPedido()
                    )
            )
        );

        nuevaFila.push_back(
            pedido->getFechaEntrega()
        );

        // =========================
        // PRODUCTOS
        // =========================

        Lista<NProductos*>* productos =
            pedido->getProductosComprados();

        int cantidadProductos =
            productos->longitud();

        nuevaFila.push_back(
            to_string(cantidadProductos)
        );

        /*
            FORMATO:
            id:cantidad,id:cantidad

            Ejemplo:
            12:2,30:1,44:5
        */

        string productosTexto = "";

        for (int i = 0; i < cantidadProductos; i++) {

            NProductos* np =
                productos->obtenerPos(i);

            // Validaciones
            if (np == nullptr) continue;

            if (np->producto == nullptr) continue;

            if (np->cantidad <= 0) continue;

            // Separador coma
            if (!productosTexto.empty()) {
                productosTexto += ",";
            }

            productosTexto +=
                to_string(np->producto->getId());

            productosTexto += ":";

            productosTexto +=
                to_string(np->cantidad);
        }

        nuevaFila.push_back(productosTexto);

        // =========================
        // GUARDAR
        // =========================

        dataExistente.push_back(nuevaFila);

        gestor.guardarLineas(
            ARCHIVO_PEDIDOS,
            dataExistente,
            DELIMITADOR
        );
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

        // Inicializar productos existentes en un servicio local
        ProductoService productoService;
        productoService.iniciaizarProductos();

        vector<vector<string>> data =
            gestor.leerLineasString(ARCHIVO_PEDIDOS, DELIMITADOR);

        for (int i = 0; i < (int)data.size(); i++) {

            // Validar columnas: esperamos 7 campos
            // [0]=idPedido, [1]=idUsuario, [2]=peso, [3]=estado, [4]=fechaEntrega, [5]=lineasProductos, [6]=pares id:cantidad
            if (data[i].size() >= 7) {

                int idPedido = stoi(data[i][0]);
                int idUsuario = stoi(data[i][1]);
                double peso = stod(data[i][2]);
                EstadoPedido estado = static_cast<EstadoPedido>(stoi(data[i][3]));
                string fechaEntrega = data[i][4];
                // data[i][5] = cantidad de líneas (opcional)
                string productosTexto = data[i][6];

                // Actualizar estado automáticamente
                estado = estadoPedido(fechaEntrega, estado);

                Lista<NProductos*>* productosDelPedido =
                    new Lista<NProductos*>();

                // =========================
                // RECONSTRUIR PRODUCTOS
                // Formato:
                // 121176:1,121177:2
                // =========================

                string temp = "";

                for (int j = 0; j <= (int)productosTexto.size(); j++) {

                    if (j == (int)productosTexto.size() ||
                        productosTexto[j] == ',') {

                        if (!temp.empty()) {
                            int pos = temp.find(':');

                            if (pos != string::npos) {

                                int idProducto =
                                    stoi(temp.substr(0, pos));

                                int cantidad =
                                    stoi(temp.substr(pos + 1));

                                // Buscar producto real en el servicio inicializado
                                Producto* productoEncontrado =
                                    productoService.buscarProductoPorId(
                                        0,
                                        [idProducto](Producto* p) {
                                            return p->getId() == idProducto;
                                        }
                                    );

                                // Si existe el producto
                                if (productoEncontrado != nullptr) {

                                    NProductos* np = new NProductos();

                                    np->producto = productoEncontrado;
                                    np->cantidad = cantidad;

                                    productosDelPedido->agregaFinal(np);
                                } else {
                                    // crear placeholder para conservar id y cantidad
                                    Producto* placeholder = new Producto("desconocido", "desconocido", 0.0, idProducto, 0, 0);
                                    NProductos* np = new NProductos();
                                    np->producto = placeholder;
                                    np->cantidad = cantidad;
                                    productosDelPedido->agregaFinal(np);
                                }
                            }
                        }

                        temp = "";
                    }
                    else {
                        temp += productosTexto[j];
                    }
                }

                // =========================
                // CREAR PEDIDO con idUsuario incluido
                // Pedido(int idPedido, int idUsuario, double peso, EstadoPedido, Lista<NProductos*>, string fecha)
                // =========================

                Pedido* pedido = new Pedido(
                    idPedido,
                    idUsuario,
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