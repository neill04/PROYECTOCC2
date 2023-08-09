#ifndef MENU_H
#define MENU_H

#include <memory>
#include <vector>
#include <string>
#include "constantes.h"
#include "allegro.h"

class Menu
{
public:
    static std::unique_ptr<Menu> getInstance();
    void agregarOpcion(const std::string& opcion);
    int mostrarMenu();

private:
    Menu() = default; // Constructor privado para evitar instanciación directa
    Menu(const Menu&) = delete; // Evitar copia del objeto
    Menu& operator=(const Menu&) = delete; // Evitar asignación del objeto

    std::vector<std::string> opciones;
};

#endif


