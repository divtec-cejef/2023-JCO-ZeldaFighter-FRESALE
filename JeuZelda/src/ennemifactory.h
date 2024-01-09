#ifndef ENNEMIFACTORY_H
#define ENNEMIFACTORY_H

class GameScene;
class Player;
class GameCore;

class EnnemiFactory
{
public:
    EnnemiFactory(GameScene* scene, Player* player);
    void createWave(int nbreEnnemiLeever, int nbreEnnemiLeeverRouge, int nbreEnnemiOctopus);

    GameScene* m_pScene = nullptr;
    Player* m_pPlayer = nullptr;
};

#endif // ENNEMIFACTORY_H
