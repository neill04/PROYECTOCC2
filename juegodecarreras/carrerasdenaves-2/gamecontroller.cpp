#include <cstdlib>
#include <algorithm>
#include "gamecontroller.h"
#include "enemy.h"
#include "constantes.h"
#include "scorecounter.h"


GameController::GameController(std::vector<Enemy>& enemies)
    : enemigos(enemies), scoreCounter(10), nivelActual(1), maxNavesEnemigas(6), navesDestruidas(0)
{
}

void GameController::notifyEnemyDestroyed()
{
    navesDestruidas++;

    if (navesDestruidas >= maxNavesEnemigas)
    {
        nivelActual++;
        navesDestruidas = 0;

        maxNavesEnemigas++;

        addNewEnemies();

    }
}

void GameController::destroyEnemy(Enemy& enemy)
{
    // Eliminar la nave enemiga de la lista
    auto it = std::remove(enemigos.begin(), enemigos.end(), enemy);
    enemigos.erase(it, enemigos.end());

    // Incrementar el contador de naves destruidas y actualizar la puntuación
    navesDestruidas++;

    scoreCounter.updateScore();  // Incrementar el puntaje por cada nave destruida

}

int GameController::getScore() const
{
    return scoreCounter.getScore();
}

void GameController::update()
{
    for (Enemy& enemy : enemigos)
    {
        enemy.update();
    }
}

void GameController::draw(BITMAP* buffer)
{
    for (const Enemy& enemy : enemigos)
    {
        enemy.draw(buffer);
    }
}

void GameController::addNewEnemies()
{
    enemigos.clear();
    // Generar nuevas naves enemigas en el borde izquierdo o derecho de la pantalla
    for (int i = 0; i < maxNavesEnemigas; ++i)
    {
        int spawnY = rand() % (SCREEN_HEIGHT - 32); // Ajusta el rango vertical
        int spawnX = rand() % (SCREEN_WIDTH - 32);  // Ajusta el rango horizontal

        // Creamos la nueva nave enemiga y la agregamos al vector
        enemigos.push_back(Enemy(spawnX, spawnY, this)); // Opcionalmente ajusta la velocidad en Y si lo necesitas
    }
}

