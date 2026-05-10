#pragma once
#include <iostream>
#include <string>
#include <ctime>
using namespace std;
static string obtenerFechaActual() {

    time_t t = time(nullptr);
    tm* now = localtime(&t);

    int anio = now->tm_year + 1900;
    int mes = now->tm_mon + 1;
    int dia = now->tm_mday;

    string fecha = "";

    fecha += to_string(anio);
    fecha += "-";

    if (mes < 10) fecha += "0";
    fecha += to_string(mes);
    fecha += "-";

    if (dia < 10) fecha += "0";
    fecha += to_string(dia);

    return fecha;
}

string generarFechaEntrega() {

    // Fecha actual
    time_t t = time(nullptr);

    // Numero aleatorio entre 3 y 15
    int diasExtra = rand() % 13 + 3;

    // Sumar dias en segundos
    t += diasExtra * 24 * 60 * 60;

    tm* fecha = localtime(&t);

    int anio = fecha->tm_year + 1900;
    int mes = fecha->tm_mon + 1;
    int dia = fecha->tm_mday;

    string resultado = "";

    resultado += to_string(anio);
    resultado += "-";

    if (mes < 10) resultado += "0";
    resultado += to_string(mes);
    resultado += "-";

    if (dia < 10) resultado += "0";
    resultado += to_string(dia);

    return resultado;
}
