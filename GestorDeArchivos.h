#pragma once
#include <fstream>
#include <string>
using namespace std;

// GestorDeArchivos<T> — utilitario de I/O binario para tipos POD.
// IMPORTANTE: No usar directamente con clases que contengan std::string u otros
// miembros no-POD (como Usuario, Cliente, Vendedor). Para esos casos, usar
// UsuarioRepository, que implementa serializacion length-prefixed propia.
template <typename T>
class GestorDeArchivos {
private:
    string ruta;

public:
    explicit GestorDeArchivos(const string& ruta) : ruta(ruta) {}

    // Guarda un elemento POD. append=true agrega al final del archivo.
    bool guardar(const T& dato, bool append = true) {
        ios::openmode modo = ios::binary | ios::out;
        if (append) modo |= ios::app;
        ofstream archivo(ruta, modo);
        if (!archivo.is_open()) return false;
        archivo.write(reinterpret_cast<const char*>(&dato), sizeof(T));
        return archivo.good();
    }

    // Lee el primer elemento POD del archivo en 'dato'.
    bool cargar(T& dato) {
        ifstream archivo(ruta, ios::binary | ios::in);
        if (!archivo.is_open()) return false;
        archivo.read(reinterpret_cast<char*>(&dato), sizeof(T));
        return archivo.gcount() == sizeof(T);
    }

    // Guarda un arreglo de 'count' elementos POD (sobreescribe el archivo).
    bool guardarColeccion(const T* datos, int count) {
        ofstream archivo(ruta, ios::binary | ios::out);
        if (!archivo.is_open()) return false;
        archivo.write(reinterpret_cast<const char*>(datos), sizeof(T) * count);
        return archivo.good();
    }

    // Carga todos los elementos POD en un arreglo dinamico.
    // Retorna la cantidad de elementos leidos. El CALLER debe hacer delete[] en datos.
    int cargarColeccion(T*& datos) {
        ifstream archivo(ruta, ios::binary | ios::in);
        if (!archivo.is_open()) { datos = nullptr; return 0; }
        archivo.seekg(0, ios::end);
        streamsize tamano = archivo.tellg();
        archivo.seekg(0, ios::beg);
        int count = (int)(tamano / sizeof(T));
        if (count <= 0) { datos = nullptr; return 0; }
        datos = new T[count];
        archivo.read(reinterpret_cast<char*>(datos), sizeof(T) * count);
        return count;
    }

    bool existeArchivo() const {
        ifstream archivo(ruta, ios::binary | ios::in);
        return archivo.is_open();
    }

    bool limpiarArchivo() {
        ofstream archivo(ruta, ios::binary | ios::out | ios::trunc);
        return archivo.is_open();
    }

    const string& getRuta() const { return ruta; }
};

// Especializacion para string — opera en modo texto (linea por linea).
template <>
class GestorDeArchivos<string> {
private:
    string ruta;

public:
    explicit GestorDeArchivos(const string& ruta) : ruta(ruta) {}

    bool guardar(const string& dato, bool append = true) {
        ios::openmode modo = ios::out;
        if (append) modo |= ios::app;
        ofstream archivo(ruta, modo);
        if (!archivo.is_open()) return false;
        archivo << dato << "\n";
        return archivo.good();
    }

    bool cargar(string& dato) {
        ifstream archivo(ruta, ios::in);
        if (!archivo.is_open()) return false;
        return (bool)getline(archivo, dato);
    }

    bool existeArchivo() const {
        ifstream archivo(ruta, ios::in);
        return archivo.is_open();
    }

    bool limpiarArchivo() {
        ofstream archivo(ruta, ios::out | ios::trunc);
        return archivo.is_open();
    }

    const string& getRuta() const { return ruta; }
};
