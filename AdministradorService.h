#pragma once
#include "PedidoService.h"
#include "ProductoService.h"
#include "UsuarioService.h"

class AdministradorService {
private:
	PedidoService* pedidoService;
	ProductoService* productoService;
	UsuarioService* usuarioService;


public:
	AdministradorService () {
		this->pedidoService = new PedidoService();
		this->productoService = new ProductoService();
		this->usuarioService = new UsuarioService();
	}

	~AdministradorService () {
	}


	void inicializarTodosLosProductos() { productoService->iniciaizarProductos(); }


	Lista<Pedido*>* obtenerPedidos() { return pedidoService->obtenerPedidosHistoricos(); }
	Lista<Usuario*>* obtenerUsuarios() { return usuarioService->obtenerTodos(); }

	void mostrarTodosLosProductos() {
		productoService->mostrarInventario();
		system("pause");
	}
	void mostrarTodosLosUsuarios() {

		obtenerUsuarios();

		system("pause");
	}

	void eliminarProductoGlobal(int id) {
		productoService->eliminarProducto(id);
	}

	void editarProductoGlobal(int id, string nNombre, string nCat, double nPrecio, int nStock) {
		Producto* p = productoService->buscarProductoPorId(0, [id](Producto* prod) { return prod->getId() == id; });

		if (p != nullptr) {
			p->setNombre(nNombre);
			p->setCategoria(nCat);
			p->setPrecio(nPrecio);
			p->setStock(nStock);
			productoService->guardarProductos(); // guarda los cambios en el txt
			cout << u8"\nProducto modificado con éxito.\n";
		}
		else {
			cout << u8"\nError: Producto no encontrado.\n";
		}
	}

	void agregarProductoGlobal(string nombre, string cat, double precio, int stock) {
		int nuevoId = rand() % 9000 + 1000;
		Producto* p = new Producto(nombre, cat, precio, nuevoId, 0, stock);
		productoService->agregarProducto(p, 0);
		cout << u8"\nProducto publicado con éxito. ID asignado: " << nuevoId << "\n";
	}

	// Función auxiliar para editar usuarios
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
};
