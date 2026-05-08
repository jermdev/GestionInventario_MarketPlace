#pragma once
#include <iostream>
#include <string>

using namespace std;

class Envio {
private:
    int idPedido;
    string direccionDestino;
    string empresaTransporte;
    string fechaEstimada;
    string estadoEnvio;

public:
    Envio(int idPedido, string direccion, string transporte, string fecha) {
        this->idPedido = idPedido;
        this->direccionDestino = direccion;
        this->empresaTransporte = transporte;
        this->fechaEstimada = fecha;
        this->estadoEnvio = "En Almacen";
    }

    ~Envio() {}

    int getIdPedido() { return idPedido; }
    string getEstado() { return estadoEnvio; }
    string getDireccion() { return direccionDestino; }

    void setEstadoEnvio(string nuevoEstado) {
        this->estadoEnvio = nuevoEstado;
    }

    void mostrarDetalleEnvio() {
        cout << "\n===== DETALLE DE ENVIO =====" << endl;
        cout << "Pedido/Guia Nro: " << idPedido << endl;
        cout << "Destino: " << direccionDestino << endl;
        cout << "Transporte: " << empresaTransporte << endl;
        cout << "Entrega estimada: " << fechaEstimada << endl;
        cout << "Estado actual: " << estadoEnvio << endl;
        cout << "============================" << endl;
    }
};