#include <iostream>
#include <fstream>
#include <bitset>
#include <string>
#include <vector>

using namespace std;

// Convierte texto a binario (string de '0' y '1')
string textoABinario_string(const string &texto) {
    string resultado;
    for (unsigned char c : texto) {
        bitset<8> bits(c);
        resultado += bits.to_string();
    }
    return resultado;
}

// Aplica el desplazamiento dentro de cada bloque
string codificarDesplazamiento(const string &bits, int semilla) {
    string resultado;
    for (size_t i = 0; i < bits.size(); i += semilla) {
        string bloque = bits.substr(i, semilla);
        string codificado = bloque; // mismo tamaño

        if (bloque.size() < (size_t)semilla)
            codificado = bloque; // no completar si último bloque es menor
        else {
            // Reglas del desplazamiento
            codificado[0] = bloque[bloque.size() - 1]; // primero ← último
            for (size_t j = 1; j < bloque.size(); j++) {
                codificado[j] = bloque[j - 1];          // resto se corre a la derecha
            }
        }

        resultado += codificado;
    }
    return resultado;
}

// Guarda una cadena binaria ("010101") como archivo binario real
void guardarComoBinario(const string &bits, const string &nombreArchivo) {
    ofstream archivo(nombreArchivo, ios::binary);
    if (!archivo) {
        cerr << "Error al abrir el archivo de salida." << endl;
        return;
    }

    // Convertir cada grupo de 8 bits a un byte
    for (size_t i = 0; i < bits.size(); i += 8) {
        string byteStr = bits.substr(i, 8);
        if (byteStr.size() < 8) break; // ignorar bits sobrantes
        bitset<8> byte(byteStr);
        unsigned char c = static_cast<unsigned char>(byte.to_ulong());
        archivo.write(reinterpret_cast<const char*>(&c), 1);
    }

    archivo.close();
}

int main() {
    string nombreFuente, nombreSalida;
    int semilla=4;

    cout << "Ingrese el nombre del archivo fuente (texto): ";
    cin >> nombreFuente;

    cout << "Ingrese el nombre del archivo de salida (binario): ";
    cin >> nombreSalida;


    // Leer archivo de entrada (texto)
    ifstream fuente(nombreFuente, ios::binary);
    if (!fuente) {
        cerr << "No se pudo abrir el archivo fuente." << endl;
        return 1;
    }

    string contenido((istreambuf_iterator<char>(fuente)), istreambuf_iterator<char>());
    fuente.close();

    // Convertir a binario
    string binario = textoABinario_string(contenido);

    // Codificar con desplazamiento
    string codificado = codificarDesplazamiento(binario, semilla);

    // Guardar archivo binario resultante
    guardarComoBinario(codificado, nombreSalida);

    cout << "Bits originales:  " << binario.substr(0, 64) << "\n";
    cout << "Bits codificados: " << codificado.substr(0, 64) << "\n";
    cout << "Archivo binario generado: " << nombreSalida << endl;

    return 0;
}
