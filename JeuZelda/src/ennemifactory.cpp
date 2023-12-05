#include "ennemifactory.h"
#include "player.h"
#include "gamecanvas.h"
#include "gamecore.h"
#include "resources.h"
#include "utilities.h"
#include "sprite.h"
#include "gamescene.h"
#include "ennemileever.h"
#include "ennemileeverrouge.h"

EnnemiFactory::EnnemiFactory(GameScene* scene)
{
    m_pScene = scene;
}

void EnnemiFactory::createVague(int nbreEnnemiLeever, int nbreEnnemiLeeverRouge) {
    //Génère le nombre d'ennemi demandé
    for(int i = 0; i < nbreEnnemiLeever; i++) {
        //Création d'un ennemi
        EnnemiLeever* ennemi = new EnnemiLeever();
        //Ajout de l'ennemi à la scène
        m_pScene->addSpriteToScene(ennemi);
        //Positionnement de l'ennemi aléatoirement
        ennemi->setX(std::rand() % (m_pScene->width() - ennemi->width()));
        ennemi->setY(std::rand() % (m_pScene->height() - ennemi->height()));
    }

    for(int i = 0; i < nbreEnnemiLeeverRouge; i++) {
        //Création d'un ennemi
        EnnemiLeeverRouge* ennemi = new EnnemiLeeverRouge();
        //Ajout de l'ennemi à la scène
        m_pScene->addSpriteToScene(ennemi);
        //Positionnement de l'ennemi aléatoirement
        ennemi->setX(std::rand() % (m_pScene->width() - ennemi->width()));
        ennemi->setY(std::rand() % (m_pScene->height() - ennemi->height()));
    }
}

