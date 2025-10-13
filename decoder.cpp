#include <iostream>
#include <fstream>
#include <bitset>
#include <string>
#include <vector>
using namespace std;

// ===================================================
// FUNCIONES AUXILIARES
// ===================================================
string leerArchivoTextoBits(const string &nombreArchivo) {
    ifstream archivo(nombreArchivo);
    if (!archivo) {
        cerr << "Error al abrir el archivo de entrada (texto de bits)." << endl;
        return "";
    }
    string contenido((istreambuf_iterator<char>(archivo)), istreambuf_iterator<char>());
    archivo.close();

    // Eliminar posibles espacios o saltos de línea
    string limpio;
    for (char c : contenido)
        if (c == '0' || c == '1') limpio += c;

    return limpio;
}

string leerArchivoBinario(const string &nombreArchivo) {
    ifstream archivo(nombreArchivo, ios::binary);
    if (!archivo) {
        cerr << "Error al abrir el archivo binario." << endl;
        return "";
    }

    string bits;
    unsigned char byte;
    while (archivo.read(reinterpret_cast<char*>(&byte), 1)) {
        bitset<8> b(byte);
        bits += b.to_string();
    }
    archivo.close();
    return bits;
}

string binarioATexto(const string &bits) {
    string texto;
    for (size_t i = 0; i + 7 < bits.size(); i += 8) {
        bitset<8> b(bits.substr(i, 8));
        texto += static_cast<char>(b.to_ulong());
    }
    return texto;
}

string invertirBits(const string &bloque) {
    string resultado = bloque;
    for (char &c : resultado)
        c = (c == '1') ? '0' : '1';
    return resultado;
}

// ===================================================
// MÉTODO 1 – Reglas según 1s y 0s
// ===================================================
string decodificarMetodo1(const string &bits, int semilla) {
    vector<string> bloques;
    for (size_t i = 0; i < bits.size(); i += semilla)
        bloques.push_back(bits.substr(i, semilla));

    if (bloques.empty()) return "";

    // Primer bloque: invertir completamente
    bloques[0] = invertirBits(bloques[0]);

    for (size_t i = 1; i < bloques.size(); i++) {
        int unos = 0, ceros = 0;
        for (char c : bloques[i - 1]) {
            if (c == '1') unos++;
            else ceros++;
        }

        string bloque = bloques[i];

        if (unos == ceros) {
            bloque = invertirBits(bloque);
        } 
        else if (ceros > unos) {
            for (int j = 1; j < semilla; j += 2)
                bloque[j] = (bloque[j] == '1') ? '0' : '1';
        } 
        else {
            for (int j = 2; j < semilla; j += 3)
                bloque[j] = (bloque[j] == '1') ? '0' : '1';
        }

        bloques[i] = bloque;
    }

    string resultado;
    for (auto &b : bloques) resultado += b;
    return resultado;
}

// ===================================================
// MÉTODO 2 – Desplazamiento circular
// ===================================================
string decodificarMetodo2(const string &bits, int semilla) {
    string resultado;
    for (size_t i = 0; i < bits.size(); i += semilla) {
        string bloque = bits.substr(i, semilla);
        if (bloque.size() < (size_t)semilla) {
            resultado += bloque;
            continue;
        }

        string decodificado = bloque;
        decodificado[semilla - 1] = bloque[0];
        for (int j = 1; j < semilla; j++) {
            decodificado[j - 1] = bloque[j];
        }

        resultado += decodificado;
    }
    return resultado;
}

// ===================================================
// PROGRAMA PRINCIPAL
// ===================================================
int main() {
    string archivoEntrada, archivoSalida;
    int semilla, metodo;

    cout << "Ingrese el nombre del archivo de entrada: ";
    cin >> archivoEntrada;

    cout << "Ingrese el nombre del archivo de salida (.txt): ";
    cin >> archivoSalida;

    cout << "Ingrese la semilla (n bits por bloque): ";
    cin >> semilla;

    cout << "Ingrese el metodo de codificacion (1 o 2): ";
    cin >> metodo;

    string bits;

    if (metodo == 1) {
        bits = leerArchivoTextoBits(archivoEntrada); // ← lee texto plano con '0' y '1'
    } 
    else if (metodo == 2) {
        bits = leerArchivoBinario(archivoEntrada);   // ← lee bytes reales
    } 
    else {
        cerr << "Metodo no valido." << endl;
        return 1;
    }

    if (bits.empty()) {
        cerr << "No se leyeron datos del archivo." << endl;
        return 1;
    }

    string bitsDecodificados;

    if (metodo == 1)
        bitsDecodificados = decodificarMetodo1(bits, semilla);
    else
        bitsDecodificados = decodificarMetodo2(bits, semilla);

    string texto = binarioATexto(bitsDecodificados);

    ofstream salida(archivoSalida);
    if (!salida) {
        cerr << "Error al crear el archivo de salida." << endl;
        return 1;
    }
    salida << texto;
    salida.close();

    cout << "Texto recuperado: " << texto << endl;
    cout << "Archivo guardado: " << archivoSalida << endl;

    return 0;
}
