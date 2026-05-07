#pragma once
#include <string>
using namespace std;

// AVISO DE SEGURIDAD: djb2 NO es una funcion criptografica.
// No tiene salt ni key-stretching. Vulnerable a rainbow tables.
// Uso exclusivamente educativo/academico. En produccion usar bcrypt o Argon2.
class HashUtils {
public:
    static string hashear(const string& input) {
        unsigned long hash = 5381;
        for (char c : input)
            hash = ((hash << 5) + hash) + (unsigned char)c;

        // Conversion manual a hex string (8 chars, 32 bits del hash)
        const char* hexDigits = "0123456789abcdef";
        string resultado = "";
        unsigned long valor = hash;
        for (int i = 0; i < 8; i++) {
            resultado = hexDigits[valor & 0xF] + resultado;
            valor >>= 4;
        }
        return resultado;
    }
};
