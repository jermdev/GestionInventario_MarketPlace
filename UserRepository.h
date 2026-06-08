#pragma once
#include <fstream>
#include <string>
#include "Lista.h"
#include "Usuario.h"
#include "Cliente.h"
#include "Vendedor.h"
#include "Administrador.h"
#include "ClienteBuilder.h"
#include "VendedoresBuilder.h"
#include "AdministradorBuilder.h"

#include "parseRol.h"
using namespace std;

class UsuarioRepository {
private:
    string rutaClientes;
    string rutaVendedores;

    void escribirString(ofstream& archivo, const string& s) {
        int len = (int)s.size();
        archivo.write(reinterpret_cast<const char*>(&len), sizeof(int));
        if (len > 0)
            archivo.write(s.c_str(), len);
    }

    string leerString(ifstream& archivo) {
        int len = 0;
        archivo.read(reinterpret_cast<char*>(&len), sizeof(int));
        if (len <= 0 || len > 4096) return "";
        string s(len, '\0');
        archivo.read(&s[0], len);
        return s;
    }

    // Carga todos los registros de un archivo.
    // esCliente=true → construye Clientes, false → Vendedores.
    Lista<Usuario*>* cargarDesdeArchivo(const string& ruta, ROL rol) {
        Lista<Usuario*>* lista = new Lista<Usuario*>();
        ifstream archivo(ruta, ios::binary | ios::in);
        if (!archivo.is_open()) return lista;

        while (archivo.peek() != EOF) {
            int id = 0;
            archivo.read(reinterpret_cast<char*>(&id), sizeof(int));
            if (archivo.fail() || archivo.eof()) break;

            string nombre = leerString(archivo);
            string correo = leerString(archivo);
            string direccion = leerString(archivo);
            string contrasenia = leerString(archivo);
            string rolString = leerString(archivo);
            string cargo;
            int extra = 0;
            if (stringToRol(rolString) == ROL::ADMINISTRADOR) {
                archivo.read(reinterpret_cast<char*>(&cargo), sizeof(string));
            }
            else {

                archivo.read(reinterpret_cast<char*>(&extra), sizeof(int));
            }
            if (archivo.fail()) break;

            Usuario* u = nullptr;
            if (rol == ROL::CLIENTE) {
                ClienteBuilder b;
                
                b.setId(id);
                b.setNombre(nombre);
                b.setCorreo(correo);
                b.setDireccion(direccion);
                b.setContrasenia(contrasenia);
                b.setRol(stringToRol(rolString));

                u = b.build();
            } else if (rol == ROL::VENDEDOR) {
                VendedorBuilder b;
                b.setNumeroEstrellas(extra);
                b.setId(id);
                b.setNombre(nombre);
                b.setCorreo(correo);
                b.setDireccion(direccion);
                b.setContrasenia(contrasenia);
                b.setRol(stringToRol(rolString));

                u = b.build();
            }
            else if (rol == ROL::ADMINISTRADOR) {
                AdministradorBuilder b;
                b.setId(id);
                b.setNombre(nombre);
                b.setCorreo(correo);
                b.setDireccion(direccion);
                b.setContrasenia(contrasenia);
                b.setRol(stringToRol(rolString));
            }
            if (u != nullptr)
                lista->agregaFinal(u);
        }
        return lista;
    }

public:
    UsuarioRepository(const string& rutaClientes  = "clientes.bin",
                      const string& rutaVendedores = "vendedores.bin")
        : rutaClientes(rutaClientes), rutaVendedores(rutaVendedores)
    {}

    ~UsuarioRepository() {}

    void guardarCliente(Cliente* cliente) {
        ofstream archivo(rutaClientes, ios::binary | ios::out | ios::app);
        if (!archivo.is_open()) return;
        int id = cliente->getId();
        archivo.write(reinterpret_cast<const char*>(&id), sizeof(int));
        escribirString(archivo, cliente->getNombre());
        escribirString(archivo, cliente->getCorreo());
        escribirString(archivo, cliente->getDireccion());
        escribirString(archivo, cliente->getContrasenia());
        escribirString(archivo, rolToString(cliente->getRol()));// verificar que se guarda correctamente
        int extra = cliente->getNumeroCompras();
        archivo.write(reinterpret_cast<const char*>(&extra), sizeof(int));
    }

