#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <Windows.h>

namespace fs = std::filesystem;

//Show a message box
void messageBox(const std::string& mensaje) {
    // Convertir std::string a LPCWSTR para MessageBox para compilar con unicode
    int longitud = MultiByteToWideChar(CP_UTF8, 0, mensaje.c_str(), -1, nullptr, 0);
    wchar_t* wMensaje = new wchar_t[longitud];
    MultiByteToWideChar(CP_UTF8, 0, mensaje.c_str(), -1, wMensaje, longitud);

    // Mostrar el mensaje
    MessageBoxW(NULL, wMensaje, L"titulo", MB_OK);

    // Liberar la memoria asignada
    delete[] wMensaje;
}

//Leer los nombres del archivo .txt
std::vector<std::string> leerNombresJuegos(const std::string& archivoJuegos) {
    std::vector<std::string> nombresJuegos;
    std::ifstream archivo(archivoJuegos);

    if (!archivo.is_open()) {
        return nombresJuegos; // Retorna el vector vacío o maneja el error según sea necesario
    }

    std::string nombre;
    while (std::getline(archivo, nombre)) {
        if (!nombre.empty()) { // Evitar agregar nombres vacíos
            nombresJuegos.push_back(nombre);
        }
    }

    archivo.close();
    return nombresJuegos;
}


// Función para mover archivos a la carpeta correspondiente.
void moverArchivo(const fs::path& archivo, const std::string& nombreJuego) {
    fs::path destino = archivo.parent_path() / nombreJuego / archivo.filename();
    fs::create_directories(destino.parent_path()); // Crea la carpeta si no existe.
    fs::rename(archivo, destino);
}

//Funcion principal
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow) {
    fs::path directorioEjecutable = fs::current_path();
    std::string nombreArchivoJuegos = directorioEjecutable.string().append("\\NOMBRE JUEGOS.txt");


    auto nombresJuegos = leerNombresJuegos(nombreArchivoJuegos);

    for (const auto& file : fs::directory_iterator(directorioEjecutable)) {
        if (file.is_regular_file() && file.path().extension() == ".mp4") {

            std::u8string u8string = file.path().filename().u8string();
            std::string nombreArchivo(u8string.begin(), u8string.end());  // Para nombres codificados en UTF-8

            for (const auto& juego : nombresJuegos) {
                // Convierte tanto nombreArchivo como juego a la misma codificación si es necesario
                if (nombreArchivo.find(juego) != std::string::npos) {
                    moverArchivo(file.path(), juego);
                    break;
                }
            }
        }
    }
    return 0;
}


