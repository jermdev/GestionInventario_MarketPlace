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
    string rutaAdministradores;

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
                cargo = leerString(archivo);
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
                
                u = b.build();
            }
            if (u != nullptr)
                lista->agregaFinal(u);
        }
        return lista;
    }

    void sobrescribirClientes(Lista<Usuario*>* lista) {
        ofstream archivo(rutaClientes, ios::binary | ios::out | ios::trunc);
        if (!archivo.is_open()) return;
        for (uint i = 0; i < lista->longitud(); i++) {
            Usuario* u = lista->obtenerPos(i);
            Cliente* c = dynamic_cast<Cliente*>(u);
            if (c == nullptr) continue;
            int id = c->getId();
            archivo.write(reinterpret_cast<const char*>(&id), sizeof(int));
            escribirString(archivo, c->getNombre());
            escribirString(archivo, c->getCorreo());
            escribirString(archivo, c->getDireccion());
            escribirString(archivo, c->getContrasenia());
            escribirString(archivo, rolToString(c->getRol()));
            int extra = c->getNumeroCompras();
            archivo.write(reinterpret_cast<const char*>(&extra), sizeof(int));
        }
    }

    void sobrescribirVendedores(Lista<Usuario*>* lista) {
        ofstream archivo(rutaVendedores, ios::binary | ios::out | ios::trunc);
        if (!archivo.is_open()) return;
        for (uint i = 0; i < lista->longitud(); i++) {
            Usuario* u = lista->obtenerPos(i);
            Vendedor* v = dynamic_cast<Vendedor*>(u);
            if (v == nullptr) continue;
            int id = v->getId();
            archivo.write(reinterpret_cast<const char*>(&id), sizeof(int));
            escribirString(archivo, v->getNombre());
            escribirString(archivo, v->getCorreo());
            escribirString(archivo, v->getDireccion());
            escribirString(archivo, v->getContrasenia());
            escribirString(archivo, rolToString(v->getRol()));
            int extra = v->getNumeroEstrellas();
            archivo.write(reinterpret_cast<const char*>(&extra), sizeof(int));
        }
    }

    void sobrescribirAdministradores(Lista<Usuario*>* lista) {
        ofstream archivo(rutaAdministradores, ios::binary | ios::out | ios::trunc);
        if (!archivo.is_open()) return;
        for (uint i = 0; i < lista->longitud(); i++) {
            Usuario* u = lista->obtenerPos(i);
            Administrador* a = dynamic_cast<Administrador*>(u);
            if (a == nullptr) continue;
            int id = a->getId();
            archivo.write(reinterpret_cast<const char*>(&id), sizeof(int));
            escribirString(archivo, a->getNombre());
            escribirString(archivo, a->getCorreo());
            escribirString(archivo, a->getDireccion());
            escribirString(archivo, a->getContrasenia());
            escribirString(archivo, rolToString(a->getRol()));
            escribirString(archivo, a->getCargo());
        }
    }

