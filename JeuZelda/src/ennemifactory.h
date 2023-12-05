#ifndef ENNEMIFACTORY_H
#define ENNEMIFACTORY_H

class GameScene;

class EnnemiFactory
{
public:
    EnnemiFactory(GameScene* scene);
    void createVague(int nbreEnnemiLeever, int nbreEnnemiLeeverRouge);

    GameScene* m_pScene = nullptr;
};

#endif // ENNEMIFACTORY_H
