#pragma once
#include "Cliente.h"
#include "Cola.h"
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
		return productoService->buscarProductoPorId(0, [id](Producto* p) { return p->getId() == id; });
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

	void mostrarTodosLosProductos() {
		productoService->mostrarInventario();
		system("pause");
	}
	void filtrarPorCategoria(string categoria) {

		Lista<Producto*>* productos = productoService->obtenerPorductosPorCondicion(0, [categoria](Producto* p) { return p->getCategoria() == categoria; });

		if (productos->longitud() == 0) {
			cout << "\n Categoria no encontrada. \n";
			system("pause");
			return;
		}

		for (int i = 0; i < productos->longitud(); i++) {
			cout << "\nN°" << i << endl;
			productos->obtenerPos(i)->MostrarProducto();
		}
		system("pause");
	}
	void listarPedidos() {

		Lista<Pedido*>* todosLosPedidos = pedidoService->obtenerPedidosHistoricos();

		if (todosLosPedidos->esVacia()) {
			cout << "No hay pedidos registrados en el sistema." << endl;
			return;
		}

		Cola<Pedido*> colaDePedidos;

	// (El más antiguo entra primero)
		for (int i = 0; i < todosLosPedidos->longitud(); i++) {
			colaDePedidos.enqueue(todosLosPedidos->obtenerPos(i));
		}

		cout << "\n--- BANDEJA DE PEDIDOS (Orden de llegada) ---" << endl;

	//(El vendedor verá primero el más urgente)
		while (!colaDePedidos.esVacia()) {
			Pedido* ped = colaDePedidos.dequeue();
			ped->mostrarDetallePedido();
		}
	}

	// Para el vendedor
	void inicializarListaProductos() {
		productoService->iniciaizarProductos();
	}

	void agregarProducto(int idVendedor ,string nombre, string categoria, double precio, int stock) {
		int id = idAleatorioPorDigitos(6);
		Producto* nuevoProducto = new Producto(nombre, categoria, precio, id , idVendedor, stock);
		productoService->agregarProducto(nuevoProducto, idVendedor);
	}
	void mostrarProductosPorVendedor(int idVendedor) {
		Lista<Producto*>* productos = productoService->obtenerPorductosPorCondicion(0, [idVendedor](Producto* p) { return p->getIdVendedor() == idVendedor; });

		Pila<Producto*> pilaProductos;

		for (int i = 0; i < productos->longitud(); i++) {
			pilaProductos.push(productos->obtenerPos(i));

			cout << "\n--- Tus productos (Mas recientes primero) ---" << endl;
			int index = productos->longitud();
			while (!pilaProductos.estaVacia()) {
				cout << "\nN°" << --index << endl;
				pilaProductos.pop()->MostrarProducto();
			}
		}
	}
	void eliminarProductosPorVendedor(int idVendedor,int idProducto) {
		Lista<Producto*>* productos = productoService->obtenerPorductosPorCondicion(0, [idVendedor](Producto* p) { return p->getIdVendedor() == idVendedor; });
		
		for (int i = 0; i < productos->longitud(); i++) {
			if (productos->obtenerPos(i)->getIdVendedor() == idVendedor && productos->obtenerPos(i)->getId()==idProducto) {
				
				productoService->eliminarProducto(idProducto);
			}
			else {
				cout << "No hay producto a eliminar" << endl;
			}
		}
	}
};
