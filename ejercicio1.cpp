#include <iostream>
#include <bitset>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

string textoABinario_string(const string &texto) {
    string resultado = "";
    for (char c : texto) {
        bitset<8> binario(c);
        resultado += binario.to_string();  // concatenamos los bits
    }
    return resultado;
}

// Convierte texto normal a bits (8 bits por carácter)
string textoABinario(const string &texto) {
    string bits;
    for (unsigned char c : texto) {
        for (int i = 7; i >= 0; i--)
            bits += ((c >> i) & 1) ? '1' : '0';
    }
    return bits;
}


string separarEnBloques(const string &bits, int semilla) {
    string resultado = "";
    for (size_t i = 0; i < bits.size(); i += semilla)
        resultado += bits.substr(i, semilla) + " ";
    return resultado;
}

string invertirBits(const string &bloque) {
    string resultado = bloque;
    for (char &c : resultado)
        c = (c == '1') ? '0' : '1';
    return resultado;
}

string codificarBloques(const string &bits, int semilla) {
    vector<string> bloquesOriginales;
    vector<string> bloquesCodificados;

    // Separar en bloques de tamaño 'semilla'
    for (size_t i = 0; i < bits.size(); i += semilla) {
        string bloque = bits.substr(i, semilla);
        bloquesOriginales.push_back(bloque);
        bloquesCodificados.push_back(bloque);
    }

    if (bloquesOriginales.empty()) return "";

    // --- Primer bloque: invertir todos los bits ---
    bloquesCodificados[0] = invertirBits(bloquesCodificados[0]);

    // --- Bloques siguientes ---
    for (size_t i = 1; i < bloquesCodificados.size(); i++) {
        int unos = 0, ceros = 0;

        // Contar 1s y 0s en el bloque anterior ORIGINAL
        for (char c : bloquesOriginales[i - 1]) {
            if (c == '1') unos++;
            else ceros++;
        }

        string bloque = bloquesCodificados[i];

        if (unos == ceros) {
            // Invertir todos los bits
            bloque = invertirBits(bloque);
        } 
        else if (ceros > unos) {
            // Invertir cada 2 bits
            for (int j = 1; j < semilla && j < (int)bloque.size(); j += 2)
                bloque[j] = (bloque[j] == '1') ? '0' : '1';
        } 
        else {
            // Invertir cada 3 bits
            for (int j = 2; j < semilla && j < (int)bloque.size(); j += 3)
                bloque[j] = (bloque[j] == '1') ? '0' : '1';
        }

        bloquesCodificados[i] = bloque;
    }

    // --- Unir todos los bloques codificados ---
    string resultado = "";
    for (const string &b : bloquesCodificados)
        resultado += b;

    return resultado;
}

int main() {
    int semilla;
    string archivoSalida;

    string nombreFuente;
    string nombreSalida;

    cout << "Ingrese el nombre del archivo fuente (texto): ";
    cin >> nombreFuente;

    ifstream fuente(nombreFuente, ios::binary);
    if (!fuente) {
        cerr << "No se pudo abrir el archivo fuente." << endl;
        return 1;
    }

    string contenido((istreambuf_iterator<char>(fuente)), istreambuf_iterator<char>());
    fuente.close();

    cout << "Ingrese la semilla (n bits por bloque): ";
    cin >> semilla;

    cout << "Ingrese el nombre del archivo de salida (.txt): ";
    cin >> archivoSalida;

    string binario = textoABinario_string(contenido);
    cout << "\nTexto en binario:\n" << binario << endl;

    string codificado = codificarBloques(binario, semilla);
    cout << "\nTexto codificado (agrupado de " << semilla << " bits):\n" 
         << separarEnBloques(codificado, semilla) << endl;

    // --- Guardar en archivo de salida ---
    ofstream salida(archivoSalida);
    if (!salida) {
        cerr << "Error al crear el archivo de salida." << endl;
        return 1;
    }

    salida << codificado;
    salida.close();
    cout << "\nCodificación completada exitosamente.\n";
    return 0;
}
