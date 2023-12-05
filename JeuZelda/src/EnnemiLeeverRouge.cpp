#include "EnnemiLeeverRouge.h"
#include "resources.h"
#include "utilities.h"
#include "gamecore.h"
#include "gamescene.h"
#include <cstdlib>

EnnemiLeeverRouge::EnnemiLeeverRouge(): Ennemy(GameFramework::imagesPath() + "JeuZelda/Ennemi2_1.gif")
{
    // Animation de l'ennemi
    addAnimationFrame(GameFramework::imagesPath() + "JeuZelda/Ennemi2_1.gif");
    addAnimationFrame(GameFramework::imagesPath() + "JeuZelda/ennemi2_2.gif");
    startAnimation(100);
    setScale(LEEVER_ROUGE_SCALE_FACTOR);
    m_Hp = 2;
}

EnnemiLeeverRouge::~EnnemiLeeverRouge() {

}

void EnnemiLeeverRouge::tick(long long elapsedTimeInMilliseconds) {
    // Définis une constante pour la fréquence de mouvement souhaitée (une chance sur 3 chaque seconde)
    const int movementFrequency = 3000;  // en millisecondes (1000 ms = 1 seconde)

    // Utilise un compteur pour suivre le temps écoulé
    static long long timeCounter = 0;

    // Ajoute le temps écoulé depuis la dernière mise à jour
    timeCounter += elapsedTimeInMilliseconds;

    // Vérifie si le temps écoulé dépasse la fréquence de mouvement
    if (timeCounter >= movementFrequency) {
        // Réinitialise le compteur
        timeCounter = 0;

        // Génére un nombre aléatoire entre 0 et 2 inclus
        int randomDirection = std::rand() % 4;

        // En fonction du nombre aléatoire, déplace l'ennemi dans une direction spécifique
        switch (randomDirection) {
        case 0:
            // Déplace vers le haut
            if (y() - LEEVER_ROUGE_RANGE >= 0) {
                setY(y() - LEEVER_ROUGE_RANGE);
            }
            break;
        case 1:
            // Déplace vers le bas
            if (y() + LEEVER_ROUGE_RANGE <= parentScene()->height() - height()) {
                setY(y() + LEEVER_ROUGE_RANGE);
            }
            break;
        case 2:
            // Déplace vers la gauche
            if (x() - LEEVER_ROUGE_RANGE >= 0) {
                setX(x() - LEEVER_ROUGE_RANGE);
            }
            break;
        case 3:
            // Déplace vers la droite
            if (x() + LEEVER_ROUGE_RANGE <= parentScene()->width() - width()) {
                setX(x() + LEEVER_ROUGE_RANGE);
            }
            break;
        }
    }
}

void EnnemiLeeverRouge::damage() {
    m_Hp--;
    if(m_Hp <= 0) {
        // L'ennemi est mort, on le supprime.
        parentScene()->removeSpriteFromScene(this);
        CreateCloudOndeath(pos());
        ChanceToSpawnHearth(pos());
        deleteLater();
    }
}

void EnnemiLeeverRouge::CreateCloudOndeath(QPointF pos) {
    Sprite* pCloud = new Sprite();
    // Ajoute les images du nuage.
    for(int index = 1; index <= 7; index++) {
        pCloud->addAnimationFrame(GameFramework::imagesPath() +
                                  QString("JeuZelda/Cloud%1.png").arg(index));
    }
    qDebug() << "Cloud created";
    pCloud->setAnimationSpeed(25);
    pCloud->setScale(5);
    pCloud->setPos(pos);
    pCloud->setEmitSignalEndOfAnimationEnabled(true);
    connect(pCloud, &Sprite::animationFinished, pCloud, &Sprite::deleteLater);

    // Ajoute le nuage à la scène et démarre son animation.
    parentScene()->addSpriteToScene(pCloud);
    qDebug() << "Cloud added to scene";
    pCloud->startAnimation();
}

void EnnemiLeeverRouge::ChanceToSpawnHearth(QPointF pos) {
    // quand l'ennemi meurt, il y a une chance sur X qu'il drop un coeur
    int randomChance = std::rand() % 2;
    if (randomChance == 0) {
        Sprite* pHeart = new Sprite(GameFramework::imagesPath() + "JeuZelda/HearthOnGround1.gif");
        pHeart->addAnimationFrame(GameFramework::imagesPath() + "JeuZelda/HearthOnGround1.gif");
        pHeart->addAnimationFrame(GameFramework::imagesPath() + "JeuZelda/HearthOnGround2.gif");
        pHeart->setData(GameCore::SPRITE_TYPE_KEY, GameCore::HEARTDROP);
        pHeart->setScale(4);
        pHeart->setPos(pos);
        parentScene()->addSpriteToScene(pHeart);
        pHeart->startAnimation(100);
        qDebug() << "Heart added to scene";
    }
}

