#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <vector>
#include <allegro.h>
#include "scorecounter.h"

class Enemy;
class GameController
{
public:
    GameController(std::vector<Enemy>& enemies);

    void update();
    void draw(BITMAP* buffer);
    void notifyEnemyDestroyed();
    void addNewEnemies();
    void destroyEnemy(Enemy& enemy);
    int getScore() const;

private:
    std::vector<Enemy>& enemigos;
    int nivelActual;
    int maxNavesEnemigas;
    int navesDestruidas;
    ScoreCounter scoreCounter;
};

#endif // GAMECONTROLLER_H