    void guardarVendedor(Vendedor* vendedor) {
        ofstream archivo(rutaVendedores, ios::binary | ios::out | ios::app);
        if (!archivo.is_open()) return;
        int id = vendedor->getId();
        archivo.write(reinterpret_cast<const char*>(&id), sizeof(int));
        escribirString(archivo, vendedor->getNombre());
        escribirString(archivo, vendedor->getCorreo());
        escribirString(archivo, vendedor->getDireccion());
        escribirString(archivo, vendedor->getContrasenia());
        escribirString(archivo, rolToString(vendedor->getRol()));// verificar que se guarda correctamente
        int extra = vendedor->getNumeroEstrellas();
        archivo.write(reinterpret_cast<const char*>(&extra), sizeof(int));
    }

    void guardarAdministrador(Administrador* administrador) {
        ofstream archivo(rutaVendedores, ios::binary | ios::out | ios::app);
        if (!archivo.is_open()) return;
        int id = administrador->getId();
        archivo.write(reinterpret_cast<const char*>(&id), sizeof(int));
        escribirString(archivo, administrador->getNombre());
        escribirString(archivo, administrador->getCorreo());
        escribirString(archivo, administrador->getDireccion());
        escribirString(archivo, administrador->getContrasenia());
        escribirString(archivo, rolToString(administrador->getRol()));// verificar que se guarda correctamente
        string extra = administrador->getCargo();
        archivo.write(reinterpret_cast<const char*>(&extra), sizeof(string));
    }
    // Busca por correo en ambos archivos.
    // Retorna puntero heap-allocated — el CALLER es dueno y debe hacer delete.
    // Retorna nullptr si no se encuentra.
    Usuario* buscarPorCorreo(const string& correo) {
       Lista<Usuario*>* clientes = cargarDesdeArchivo(rutaClientes, ROL::CLIENTE);

        Usuario* encontrado = nullptr;

        for (uint i = 0; i < clientes->longitud(); i++) {
            Usuario* u = clientes->obtenerPos(i);

            if (u != nullptr && u->getCorreo() == correo) {
                encontrado = u;
            }
            else {
                delete u;
            }
        }

        delete clientes;

        if (encontrado != nullptr) {
            return encontrado;
            }

    // ===== VENDEDORES =====
        Lista<Usuario*>* vendedores = cargarDesdeArchivo(rutaVendedores, ROL::VENDEDOR);

        for (uint i = 0; i < vendedores->longitud(); i++) {
            Usuario* u = vendedores->obtenerPos(i);

            if (u != nullptr && u->getCorreo() == correo) {
                encontrado = u;
            }
            else {
                delete u;
            }
        }

        delete vendedores;

        return encontrado;
    }

    // Carga todos los usuarios de ambos archivos en una sola lista.
    // El CALLER es dueno de la lista y de todos los elementos dentro.
    Lista<Usuario*>* cargarTodos() {
        Lista<Usuario*>* todos      = cargarDesdeArchivo(rutaClientes,   ROL::CLIENTE);
        Lista<Usuario*>* vendedores = cargarDesdeArchivo(rutaVendedores, ROL::VENDEDOR);
        for (uint i = 0; i < vendedores->longitud(); i++)
            todos->agregaFinal(vendedores->obtenerPos(i));
        while (!vendedores->esVacia()) vendedores->eliminaInicial();
        delete vendedores;
        return todos;
    }

    // Genera un ID unico: max(ids existentes) + 1. Retorna 1 si no hay usuarios.
    int generarNuevoId() {
        int maxId = 0;
        Lista<Usuario*>* todos = cargarTodos();
        for (uint i = 0; i < todos->longitud(); i++) {
            Usuario* u = todos->obtenerPos(i);
            if (u != nullptr && u->getId() > maxId)
                maxId = u->getId();
        }
        for (uint i = 0; i < todos->longitud(); i++)
            delete todos->obtenerPos(i);
        delete todos;
        return maxId + 1;
    }

    bool correoExiste(const string& correo) {
        Usuario* u = buscarPorCorreo(correo);
        if (u != nullptr) {
            delete u;
            return true;
        }
        return false;
    }
};