public:
    UsuarioRepository(const string& rutaClientes  = "clientes.bin",
                      const string& rutaVendedores = "vendedores.bin",
                      const string& rutaAdministradores = "administradores.bin")
        : rutaClientes(rutaClientes), rutaVendedores(rutaVendedores), rutaAdministradores(rutaAdministradores)
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
        ofstream archivo(rutaAdministradores, ios::binary | ios::out | ios::app);
        if (!archivo.is_open()) return;
        int id = administrador->getId();
        archivo.write(reinterpret_cast<const char*>(&id), sizeof(int));
        escribirString(archivo, administrador->getNombre());
        escribirString(archivo, administrador->getCorreo());
        escribirString(archivo, administrador->getDireccion());
        escribirString(archivo, administrador->getContrasenia());
        escribirString(archivo, rolToString(administrador->getRol()));// verificar que se guarda correctamente
        string extra = administrador->getCargo();
        escribirString(archivo, extra);
    }
    // Busca por correo en ambos archivos.
    // Retorna puntero heap-allocated — el CALLER es dueno y debe hacer delete.
    // Retorna nullptr si no se encuentra.
    
    template<typename F>
    Usuario* buscarPorCondicion(F condicion) {
        // ===== CLIENTES =====
        Lista<Usuario*>* clientes = cargarDesdeArchivo(rutaClientes, ROL::CLIENTE);

        Usuario* encontrado = nullptr;

        for (uint i = 0; i < clientes->longitud(); i++) {
            Usuario* u = clientes->obtenerPos(i);

            if (u != nullptr && condicion(u)) {
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

            if (u != nullptr && condicion(u)) {
                encontrado = u;
            }
            else {
                delete u;
            }
        }

        delete vendedores;

        // ===== ADMINISTRADORES =====
        Lista<Usuario*>* administradores = cargarDesdeArchivo(rutaAdministradores, ROL::ADMINISTRADOR);
        for (uint i = 0; i < administradores->longitud(); i++) {
            Usuario* u = administradores->obtenerPos(i);
            if (u != nullptr && condicion(u)) {
                encontrado = u;
            }
            else {
                delete u;
            }
        }
        delete administradores;

        return encontrado;
    }

    // Carga todos los usuarios de ambos archivos en una sola lista.
    // El CALLER es dueno de la lista y de todos los elementos dentro.
    Lista<Usuario*>* cargarTodos() {
        Lista<Usuario*>* todos = cargarDesdeArchivo(rutaClientes, ROL::CLIENTE);
        Lista<Usuario*>* vendedores = cargarDesdeArchivo(rutaVendedores, ROL::VENDEDOR);
        for (uint i = 0; i < vendedores->longitud(); i++)
            todos->agregaFinal(vendedores->obtenerPos(i));
        while (!vendedores->esVacia()) vendedores->eliminaInicial();
        delete vendedores;

        Lista<Usuario*>* administradores = cargarDesdeArchivo(rutaAdministradores, ROL::ADMINISTRADOR);
        for (uint i = 0; i < administradores->longitud(); i++)
            todos->agregaFinal(administradores->obtenerPos(i));
        while (!administradores->esVacia()) administradores->eliminaInicial();
        delete administradores;

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
        Usuario* u = buscarPorCondicion([correo](Usuario* u) {return u->getCorreo() == correo; });
        if (u != nullptr) {
            delete u;
            return true;
        }
        return false;
    }

    bool actualizarUsuario(Usuario* usuarioActualizado) {
        if (usuarioActualizado == nullptr) return false;
        Lista<Usuario*>* clientes = cargarDesdeArchivo(rutaClientes, ROL::CLIENTE);
        Lista<Usuario*>* vendedores = cargarDesdeArchivo(rutaVendedores, ROL::VENDEDOR);
        Lista<Usuario*>* administradores = cargarDesdeArchivo(rutaAdministradores, ROL::ADMINISTRADOR);

        bool actualizado = false;

        // Reemplazar por id en la lista correspondiente
        auto reemplazar = [&](Lista<Usuario*>* lista) {
            for (uint i = 0; i < lista->longitud(); i++) {
                Usuario* u = lista->obtenerPos(i);
                if (u != nullptr && u->getId() == usuarioActualizado->getId()) {
                    // delete viejo y reemplazar puntero por una copia (caller manda ownership)
                    delete u;
                    lista->eliminaPos(i); // elimina y deja posicion i libre para insertar
                    lista->agregarPos(usuarioActualizado, i); // asume inserta existente; si no existe, coloca al final
                    actualizado = true;
                    return;
                }
            }
            };

        reemplazar(clientes);
        reemplazar(vendedores);
        reemplazar(administradores);

        // Si no estaba en ninguna lista, no se actualiza.
        if (actualizado) {
            sobrescribirClientes(clientes);
            sobrescribirVendedores(vendedores);
            sobrescribirAdministradores(administradores);
        }

        // liberar memoria restante: si usuarioActualizado quedó ya en listas no borrarlo aquí.
        // los elementos que permanecen son propiedad de las listas (ya fueron escritos).
        // Limpiar listas sin borrar los elementos que ya escribimos (no queremos doble delete).
        // Para simplicidad, vamos a eliminar todos nodos desde las listas (los objetos siguen en heap y ya se liberarán por quien sea dueño).
        // NOTA: Asumiendo contrato de propiedad consistente en el proyecto.
        while (!clientes->esVacia()) clientes->eliminaInicial();
        delete clientes;
        while (!vendedores->esVacia()) vendedores->eliminaInicial();
        delete vendedores;
        while (!administradores->esVacia()) administradores->eliminaInicial();
        delete administradores;

        return actualizado;
    }

    bool eliminarPorCorreo(const string& correo) {
        Lista<Usuario*>* clientes = cargarDesdeArchivo(rutaClientes, ROL::CLIENTE);
        Lista<Usuario*>* vendedores = cargarDesdeArchivo(rutaVendedores, ROL::VENDEDOR);
        Lista<Usuario*>* administradores = cargarDesdeArchivo(rutaAdministradores, ROL::ADMINISTRADOR);

        bool removed = false;

        auto filtrar = [&](Lista<Usuario*>* lista) {
            Lista<Usuario*>* nueva = new Lista<Usuario*>();
            for (uint i = 0; i < lista->longitud(); i++) {
                Usuario* u = lista->obtenerPos(i);
                if (u != nullptr && u->getCorreo() == correo) {
                    removed = true;
                    delete u;
                }
                else {
                    nueva->agregaFinal(u);
                }
            }
            // sobrescribir archivo correspondiente usando nueva
            lista->~Lista(); // liberar estructura antigua (sin borrar elementos porque ya los movimos a nueva or borramos)
            *lista = *nueva; // copy structure (dependiendo de implementación de Lista puede necesitar ajuste)
            delete nueva;
            };

        filtrar(clientes);
        filtrar(vendedores);
        filtrar(administradores);

        sobrescribirClientes(clientes);
        sobrescribirVendedores(vendedores);
        sobrescribirAdministradores(administradores);

        // limpiar listas
        while (!clientes->esVacia()) clientes->eliminaInicial();
        delete clientes;
        while (!vendedores->esVacia()) vendedores->eliminaInicial();
        delete vendedores;
        while (!administradores->esVacia()) administradores->eliminaInicial();
        delete administradores;

        return removed;
    }
};
