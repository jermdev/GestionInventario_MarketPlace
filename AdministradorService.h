#pragma once
#include "PedidoService.h"
#include "ProductoService.h"
#include "UsuarioService.h"
#include "Ordenamiento.h"
#include "ArbolAVL.hpp"

class AdministradorService {
private:
	PedidoService* pedidoService;
	ProductoService* productoService;
	UsuarioService* usuarioService;
	ArbolAVL<Pedido*>* pedidosArbol;


	static void imprimir(Pedido *p) {
		cout << "ID: " << p->getIdPedido() << " - Cliente: " << p->getIdCliente() << "\n";
	}

	static void procesarAltura(int) {

	}

public:
	AdministradorService () {
		this->pedidoService = new PedidoService();
		this->productoService = new ProductoService();
		this->usuarioService = new UsuarioService();
		pedidosArbol = new ArbolAVL<Pedido*>(imprimir, [](Pedido* a, Pedido* b) -> int {
			if (a->getIdPedido() < b->getIdPedido()) return -1;
			if (a->getIdPedido() > b->getIdPedido()) return 1;
			return 0;
		});

		incializarArbolAVl();
	}

	~AdministradorService () {
	}


	void inicializarTodosLosProductos() { productoService->iniciaizarProductos(); }

	void incializarArbolAVl() {
		Lista<Pedido*>* listaPedidos = pedidoService->obtenerPedidosHistoricos();
		int nPedidos = listaPedidos->longitud();
		for (int i = 0; i < nPedidos; i++) {
			Pedido *p = listaPedidos->obtenerPos(i);
			pedidosArbol->Insertar(p);
		}
	}

	Usuario* obtenerUsuario(int id) { return usuarioService->buscarPorID(id); }
	Lista<Pedido*>*   obtenerPedidos() { return pedidoService->obtenerPedidosHistoricos(); }
	Lista<Usuario*>*  obtenerUsuarios() { return usuarioService->obtenerTodos(); }
	Lista<Producto*>* obtenerTodosLosProductos() { return productoService->obtenerPorductosPorCondicion(0, [](Producto* p) { return true; }); }

	void mostrarTodosLosProductos() {
		productoService->mostrarInventario();
		system("pause");
	}
	void mostrarTodosLosUsuarios() {

		obtenerUsuarios();

		system("pause");
	}


	void editarProductoGlobal(int id, string nNombre, string nCat, double nPrecio, int nStock) {
		Producto* p = productoService->buscarProductoPorId(0, [id](Producto* prod) { return prod->getId() == id; });

		if (p != nullptr) {
			p->setNombre(nNombre);
			p->setCategoria(nCat);
			p->setPrecio(nPrecio);
			p->setStock(nStock);
			productoService->guardarProductos(); // guarda los cambios en el txt
			cout << u8"\nProducto modificado con �xito.\n";
		}
		else {
			cout << u8"\nError: Producto no encontrado.\n";
		}
	}

	void agregarProductoGlobal(string nombre, string cat, double precio, int stock) {
		int nuevoId = rand() % 9000 + 1000;
		Producto* p = new Producto(nombre, cat, precio, nuevoId, 0, stock);
		productoService->agregarProducto(p, 0);
		cout << u8"\nProducto publicado con �xito. ID asignado: " << nuevoId << "\n";
	}

	// Funci�n auxiliar para editar usuarios
	Usuario* obtenerUsuarioParaEditar(string correo) {
		return usuarioService->buscarPorCorreo(correo);
	}

	void MostrarUsuarioPorCorreo(string correo) {
		Usuario* u = usuarioService->buscarPorCorreo(correo);
		cout << "\n************************\n";
		u->mostrarUsuario();
		cout << "************************\n";
	}
	
	void modificarUsuario(Usuario* u) {
		bool estaModificado = usuarioService->modificarUsuario(u);

		if (estaModificado) {
			cout << "\nUsuario Modificado con exito";
		}
		else {
			cout << "\Error al modificar el Usuario Verifique que el usuario exista";
		}
	}

	void eliminarUsuario(string correo) {
		bool estaEliminado = usuarioService->eliminarUsuarioPorCorreo(correo);
		if (estaEliminado) {
			cout << "\nUsuario elimiando con exito.";
		}
		else {
			cout << "\nError al eliminar el usuario con el correo: " << correo << " verifique que el correo sea correcto.";
		}
	}

