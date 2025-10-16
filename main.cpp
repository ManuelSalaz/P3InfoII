#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <vector>
using namespace std;

struct Usuario {
    char nombre[50];
    char password[50];
    double monto;
};

string invertirBits(const string &bloque) {
    string resultado = bloque;
    for (char &c : resultado)
        c = (c == '1') ? '0' : '1';
    return resultado;
}

string decodificarMetodo1(const string &bits, int semilla) {
    vector<string> bloques;
    for (size_t i = 0; i < bits.size(); i += semilla)
        bloques.push_back(bits.substr(i, semilla));

    if (bloques.empty()) return "";

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
            for (int j = 1; j < (int)semilla; j += 2)
                bloque[j] = (bloque[j] == '1') ? '0' : '1';
        } 
        else {
            for (int j = 2; j < (int)semilla; j += 3)
                bloque[j] = (bloque[j] == '1') ? '0' : '1';
        }

        bloques[i] = bloque;
    }

    string resultado;
    for (auto &b : bloques) resultado += b;
    return resultado;
}

string binarioATexto(const string &bits) {
    string texto;
    for (size_t i = 0; i + 7 < bits.size(); i += 8) {
        string byte = bits.substr(i, 8);
        char c = static_cast<char>(stoi(byte, nullptr, 2));
        texto.push_back(c);
    }
    return texto;
}

