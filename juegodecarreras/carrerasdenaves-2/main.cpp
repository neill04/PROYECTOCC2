#include <allegro.h>
#include <iostream>
#include "constantes.h"
#include "menu.h"
#include "nave.h"
#include "pista.h"
#include "enemy.h"
#include "proyectil.h"
#include "colisiones.h"
#include "gamecontroller.h"
#include <vector>
#include <cstdlib>
#include <cassert>
#include <stdlib.h>
#include <cmath>
#include <string>
#include <memory>
#include <ctime>

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

const int DIALOG_WIDTH = 300; // Ancho del cuadro de diálogo
const int DIALOG_HEIGHT = 200; // Alto del cuadro de diálogo
const int BUTTON_WIDTH = 120; // Ancho de los botones
const int BUTTON_HEIGHT = 30; // Alto de los botones
const int BUTTON_SPACING = 20; // Espaciado entre los botones

enum class GameOverOption { PlayAgain, Quit };

bool colisionCircular(int x1, int y1, int radio1, int x2, int y2, int radio2)
{
    // Calcula la distancia entre los dos puntos usando la fórmula de la distancia Euclidiana
    int distanciaX = x2 - x1;
    int distanciaY = y2 - y1;
    int distancia = std::sqrt(distanciaX * distanciaX + distanciaY * distanciaY);

    // Compara la distancia con la suma de los radios de los objetos
    return distancia < radio1 + radio2;
}

bool colisionRectangular(const Nave& nave, const Pista& pista)
{
    // Verificar si hay colisión entre la nave y la pista mediante rectángulos circunscritos
    if (nave.getX() < pista.getX() + pista.getWidth() &&
        nave.getX() + nave.getWidth() > pista.getX() &&
        nave.getY() < pista.getY() + pista.getHeight() &&
        nave.getY() + nave.getHeight() > pista.getY())
    {
        return true;
    }

    return false;
}

// Función para mostrar el cuadro de diálogo de Game Over con opciones de volver a jugar o salir
GameOverOption showGameOverDialog()
{
    BITMAP* dialogBuffer = create_bitmap(DIALOG_WIDTH, DIALOG_HEIGHT);
    clear_to_color(dialogBuffer, makecol(192, 192, 192)); // Fondo gris claro

    // Dibujar el mensaje de Game Over en el cuadro de diálogo
    textout_centre_ex(dialogBuffer, font, "Game Over", DIALOG_WIDTH / 2, 50, makecol(0, 0, 0), -1);

    // Dibujar los botones de las opciones
    int button1X = (DIALOG_WIDTH - BUTTON_WIDTH) / 2;
    int button1Y = DIALOG_HEIGHT / 2 - BUTTON_HEIGHT / 2;
    int button2X = button1X;
    int button2Y = button1Y + BUTTON_HEIGHT + BUTTON_SPACING;

    rectfill(dialogBuffer, button1X, button1Y, button1X + BUTTON_WIDTH, button1Y + BUTTON_HEIGHT, makecol(255, 255, 255)); // Botón blanco
    rectfill(dialogBuffer, button2X, button2Y, button2X + BUTTON_WIDTH, button2Y + BUTTON_HEIGHT, makecol(255, 255, 255)); // Botón blanco
    rect(dialogBuffer, button1X, button1Y, button1X + BUTTON_WIDTH, button1Y + BUTTON_HEIGHT, makecol(0, 0, 0)); // Borde negro
    rect(dialogBuffer, button2X, button2Y, button2X + BUTTON_WIDTH, button2Y + BUTTON_HEIGHT, makecol(0, 0, 0)); // Borde negro

    textout_centre_ex(dialogBuffer, font, "Volver a jugar", button1X + BUTTON_WIDTH / 2, button1Y + BUTTON_HEIGHT / 2 - 10, makecol(0, 0, 0), -1);
    textout_centre_ex(dialogBuffer, font, "Salir del juego", button2X + BUTTON_WIDTH / 2, button2Y + BUTTON_HEIGHT / 2 - 10, makecol(0, 0, 0), -1);

    // Mostrar el cuadro de diálogo en la pantalla
    int dialogX = (SCREEN_WIDTH - DIALOG_WIDTH) / 2;
    int dialogY = (SCREEN_HEIGHT - DIALOG_HEIGHT) / 2;
    blit(dialogBuffer, screen, 0, 0, dialogX, dialogY, DIALOG_WIDTH, DIALOG_HEIGHT);

    // Esperar a que el usuario haga una selección
    bool selectionMade = false;
    int selectedOption = 0;

    while (!selectionMade)
    {
        if (key[KEY_DOWN])
        {
            selectedOption = (selectedOption + 1) % 2;
            rest(200); // Retardo para evitar cambio rápido de selección
        }
        else if (key[KEY_UP])
        {
            selectedOption = (selectedOption - 1 + 2) % 2;
            rest(200); // Retardo para evitar cambio rápido de selección
        }
        else if (key[KEY_ENTER])
        {
            selectionMade = true;
        }

        // Resaltar la opción seleccionada
        clear_to_color(dialogBuffer, makecol(192, 192, 192));
        textout_centre_ex(dialogBuffer, font, "Game Over", DIALOG_WIDTH / 2, 50, makecol(0, 0, 0), -1);

        if (selectedOption == 0)
        {
            rectfill(dialogBuffer, button1X, button1Y, button1X + BUTTON_WIDTH, button1Y + BUTTON_HEIGHT, makecol(200, 200, 200));
        }
        else
        {
            rectfill(dialogBuffer, button2X, button2Y, button2X + BUTTON_WIDTH, button2Y + BUTTON_HEIGHT, makecol(200, 200, 200));
        }

        textout_centre_ex(dialogBuffer, font, "Volver a jugar", button1X + BUTTON_WIDTH / 2, button1Y + BUTTON_HEIGHT / 2 - 10, makecol(0, 0, 0), -1);
        textout_centre_ex(dialogBuffer, font, "Salir del juego", button2X + BUTTON_WIDTH / 2, button2Y + BUTTON_HEIGHT / 2 - 10, makecol(0, 0, 0), -1);

        blit(dialogBuffer, screen, 0, 0, dialogX, dialogY, DIALOG_WIDTH, DIALOG_HEIGHT);
    }

    destroy_bitmap(dialogBuffer);

    return (selectedOption == 0) ? GameOverOption::PlayAgain : GameOverOption::Quit;
}

