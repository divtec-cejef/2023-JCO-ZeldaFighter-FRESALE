#ifndef ENNEMIFACTORY_H
#define ENNEMIFACTORY_H

class GameScene;

class EnnemiFactory
{
public:
    EnnemiFactory(GameScene* scene);
    void createWave(int nbreEnnemiLeever, int nbreEnnemiLeeverRouge, int nbreEnnemiOctopus);

    GameScene* m_pScene = nullptr;
};

#endif // ENNEMIFACTORY_H
