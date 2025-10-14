#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
using namespace std;

struct Usuario {
    char nombre[50];
    char password[50];
    double monto;
};

int main() {
    FILE *archivo = fopen("sudo.txt", "r");
    if (!archivo) {
        cout << "No se pudo abrir el archivo usuarios.txt\n";
        return 1;
    }

    Usuario usuarios[100];
    int total = 0;
    char linea[150];

    while (fgets(linea, sizeof(linea), archivo)) {
        linea[strcspn(linea, "\n")] = 0; // eliminar salto de línea

        char *token = strtok(linea, ",");
        if (!token) continue;
        strcpy(usuarios[total].nombre, token);

        token = strtok(nullptr, ",");
        if (!token) continue;
        strcpy(usuarios[total].password, token);

        token = strtok(nullptr, ",");
        usuarios[total].monto = token ? atof(token) : 0;

        total++;
    }

    fclose(archivo);

    cout << "Usuarios cargados: " << total << endl;

    // Login
    char user[50], pass[50];
    cout << "Usuario: ";
    cin.getline(user, 50);
    cout << "Password: ";
    cin.getline(pass, 50);

    bool encontrado = false;
    for (int i = 0; i < total; i++) {
        if (strcmp(user, usuarios[i].nombre) == 0 &&
            strcmp(pass, usuarios[i].password) == 0) {
            cout << "Bienvenido " << usuarios[i].nombre
                 << ", tu saldo es: " << usuarios[i].monto << endl;
            encontrado = true;
            break;
        }
    }

    if (!encontrado) cout << "Usuario o contraseña incorrectos.\n";
    return 0;
}
