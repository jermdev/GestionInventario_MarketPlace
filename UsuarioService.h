#pragma once
#include "Cliente.h"
#include "Cola.h"
#include "ProductoService.h"
#include "PedidoService.h"
#include "CompraService.h"

//utilidades
#include "idAleatorio.h"
#include "Ordenamiento.h"

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
			if (productos->obtenerPos(i)->getStock() > 0) {
				cout << "\nN°" << i << endl;
				productos->obtenerPos(i)->MostrarProducto();

			}
		}
		system("pause");
	}

	void mostrarListaProductosOrdenadaPorPrecio(string tipoOrden) {
		// Obtener todos los productos (productoService devuelve una Lista nueva en heap)
		Lista<Producto*>* productos = productoService->obtenerPorductosPorCondicion(0, [](Producto* p) { return true; });

		int n = productos->longitud();
		if (n == 0) {
			cout << "No hay productos para ordenar." << endl;
			delete productos;
			system("pause");
			return;
		}

		bool asc = true;
		if (tipoOrden == "desc" || tipoOrden == "mayor") asc = false;
		else if (tipoOrden == "asc" || tipoOrden == "menor") asc = true;
		else asc = true;

		// Orden sencillo: insertion sort sobre la Lista
		insertionSortListaPorPrecio(productos, asc);

		// Mostrar productos ordenados
		for (int i = 0; i < n; ++i) {
			Producto* p = productos->obtenerPos(i);
			if (p->getStock() > 0) {
				cout << "\nN°" << i << endl;
				if (p) p->MostrarProducto();
			}
		}

		// limpiar lista temporal creada por ProductoService
		delete productos;

		system("pause");
	}

	//Inicializar Pedidos

	void InicializarPedidosPorIdCliente(int idCLiente) {
		pedidoService->setPedidos(pedidoService->obtenerPedidosHistoricos());
		Lista<Pedido*>* pedidos = pedidoService->obtenerPedidosPorCondicion(0 ,[idCLiente](Pedido* p) {return p->getIdPedido() == idCLiente; });
		pedidoService->setPedidos(pedidos);
	}

	void listarPedidos(int idCliente) {

		Lista<Pedido*>* todosLosPedidos = pedidoService->obtenerPedidosHistoricos();

		if (todosLosPedidos->esVacia()) {
			cout << "No hay pedidos registrados en el sistema." << endl;
			return;
		}

		Cola<Pedido*> colaDePedidos;

	// (El más antiguo entra primero)
		for (int i = 0; i < todosLosPedidos->longitud(); i++) {
			if (todosLosPedidos->obtenerPos(i)->getIdCliente() == idCliente) {

				colaDePedidos.enqueue(todosLosPedidos->obtenerPos(i));
			}
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

			int index = productos->longitud() + 1;
		for (int i = 0; i < productos->longitud(); i++) {
			pilaProductos.push(productos->obtenerPos(i));

			cout << "\n--- Tus productos (Mas recientes primero) ---" << endl;
			while (!pilaProductos.estaVacia()) {
				cout << "\nN°" << --index << endl;
				pilaProductos.pop()->MostrarProducto();
			}
		}
	}
	void eliminarProductosPorVendedor(int idVendedor, int idProducto) {
		// Buscamos directamente el producto validando que exista y le pertenezca a este vendedor
		Producto* p = productoService->buscarProductoPorId(0, [idVendedor, idProducto](Producto* prod) {
			return prod->getId() == idProducto && prod->getIdVendedor() == idVendedor;
			});

		if (p != nullptr) {
			// Si lo encuentra, lo elimina
			productoService->eliminarProducto(idProducto);
			cout << "Producto eliminado exitosamente." << endl;
		}
		else {
			// Si no lo encuentra, o si el producto es de otro vendedor
			cout << "Error: No se encontro el producto o no te pertenece." << endl;
		}
	}
	Lista<Producto*>* obtenerListaProductosVendedor(int idVendedor) {
		return productoService->obtenerPorductosPorCondicion(0, [idVendedor](Producto* p) {
			return p->getIdVendedor() == idVendedor;
			});
	}
};
