#ifndef ENEMY_H
#define ENEMY_H

#include <allegro.h>
#include <memory>
#include <vector>
#include "proyectil.h"

class GameController;
class Enemy
{
public:
    Enemy(int x, int y, GameController* controller);

    void update();
    void draw(BITMAP* buffer) const;
    int getX() const;
    int getY() const;
    int getWidth() const;
    int getHeight() const;
    int getRadius() const;
    bool operator==(const Enemy& other) const {
    // Implementa la comparación aquí basada en los atributos de Enemy
    return x == other.x && y == other.y; // Por ejemplo, compara las posiciones
    }

    void setVelocidadX(double velocidadX) { this->velocidadX = velocidadX; }
    void setVelocidadY(double velocidadY) { this->velocidadY = velocidadY; }

private:
    int radio;
    int x, y;
    int width, height;
    double velocidadX, velocidadY;
    GameController* gameController;
};

#endif // ENEMY_H

