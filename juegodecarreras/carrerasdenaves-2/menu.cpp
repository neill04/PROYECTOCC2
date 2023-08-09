#include "menu.h"
#include <allegro.h>
#include <allegro/font.h>
#include <memory>
#include <iostream>

std::unique_ptr<Menu> Menu::getInstance()
{
    static std::unique_ptr<Menu> instance(new Menu);
    return std::move(instance);
}

void Menu::agregarOpcion(const std::string& opcion)
{
    opciones.push_back(opcion);
}

int Menu::mostrarMenu()
{
    BITMAP* buffer = create_bitmap(SCREEN_W, SCREEN_H);
    bool exit_menu = false;
    int selected_option = 0;

    while (!exit_menu)
    {
        if (key[KEY_UP])
        {
            selected_option--;
            if (selected_option < 0)
                selected_option = opciones.size() - 1;
        }
        if (key[KEY_DOWN])
        {
            selected_option++;
            if (selected_option >= opciones.size())
                selected_option = 0;
        }

        if (key[KEY_ENTER])
        {
            if (selected_option == opciones.size() - 1)
            {
                // Opción "Salir" seleccionada, cerrar la aplicación
                return opciones.size() - 1;
            }
            else
            {
                // Opción "Jugar" seleccionada, salir del menú
                break;
            }
        }

        clear_to_color(buffer, makecol(0, 0, 0));
        // Dibujar el título del juego con borde
        for (int dy = -2; dy <= 2; dy++) {
            for (int dx = -2; dx <= 2; dx++) {
                if (dx == 0 && dy == 0) continue; // Saltar el dibujo central para evitar sombras
                textout_centre_ex(buffer, font, "GRAVITY", SCREEN_W / 2 + dx, 100 + dy, makecol(0, 0, 0), -1);
            }
        }
        textout_centre_ex(buffer, font, "GRAVITY", SCREEN_W / 2, 100, makecol(255, 255, 255), -1);

        int y = 200;
        for (size_t i = 0; i < opciones.size(); ++i)
        {
            if (i == selected_option)
                textprintf_centre_ex(buffer, font, SCREEN_W / 2, y, makecol(255, 255, 255), -1, "> %s <", opciones[i].c_str());
            else
                textprintf_centre_ex(buffer, font, SCREEN_W / 2, y, makecol(255, 255, 255), -1, opciones[i].c_str());
            y += 30;
        }

        blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
        rest(1000 / FPS);
    }

    destroy_bitmap(buffer);
    return selected_option;
}

