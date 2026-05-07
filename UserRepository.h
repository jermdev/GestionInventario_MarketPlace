#pragma once
#include <fstream>
#include <string>
#include "Lista.h"
#include "Usuario.h"
#include "Cliente.h"
#include "Vendedor.h"
#include "ClienteBuilder.h"
#include "VendedoresBuilder.h"
using namespace std;

// Formato binario por registro (mismo para clientes y vendedores):
//   [id:         int32]
//   [nombre_len: int32][nombre:     char*nombre_len]
//   [correo_len: int32][correo:     char*correo_len]
//   [dir_len:    int32][direccion:  char*dir_len]
//   [hash_len:   int32][hash:       char*hash_len]
//   [extra:      int32]  → numCompras (Cliente) o numeroEstrellas (Vendedor)
//
// std::string no es POD: serializarla con sizeof() escribiria un puntero
// invalido en la proxima ejecucion. Por eso se usa length-prefixed encoding.
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
    Lista<Usuario*>* cargarDesdeArchivo(const string& ruta, bool esCliente) {
        Lista<Usuario*>* lista = new Lista<Usuario*>();
        ifstream archivo(ruta, ios::binary | ios::in);
        if (!archivo.is_open()) return lista;

        while (archivo.peek() != EOF) {
            int id = 0;
            archivo.read(reinterpret_cast<char*>(&id), sizeof(int));
            if (archivo.fail() || archivo.eof()) break;

            string nombre    = leerString(archivo);
            string correo    = leerString(archivo);
            string direccion = leerString(archivo);
            string hashStr   = leerString(archivo);
            int    extra     = 0;
            archivo.read(reinterpret_cast<char*>(&extra), sizeof(int));
            if (archivo.fail()) break;

            Usuario* u = nullptr;
            if (esCliente) {
                ClienteBuilder b;
                u = b.setId(id)
                     .setNombre(nombre)
                     .setCorreo(correo)
                     .setDireccion(direccion)
                     .setContraseniaHash(hashStr)
                     .build();
            } else {
                VendedorBuilder b;
                b.setNumeroEstrellas(extra);
                u = b.setId(id)
                     .setNombre(nombre)
                     .setCorreo(correo)
                     .setDireccion(direccion)
                     .setContraseniaHash(hashStr)
                     .build();
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
        escribirString(archivo, cliente->getContraseniaHash());
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
        escribirString(archivo, vendedor->getContraseniaHash());
        int extra = vendedor->getNumeroEstrellas();
        archivo.write(reinterpret_cast<const char*>(&extra), sizeof(int));
    }

    // Busca por correo en ambos archivos.
    // Retorna puntero heap-allocated — el CALLER es dueno y debe hacer delete.
    // Retorna nullptr si no se encuentra.
    Usuario* buscarPorCorreo(const string& correo) {
        Lista<Usuario*>* clientes = cargarDesdeArchivo(rutaClientes, true);
        for (uint i = 0; i < clientes->longitud(); i++) {
            Usuario* u = clientes->obtenerPos(i);
            if (u != nullptr && u->getCorreo() == correo) {
                // Liberar todos los demas usuarios cargados
                for (uint j = 0; j < clientes->longitud(); j++) {
                    if (j != i) delete clientes->obtenerPos(j);
                }
                delete clientes;
                return u;
            }
        }
        for (uint i = 0; i < clientes->longitud(); i++)
            delete clientes->obtenerPos(i);
        delete clientes;

        Lista<Usuario*>* vendedores = cargarDesdeArchivo(rutaVendedores, false);
        for (uint i = 0; i < vendedores->longitud(); i++) {
            Usuario* u = vendedores->obtenerPos(i);
            if (u != nullptr && u->getCorreo() == correo) {
                for (uint j = 0; j < vendedores->longitud(); j++) {
                    if (j != i) delete vendedores->obtenerPos(j);
                }
                delete vendedores;
                return u;
            }
        }
        for (uint i = 0; i < vendedores->longitud(); i++)
            delete vendedores->obtenerPos(i);
        delete vendedores;

        return nullptr;
    }

    // Carga todos los usuarios de ambos archivos en una sola lista.
    // El CALLER es dueno de la lista y de todos los elementos dentro.
    Lista<Usuario*>* cargarTodos() {
        Lista<Usuario*>* todos      = cargarDesdeArchivo(rutaClientes,   true);
        Lista<Usuario*>* vendedores = cargarDesdeArchivo(rutaVendedores, false);
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
