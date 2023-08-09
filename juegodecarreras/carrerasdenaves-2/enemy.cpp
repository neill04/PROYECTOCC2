#include "enemy.h"
#include "constantes.h"
#include <cmath>

Enemy::Enemy(int x, int y, GameController* controller)
    : x(x), y(y), gameController(controller), width(32), height(32), radio(16)
{
    velocidadX = (rand() % 3) - 1;
    velocidadY = (rand() % 3) - 1;
}

void Enemy::update()
{
    // Actualizamos la posición del enemigo
    x += velocidadX;
    y += velocidadY;

    // Lógica para mantener a los enemigos dentro de la pantalla (como antes)
    if (x <= 0 || x + width >= SCREEN_WIDTH)
    {
        velocidadX = -velocidadX;
    }
    if (y <= 0 || y + height >= SCREEN_HEIGHT)
    {
        velocidadY = -velocidadY;
    }

}

void Enemy::draw(BITMAP* buffer) const
{
    rectfill(buffer, x, y, x + width, y + height, makecol(255, 0, 0)); // En este ejemplo, los enemigos serán rectángulos rojos
}

int Enemy::getX() const
{
    return x;
}

int Enemy::getY() const
{
    return y;
}

int Enemy::getWidth() const
{
    return width;
}

int Enemy::getHeight() const
{
    return height;
}

int Enemy::getRadius() const
{
    // El radio será la mitad de la diagonal del rectángulo (enemigo)
    int diagonal = std::sqrt(width * width + height * height);
    int radio = diagonal / 2;
    return radio;
}


