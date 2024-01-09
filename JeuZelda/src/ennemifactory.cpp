#include "ennemifactory.h"
#include "player.h"
#include "gamecanvas.h"
#include "gamecore.h"
#include "resources.h"
#include "utilities.h"
#include "sprite.h"
#include "gamescene.h"
#include "ennemy.h"
#include "ennemileever.h"
#include "ennemileeverrouge.h"
#include "ennemioctopus.h"

EnnemiFactory::EnnemiFactory(GameScene* scene, Player* player)
{
    m_pScene = scene;
    m_pPlayer = player;
}

//! \param ennemi L'ennemi à positionner
//! \param playerPosX La position X du joueur
//! \param playerPosY La position Y du joueur
//! \param sceneWidth La largeur de la scène
//! \param sceneHeight La hauteur de la scène
//! Positionne un ennemi de manière aléatoire avec une marge de 100px par rapport au joueur
void EnnemiFactory::randomlyPositionEnemyWithMargin(Ennemy* ennemi, qreal playerPosX, qreal playerPosY, qreal sceneWidth, qreal sceneHeight) {
    ennemi->setX(QRandomGenerator::global()->bounded(sceneWidth - ennemi->width()));
    ennemi->setY(QRandomGenerator::global()->bounded(sceneHeight - ennemi->height() - 100) + 50);

    while (qAbs(ennemi->x() - playerPosX) < 100 || qAbs(ennemi->y() - playerPosY) < 100) {
        // Réessayer si la distance minimale entre le joueur et l'ennemi n'est pas respectée
        ennemi->setX(QRandomGenerator::global()->bounded(sceneWidth - ennemi->width()));
        ennemi->setY(QRandomGenerator::global()->bounded(sceneHeight - ennemi->height() - 100) + 50);
    }
}

//! Crée une vague d'ennemis
//! \param nbreEnnemiLeever Le nombre d'ennemis Leever à générer
//! \param nbreEnnemiLeeverRouge Le nombre d'ennemis Leever Rouge à générer
//! \param nbreEnnemiOctopus Le nombre d'ennemis Octopus à générer
void EnnemiFactory::createWave(int nbreEnnemiLeever, int nbreEnnemiLeeverRouge, int nbreEnnemiOctopus) {
    qreal playerPosX = m_pPlayer->x();
    qreal playerPosY = m_pPlayer->y();
    qreal sceneWidth = m_pScene->width();
    qreal sceneHeight = m_pScene->height();

    // Génère le nombre d'ennemis Leever demandés
    for (int i = 0; i < nbreEnnemiLeever; i++) {
        EnnemiLeever* ennemi = new EnnemiLeever();
        m_pScene->addSpriteToScene(ennemi);
        randomlyPositionEnemyWithMargin(ennemi, playerPosX, playerPosY, sceneWidth, sceneHeight);
    }

    // Génère le nombre d'ennemis LeeverRouge demandés
    for (int i = 0; i < nbreEnnemiLeeverRouge; i++) {
        EnnemiLeeverRouge* ennemi = new EnnemiLeeverRouge();
        m_pScene->addSpriteToScene(ennemi);
        randomlyPositionEnemyWithMargin(ennemi, playerPosX, playerPosY, sceneWidth, sceneHeight);
    }

    // Génère le nombre d'ennemis Octopus demandés
    for (int i = 0; i < nbreEnnemiOctopus; i++) {
        EnnemiOctopus* ennemi = new EnnemiOctopus();
        m_pScene->addSpriteToScene(ennemi);
        randomlyPositionEnemyWithMargin(ennemi, playerPosX, playerPosY, sceneWidth, sceneHeight);
    }
}


