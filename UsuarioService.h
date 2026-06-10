#pragma once
#include "UserRepository.h"


class UsuarioService {
private:
	
	UsuarioRepository* usuarioRepository;
	
public:
	UsuarioService () {
		this->usuarioRepository = new UsuarioRepository();
		
	}

	~UsuarioService () {
	}

    bool agregarUsuario(Usuario* u) {
        if (u == nullptr) return false;
        if (usuarioRepository->correoExiste(u->getCorreo())) return false;
        if (u->getId() == 0) {
            int nuevoId = usuarioRepository->generarNuevoId();
            u->setId(nuevoId);
        }
        ROL r = u->getRol();
        if (r == ROL::CLIENTE) {
            usuarioRepository->guardarCliente(static_cast<Cliente*>(u));
        }
        else if (r == ROL::VENDEDOR) {
            usuarioRepository->guardarVendedor(static_cast<Vendedor*>(u));
        }
        else if (r == ROL::ADMINISTRADOR) {
            usuarioRepository->guardarAdministrador(static_cast<Administrador*>(u));
        }
        else {
            return false;
        }
        return true;
    }
    bool modificarUsuario(Usuario* usuarioActualizado) {
        if (usuarioActualizado == nullptr) return false;
        return usuarioRepository->actualizarUsuario(usuarioActualizado);
    }

    bool eliminarUsuarioPorCorreo(const string& correo) {
        return usuarioRepository->eliminarPorCorreo(correo);
    }

    // Buscar por correo — devuelve puntero heap (caller es dueño)
    Usuario* buscarPorCorreo(const string& correo) {
        return usuarioRepository->buscarPorCorreo(correo);
    }

    // Devuelve lista heap-allocated con todos los usuarios (caller debe liberar)
    Lista<Usuario*>* obtenerTodos() {
        return usuarioRepository->cargarTodos();
    }

    int generarNuevoId() {
        return usuarioRepository->generarNuevoId();
    }

    bool correoExiste(const string& correo) {
        return usuarioRepository->correoExiste(correo);
    }
};