string textoABinario(const string &texto) {
    string bits;
    for (unsigned char c : texto) {
        for (int i = 7; i >= 0; i--)
            bits += ((c >> i) & 1) ? '1' : '0';
    }
    return bits;
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


void menuAdmin(Usuario usuarios[], int &total); 
void menuUsuario(Usuario usuarios[], int indice, int total); 
void guardarUsuarios(Usuario usuarios[], int total);

int main() {
    FILE *archivo = fopen("sudo.txt", "r");
    if (!archivo) {
        cout << "No se pudo abrir el archivo sudo.txt\n";
        return 1;
    }

    Usuario usuarios[100];
    int total = 0;
    char linea[200];

    while (fgets(linea, sizeof(linea), archivo)) {
        linea[strcspn(linea, "\n")] = 0;
        char *token = strtok(linea, ",");
        if (!token) continue;
        strcpy(usuarios[total].nombre, token);

        token = strtok(nullptr, ",");
        if (!token) continue;
        string passBinario = token;

        // 🔹 Desencriptar y convertir binario a texto
        string desencriptado = decodificarMetodo1(passBinario, 4); // semilla = 4
        string textoPlano = binarioATexto(desencriptado);
        //cout << "Usuario: " << usuarios[total].nombre 
             //<< " | Password (bin): " << passBinario 
             //<< "| Desencriptado: " << desencriptado
             //<< " | Password (text): " << textoPlano << endl;
        strcpy(usuarios[total].password, textoPlano.c_str());

        token = strtok(nullptr, ",");
        usuarios[total].monto = token ? atof(token) : 0;

        total++;
    }

    fclose(archivo);

    cout << "Usuarios cargados: " << total << endl;

    char user[50], pass[50];
    cout << "Cédula o Usuario: ";
    cin.getline(user, 50);
    cout << "Clave: ";
    cin.getline(pass, 50);

    bool encontrado = false;

    for (int i = 0; i < total; i++) {
        if (strcmp(user, usuarios[i].nombre) == 0 &&
            strcmp(pass, usuarios[i].password) == 0) {

            encontrado = true;
            cout << "\nBienvenido " << usuarios[i].nombre << "!\n";
            if (strcmp(user, "admin") == 0) {
                menuAdmin(usuarios, total);
            } else {
                menuUsuario(usuarios, i, total);
            }
            break;
        }
    }

    if (!encontrado)
        cout << "Usuario o contraseña incorrectos.\n";

    return 0;
}

void menuAdmin(Usuario usuarios[], int &total) {
    int opcion;
    do {
        cout << "\n===== MENU ADMINISTRADOR =====\n";
        cout << "1. Registrar nuevo usuario\n";
        cout << "2. Ver todos los usuarios\n";
        cout << "3. Salir\n";
        cout << "Opcion: ";
        cin >> opcion;
        cin.ignore();

        if (opcion == 1) {
            if (total >= 100) {
                cout << "No se pueden registrar más usuarios.\n";
                continue;
            }
            Usuario nuevo;
            cout << "\n--- Registro de nuevo usuario ---\n";
            cout << "Ingrese cédula: ";
            cin.getline(nuevo.nombre, 50);

            bool existe = false;
            for (int i = 0; i < total; i++) {
                if (strcmp(nuevo.nombre, usuarios[i].nombre) == 0) {
                    cout << "Ya existe un usuario con esa cédula.\n";
                    existe = true;
                    break;
                }
            }
            if (existe) continue;

            cout << "Ingrese clave: ";
            cin.getline(nuevo.password, 50);
            cout << "Ingrese saldo inicial (COP): ";
            while (!(cin >> nuevo.monto)) {
                cout << "Entrada inválida. Por favor, ingrese un número: ";
                cin.clear();                // limpia el estado de error de cin
                cin.ignore(10000, '\n');    // descarta la entrada no válida
            }
            cin.ignore();

            usuarios[total++] = nuevo;
            guardarUsuarios(usuarios, total);

            cout << "Usuario registrado correctamente.\n";

        } else if (opcion == 2) {
            cout << "\n--- Lista de usuarios ---\n";
            for (int i = 0; i < total; i++) {
                cout << i + 1 << ". " << usuarios[i].nombre
                     << " | Saldo: " << usuarios[i].monto << endl;
            }
        } else if (opcion == 3) {
            cout << "Saliendo del menú administrador...\n";
        } else {
            cout << "Opción inválida.\n";
        }

    } while (opcion != 3);
}

void menuUsuario(Usuario usuarios[], int indice, int total) {
    int opcion;
    do {
        cout << "\n===== MENU USUARIO =====\n";
        cout << "1. Ver saldo (costo 1000 COP)\n";
        cout << "2. Retirar\n";
        cout << "3. Salir\n";
        cout << "Opcion: ";
        cin >> opcion;

        if (opcion == 1) {
            if (usuarios[indice].monto >= 1000) {
                usuarios[indice].monto -= 1000;
                guardarUsuarios(usuarios, total);
                cout << "Tu saldo actual es: " << usuarios[indice].monto << " COP\n";
            } else {
                cout << "Saldo insuficiente para consultar.\n";
            }
        } 
        else if (opcion == 2) {
            double ret;
            cout << "Cantidad a retirar (COP): ";
            cin >> ret;
            if (ret > 0 && ret <= (usuarios[indice].monto - 1000)) {
                usuarios[indice].monto -= ret;
                usuarios[indice].monto -= 1000;
                guardarUsuarios(usuarios, total);
                cout << "Retiro exitoso. Nuevo saldo: " << usuarios[indice].monto << " COP\n";
            } else {
                cout << "No se puede retirar esa cantidad.\n";
            }
        } 
        else if (opcion == 3) {
            cout << "Cerrando sesión...\n";
        } 
        else {
            cout << "Opción inválida.\n";
        }

    } while (opcion != 3);
}

void guardarUsuarios(Usuario usuarios[], int total) {
    FILE *archivo = fopen("sudo.txt", "w");
    if (!archivo) {
        cout << "Error al guardar los datos.\n";
        return;
    }

    for (int i = 0; i < total; i++) {
        // 🔹 Convertir password a binario y encriptar
        string passTexto = usuarios[i].password;
        string binario = textoABinario(passTexto);
        string codificado = codificarBloques(binario, 4); // misma semilla que usas al leer

        fprintf(archivo, "%s,%s,%.2f\n",
                usuarios[i].nombre,
                codificado.c_str(),
                usuarios[i].monto);
    }

    fclose(archivo);
}