	void verPedidos(string correo) {
		Lista<Pedido*>* pedidos  = obtenerPedidos();
		int nPedidos = pedidos->longitud();
		for (int i = 0; i < nPedidos; i++) {
			Usuario* u = usuarioService->buscarPorCorreo(correo);
			if (u != nullptr) {
				u->mostrarUsuario();
				pedidos->obtenerPos(i)->mostrarDetallePedido();
			}
			else {
				cout << "No hay pedidos"<<endl;
			}
		}
	}

	void eliminarProductoGlobal(int idProducto) {
		productoService->eliminarProducto(idProducto);
		cout << "\nProducto eliminado del sistema.\n";
	}

	// Función para mostrar ordenado por Precio usando QuickSort
	void mostrarProductosOrdenadosPorPrecio() {
		Lista<Producto*>* productos = productoService->obtenerPorductosPorCondicion(0, [](Producto* p) { return true; });
		int n = productos->longitud();
		if (n == 0) { cout << "No hay productos.\n"; return; }

		// Pasamos de Lista a Arreglo nativo
		Producto** arr = new Producto * [n];
		for (int i = 0; i < n; i++) arr[i] = productos->obtenerPos(i);

		quicksort(arr, 0, n - 1);

		for (int i = 0; i < n; i++) arr[i]->MostrarProducto();

		delete[] arr; // Limpiamos memoria
	}

	// Función para mostrar ordenado por Nombre usando MergeSort
	void mostrarProductosOrdenadosPorNombre() {
		Lista<Producto*>* productos = productoService->obtenerPorductosPorCondicion(0, [](Producto* p) { return true; });
		int n = productos->longitud();
		if (n == 0) { cout << "No hay productos.\n"; return; }

		Producto** arr = new Producto * [n];
		for (int i = 0; i < n; i++) arr[i] = productos->obtenerPos(i);

		mergeSort(arr, n);

		for (int i = 0; i < n; i++) arr[i]->MostrarProducto();

		delete[] arr;
	}

	void editarProductoGlobal(int id, string nNombre, string nCat, double nPrecio) {
		Lista<Producto*>* productos = productoService->obtenerPorductosPorCondicion(0, [](Producto* p) { return true; });
		for (int i = 0; i < productos->longitud(); i++) {
			Producto* p = productos->obtenerPos(i);
			if (p->getId() == id) {
				p->setNombre(nNombre);
				p->setCategoria(nCat);
				p->setPrecio(nPrecio);
				productoService->guardarProductos();
				cout << "\nProducto actualizado con exito.\n";
				return;
			}
		}
		cout << "\nNo se encontro el producto.\n";
	}

	Pedido* buscarPedidoPorId(int id) {
		Pedido* ptemporal = new Pedido(id);
		Pedido** resultado = pedidosArbol->Buscar(ptemporal);
		delete ptemporal;
		return (resultado != nullptr) ? *resultado : nullptr;
	}

	void matchArbolPedidos() {
		Lista<Pedido*>* pedidosDeArbol = new Lista<Pedido*>();
		std::vector<Pedido*> pedidos = pedidosArbol->ObtenerElementos();
		for (Pedido* e : pedidos) {
			pedidosDeArbol->agregaInicial(e);
		}
		pedidoService->setPedidos(pedidosDeArbol);
		pedidoService->GuardarTodoLosPedidosEnArchivo();
	}

	bool borrarPedido(int id) {
		Pedido* pTemporal = new Pedido(id);
		bool es_borrado = pedidosArbol->Eliminar(pTemporal);
		delete pTemporal;

		if (es_borrado) {
			matchArbolPedidos();
		}

		return es_borrado;
	}

	bool actualizarPedido(int id, double nuevoPeso,EstadoPedido nuevoEstado, string nuevaFecha) {
		Pedido* p = buscarPedidoPorId(id);
		if (p == nullptr) {
			cout << "\nError: Pedido no encontrado.\n";
			return false;
		}

		// idPedido NO cambia (es la clave de orden del AVL), solo se modifican los demás campos
		p->setPeso(nuevoPeso);
		p->setEstadoPedido(nuevoEstado);
		p->setFechaEntrega(nuevaFecha);

		matchArbolPedidos(); // reconstruye la lista desde el árbol y persiste TODO el archivo de una vez
		cout << "\nPedido actualizado con exito.\n";
		return true;
	}
};