int main()
{
    srand(time(NULL));
    allegro_init();
    install_sound(DIGI_AUTODETECT, MIDI_NONE, NULL);
    install_keyboard();
    set_color_depth(32);
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0);
    set_window_title("Carreras en Gravedad Cero");

    BITMAP* buffer = create_bitmap(SCREEN_WIDTH, SCREEN_HEIGHT);

    bool exit_game = false;
    Nave nave(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 50); // Ahora debería reconocer la clase Nave

    // Cargar la música desde un archivo WAV
    SAMPLE* musica = load_wav("musica/interestellar.wav");
    if (!musica)
    {
        allegro_message("Error al cargar la musica.");
        return 1;
    }

    // Reproducir la música en bucle
    play_sample(musica, 255, 128, 1000, true);

    // Crea una instancia del menú utilizando el patrón Singleton con smart pointers
    std::unique_ptr<Menu> menu = Menu::getInstance();

    // Agrega las opciones al menú
    menu->agregarOpcion("Jugar");
    menu->agregarOpcion("Salir");

    int opcionSeleccionada = menu->mostrarMenu();

    // Verifica la opción seleccionada
    if (opcionSeleccionada == 0)
    {
        // El jugador seleccionó "Jugar"
        // Aquí puedes agregar el código para iniciar el juego
    }
    else if (opcionSeleccionada == 1)
    {
        // El jugador seleccionó "Salir"
        allegro_exit();
        return 0;
    }

    std::vector<Enemy> enemigos;
    std::vector<Pista> pistas;
    GameController gameController(enemigos);

    // Crear instancias de Enemy y pasar la referencia al GameController
    int x = 100; // Ejemplo de coordenada x
    int y = 200; // Ejemplo de coordenada y
    enemigos.push_back(Enemy(x, y, &gameController));

    // Agregar enemigos a la colección
    for (int i = 0; i < 3; ++i)
    {
        int x = rand() % SCREEN_WIDTH;
        int y = rand() % SCREEN_HEIGHT;
        enemigos.push_back(Enemy(x, y, &gameController));
    }

    // Agregamos pistas flotantes a la colección
    for (int i = 0; i < 5; ++i)
    {
        int x = rand() % SCREEN_WIDTH; // Posición x aleatoria
        int y = rand() % SCREEN_HEIGHT; // Posición y aleatoria
        int width = 100; // Ancho de la pista
        int height = 20; // Alto de la pista

        double velocidadX = (rand() % 3) - 1;
        double velocidadY = (rand() % 3) - 1;

        pistas.push_back(Pista(x, y, width, height));
        pistas[i].setVelocidadX(velocidadX);
        pistas[i].setVelocidadY(velocidadY);
    }

    bool gameOver = false;

    while (!exit_game)
    {
        if (key[KEY_ESC])
            exit_game = true;

        // Manejar las entradas del teclado para mover la nave
        if (key[KEY_LEFT])
            nave.moveLeft();
        if (key[KEY_RIGHT])
            nave.moveRight();
        if (key[KEY_UP])
            nave.moveUp();
        if (key[KEY_DOWN])
            nave.moveDown();

        // Disparar proyectil cuando se presiona la tecla de espacio
        if (key[KEY_SPACE])
        nave.disparar(enemigos);

        // Actualizar la nave
        nave.update();

        // Actualizamos y dibujamos los enemigos a través del controlador
        gameController.update();
        gameController.draw(buffer);

        if (enemigos.empty()) // Verifica si no hay enemigos
        {
            gameController.addNewEnemies();
        }

        // Dibujar en el buffer
        clear_to_color(buffer, makecol(0, 0, 0));

        for (int i = 0; i < 100; ++i)
        {
            int x = rand() % SCREEN_WIDTH;
            int y = rand() % SCREEN_HEIGHT;
            putpixel(buffer, x, y, makecol(255, 255, 255));
        }

        for (size_t i = 0; i < enemigos.size(); ++i)
        {
            enemigos[i].update();
            enemigos[i].draw(buffer);
        }

        // Actualizar y dibujar los proyectiles
        std::vector<Proyectil>& proyectiles = nave.getProyectiles();
        for (size_t i = 0; i < proyectiles.size(); ++i)
        {
            proyectiles[i].update();
            proyectiles[i].draw(buffer);
        }

        //Colisiones con pistas flotantes
        for (size_t i = 0; i < pistas.size(); ++i)
        {
        const Pista& pista = pistas[i];
        if (!nave.getInmunidad() && colisionRectangular(nave, pista))
        {
            // Disminuir una vida y entrar en período de inmunidad
            nave.setInmunidad(true);
            nave.disminuirVidas();

            nave.setTiempoInmunidad(60); // Por ejemplo, 60 actualizaciones (aproximadamente 1 segundo)

            // Iniciar tiempo de parpadeo
            nave.setTiempoParpadeo(30); // Por ejemplo, 30 actualizaciones (aproximadamente medio segundo)

            break;
        }
        }

        // Verificar si las vidas llegaron a cero
        if (nave.getVidas() <= 0 && !gameOver)
        {
            // Mostrar el cuadro de diálogo personalizado con opciones
            gameOver = true; // Marcar el juego como terminado para que el cuadro de diálogo se muestre solo una vez
            GameOverOption selectedOption = showGameOverDialog();

            if (selectedOption == GameOverOption::PlayAgain)
            {
                // El jugador seleccionó "Volver a jugar"
                // Aquí puedes agregar el código para reiniciar el juego, restaurar las vidas, etc.
                nave.setVidas(3);
                gameOver = false; // Reiniciar el estado del juego
            }
            else
            {
                // El jugador seleccionó "Salir del juego"
                exit_game = true;
                return 0;
            }
        }

        // Dibujar la nave en el buffer
        nave.draw(buffer);

        // Dibujar el contador de vidas en la esquina superior izquierda
        textprintf_ex(buffer, font, 10, 10, makecol(255, 255, 255), -1, "Vidas: %d", nave.getVidas());

        // Dibujar el contador de puntuacion
        textprintf_ex(buffer, font, SCREEN_WIDTH - 100, 10, makecol(255, 255, 255), -1, "Score: %d", gameController.getScore());

        for (size_t i = 0; i < pistas.size(); ++i)
        {
            //pistas[i].setX(rand() % SCREEN_WIDTH);
            //pistas[i].setY(rand() % SCREEN_HEIGHT);
            //std::cout << "Actualizando pista " << i << std::endl;
            pistas[i].update();
            //std::cout << "Dibujando pista " << i << std::endl;
            pistas[i].draw(buffer);
        }

        blit(buffer, screen, 0, 0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

        rest(1000 / FPS);
    }
    destroy_sample(musica);
    destroy_bitmap(buffer);
    allegro_exit();
    return 0;
}

END_OF_MAIN ()
