#ifndef ENNEMIFACTORY_H
#define ENNEMIFACTORY_H
#include "gamecanvas.h"
#include "gamecore.h"
#include "resources.h"
#include "utilities.h"
#include "sprite.h"
#include "gamescene.h"

class GameScene;
class Player;
class Ennemy;
class GameCore;

class EnnemiFactory
{
public:
    EnnemiFactory(GameScene* scene, Player* player);
    void randomlyPositionEnemyWithMargin(Ennemy* ennemi, qreal playerPosX, qreal playerPosY, qreal sceneWidth, qreal sceneHeight);
    void createWave(int nbreEnnemiLeever, int nbreEnnemiLeeverRouge, int nbreEnnemiOctopus);

    GameScene* m_pScene = nullptr;
    Player* m_pPlayer = nullptr;
};

#endif // ENNEMIFACTORY_H
