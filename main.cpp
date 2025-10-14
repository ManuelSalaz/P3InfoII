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
    char linea[150];

    while (fgets(linea, sizeof(linea), archivo)) {
        linea[strcspn(linea, "\n")] = 0;
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

            if (strcmp(usuarios[i].nombre, "sborion") == 0) {
                cout << "\nBienvenido Administrador " << usuarios[i].nombre << "!\n";
                menuAdmin(usuarios, total);
            } else {
                cout << "\nBienvenido " << usuarios[i].nombre << "!\n";
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
            cin >> nuevo.monto;
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
        fprintf(archivo, "%s,%s,%.2f\n",
                usuarios[i].nombre,
                usuarios[i].password,
                usuarios[i].monto);
    }

    fclose(archivo);
}
