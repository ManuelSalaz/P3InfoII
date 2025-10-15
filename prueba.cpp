#include <iostream>
#include <bitset>
#include <string>
using namespace std;

string textoABinario_string(const string &texto) {
    string resultado;
    for (unsigned char c : texto) {
        bitset<8> bits(c);
        resultado += bits.to_string();
    }
    return resultado;
}

string codificarDesplazamiento(const string &bits, int semilla) {
    string resultado;
    for (size_t i = 0; i < bits.size(); i += semilla) {
        string bloque = bits.substr(i, semilla);
        string codificado = bloque;
        if (bloque.size() >= (size_t)semilla) {
            codificado[0] = bloque[bloque.size() - 1];
            for (size_t j = 1; j < bloque.size(); j++) codificado[j] = bloque[j - 1];
        }
        resultado += codificado;
    }
    return resultado;
}

string encriptar(const string &texto) {
    int semilla = 4;
    string binario = textoABinario_string(texto);
    string codificado = codificarDesplazamiento(binario, semilla);
    string resultado;
    for (size_t i = 0; i < codificado.size(); i += 8) {
        string byteStr = codificado.substr(i, 8);
        if (byteStr.size() < 8) break;
        bitset<8> byte(byteStr);
        resultado += static_cast<unsigned char>(byte.to_ulong());
    }
    return resultado;
}

int main() {
    cout << "Clave original: ";
    string clave;
    cin >> clave;
    cout << "Clave encriptada: " << encriptar(clave) << endl;
    return 0;
}
