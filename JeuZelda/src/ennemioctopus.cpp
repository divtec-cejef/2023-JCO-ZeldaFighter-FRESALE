#include "ennemioctopus.h"
#include "resources.h"
#include "utilities.h"
#include "gamecore.h"
#include "gamescene.h"
#include <cstdlib>
#include <QRandomGenerator>

EnnemiOctopus::EnnemiOctopus(): Ennemy(GameFramework::imagesPath() + "JeuZelda/EnnemiOctopus_1.gif")
{
    // Animation de l'ennemi
    addAnimationFrame(GameFramework::imagesPath() + "JeuZelda/EnnemiOctopus_1.gif");
    addAnimationFrame(GameFramework::imagesPath() + "JeuZelda/EnnemiOctopus_2.gif");
    startAnimation(100);
    // offset au millieu du sprite
    setOffset(sceneBoundingRect().width() / -2.0, sceneBoundingRect().height() / -2.0);
    setScale(OCTOPUS_SCALE_FACTOR);
    m_Hp = 1;
}

void EnnemiOctopus::tick(long long elapsedTimeInMilliseconds) {
    // L'ennemi attaque le joueur avec un projectile
    if(m_pProjectil != nullptr) {
        m_pProjectil->tick(elapsedTimeInMilliseconds);
    } else {
        // Génére un nombre aléatoire entre 0 et 2 inclus
        int randomDirection = QRandomGenerator::global()->bounded(0, 4);
        switch (randomDirection) {
        case 0:
            // Attaque vers le haut
            attack(QPointF(0, -1));
            setRotation(180);
            break;
        case 1:
            // Attaque vers le bas
            attack(QPointF(0, 1));
            setRotation(0);
            break;
        case 2:
            // Attaque vers la gauche
            attack(QPointF(-1, 0));
            setRotation(90);
            break;
        case 3:
            // Attaque vers la droite
            attack(QPointF(1, 0));
            setRotation(270);
            break;
        }
    }
}


void EnnemiOctopus::attack(QPointF direction) {
    if(m_pProjectil = nullptr)
        return;
    m_pProjectil = new Projectile(300, direction, GameFramework::imagesPath() + "JeuZelda/RockProjectil.png", this);
    m_pProjectil->setScale(2);
    m_pProjectil->setPos(pos());
    // m_pSword->setOffset(sceneBoundingRect().width() / -2.0, sceneBoundingRect().height() / -2.0);
    parentScene()->addSpriteToScene(m_pProjectil);
}

void EnnemiOctopus::damage() {
    m_Hp--;
    if(m_Hp <= 0) {
        CreateCloudOndeath(pos());
        ChanceToSpawnItems(pos(), 8, 16, 50);
        removeEnnemyFromScene();
        removeProjectile();
    }
}

void EnnemiOctopus::removeProjectile() {
    parentScene()->removeSpriteFromScene(m_pProjectil);
    delete m_pProjectil;
    m_pProjectil = nullptr;
}
