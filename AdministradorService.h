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


	Lista<Pedido*>* obtenerPedidos() { pedidoService->obtenerPedidosHistoricos(); }
	Lista<Usuario*>* obtenerUsuarios() { usuarioService->obtenerTodos(); }

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

	void verProductos() {
		productoService->mostrarInventario();
	}

	void verPedidos() {
		Lista<Pedido*>* pedidos  = obtenerPedidos();
		int nPedidos = pedidos->longitud();
		for (int i = 0; i < nPedidos; i++) {
			Usuario* u = usuarioService->buscarPorCorreo


		}
	}
};
