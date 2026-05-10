#pragma once
#include "Cliente.h"
#include "ProductoService.h"
#include "PedidoService.h"
#include "CompraService.h"
#include "idAleatorio.h"

class UsuarioService {
private:
	ProductoService* productoService;
	PedidoService* pedidoService;


public:
	UsuarioService () {
		this->productoService = new ProductoService();
		this->pedidoService = new PedidoService();

	}

	~UsuarioService () {
	}

	// Para el cliente
	void ListarProductos() {productoService->mostrarInventario();}

	Producto* buscarProductoPorID(int id) {
		return productoService->buscarProductoPorId(id, [](Producto* p) {return true; });
	}

	Lista<Producto*>* buscarProdutoPoCategoria( string categoria) {
		return productoService->obtenerPorductosPorCondicion(0, [categoria](Producto* p) {return (p->getCategoria() == categoria); });
	}

	void realizarCompraProductos(int idUsuario, Cliente* cli, MetodoPago metodo,TipoComprobante tipo) {
		CompraService* compraService = new CompraService(productoService, pedidoService);
		compraService->generarCompra(cli, metodo, tipo);
	}

	Lista<Pedido*>* obtenerPedidos() {
		return pedidoService->obtenerPedidosHistoricos();
	}

	void listarPedidos() {

	}

	// Para el vendedor
	void inicializarListaProductosVendedor() {
		productoService->iniciaizarProductos();
	}

	void agregarProducto(int idVendedor ,string nombre, string categoria, double precio, int stock) {
		int id = idAleatorioPorDigitos(6);
		Producto* nuevoProducto = new Producto(nombre, categoria, precio, id , idVendedor, stock);
		productoService->agregarProducto(nuevoProducto, idVendedor);
	}
	void mostrarProductosPorVendedor(int idVendedor) {
		Lista<Producto*>* productos = productoService->obtenerPorductosPorCondicion(0, [idVendedor](Producto* p) { return p->getIdVendedor() == idVendedor; });
		for (int i = 0; i < productos->longitud(); i++) {
			cout << "\nN°" << i << endl;
			productos->obtenerPos(i)->MostrarProducto();
		}
	}
};
