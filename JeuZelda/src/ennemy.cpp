#include "ennemy.h"
#include "resources.h"
#include "utilities.h"
#include "gamecore.h"
#include "gamescene.h"
#include <QRandomGenerator>

Ennemy::Ennemy(QString imagePath) : Sprite(imagePath)
{
    setData(GameCore::SpriteDataKey::SPRITE_TYPE_KEY, GameCore::ENNEMI);
}

void Ennemy::CreateCloudOndeath(QPointF pos) {
    Sprite* pCloud = new Sprite();
    // Ajoute les images du nuage.
    for(int index = 1; index <= 7; index++) {
        pCloud->addAnimationFrame(GameFramework::imagesPath() +
                                  QString("JeuZelda/Cloud%1.png").arg(index));
    }
    qDebug() << "Cloud created";
    pCloud->setAnimationSpeed(25);
    pCloud->setScale(CLOUD_SCALE_FACTOR);
    pCloud->setPos(pos);
    pCloud->setEmitSignalEndOfAnimationEnabled(true);
    connect(pCloud, &Sprite::animationFinished, pCloud, &Sprite::deleteLater);

    // Ajoute le nuage à la scène et démarre son animation.
    parentScene()->addSpriteToScene(pCloud);
    qDebug() << "Cloud added to scene";
    pCloud->startAnimation();
}


void Ennemy::ChanceToSpawnItems(QPointF pos, int chanceToSpawnHearth, int chanceToSpawnBlueRing, int chanceToSpawnTriForce) {
    // quand l'ennemi meurt, il y a une chance sur chanceToSpawn qu'il drop un coeur
    int randomChanceToSpawnHeart = QRandomGenerator::global()->bounded(0, chanceToSpawnHearth);
    int randomChanceToSpawnBlueRing = QRandomGenerator::global()->bounded(0, chanceToSpawnBlueRing);
    int randomChanceToSpawnTriForce = QRandomGenerator::global()->bounded(0, chanceToSpawnTriForce);
    if (randomChanceToSpawnHeart == 0) {
        Sprite* pHeart = new Sprite(GameFramework::imagesPath() + "JeuZelda/HearthOnGround1.gif");
        pHeart->addAnimationFrame(GameFramework::imagesPath() + "JeuZelda/HearthOnGround1.gif");
        pHeart->addAnimationFrame(GameFramework::imagesPath() + "JeuZelda/HearthOnGround2.gif");
        pHeart->setData(GameCore::SPRITE_TYPE_KEY, GameCore::HEARTDROP);
        pHeart->setScale(GameCore::ITEM_DROP_SCALE_FACTOR);
        // positionne le coeur au centre de l'ennemi
        pHeart->setPos(pos.x() + (width() / 2), pos.y() + (height() / 2));
        parentScene()->addSpriteToScene(pHeart);
        pHeart->startAnimation(200);
        qDebug() << "Coeur ajouté à la scène";

        // si le coeur reste 3 secondes sur la scene, il clignote plus vite pour avertir le joueur de sa disparition prochaine
        QTimer::singleShot(3000, pHeart, [pHeart]() {
            pHeart->setAnimationSpeed(50);
        });

        // si le coeur reste 6 secondes sur la scene, on le supprime
        QTimer::singleShot(6000, pHeart, [pHeart]() {
            pHeart->deleteLater();
        });
    }

    if(randomChanceToSpawnBlueRing == 0) {
        Sprite* pBlueRing = new Sprite(GameFramework::imagesPath() + "JeuZelda/BlueRing.png");
        pBlueRing->addAnimationFrame(GameFramework::imagesPath() + "JeuZelda/BlueRing.png");
        pBlueRing->addAnimationFrame(GameFramework::imagesPath() + "jeuZelda/RedRing.png");
        pBlueRing->setData(GameCore::SPRITE_TYPE_KEY, GameCore::BLUE_RING);
        pBlueRing->setScale(GameCore::ITEM_DROP_SCALE_FACTOR);

        // positionne le blue ring au centre de l'ennemi
        pBlueRing->setPos(pos.x() + (width() / 2), pos.y() + (height() / 2));
        parentScene()->addSpriteToScene(pBlueRing);
        qDebug() << "Blue Ring ajouté à la scène";
        pBlueRing->startAnimation(200);

        // Si le blue ring reste 3 secondes sur la scene, il clignote plus vite pour avertir le joueur de sa disparition prochaine
        QTimer::singleShot(3000, pBlueRing, [pBlueRing]() {
            pBlueRing->setAnimationSpeed(50);
        });

        // si le blue ring reste 6 secondes sur la scene, on la supprime
        QTimer::singleShot(6000, pBlueRing, [pBlueRing]() {
            pBlueRing->deleteLater();
        });
    }

    if(randomChanceToSpawnTriForce == 0) {
        Sprite* pTriForce = new Sprite(GameFramework::imagesPath() + "JeuZelda/Triforce1.gif");
        pTriForce->addAnimationFrame(GameFramework::imagesPath() + "JeuZelda/Triforce1.gif");
        pTriForce->addAnimationFrame(GameFramework::imagesPath() + "JeuZelda/Triforce2.gif");
        pTriForce->setData(GameCore::SPRITE_TYPE_KEY, GameCore::TRIFORCE);
        pTriForce->setScale(GameCore::ITEM_DROP_SCALE_FACTOR);

        // positionne le blue ring au centre de l'ennemi
        pTriForce->setPos(pos.x() + (width() / 2), pos.y() + (height() / 2));
        parentScene()->addSpriteToScene(pTriForce);
        qDebug() << "TriForce ajouté à la scène";
        pTriForce->startAnimation(200);

        // si la triforce reste 3 secondes sur la scene, elle clignote plus vite pour avertir le joueur de sa disparition prochaine
        QTimer::singleShot(3000, pTriForce, [pTriForce]() {
            pTriForce->setAnimationSpeed(50);
        });

        // si la triforce reste 6 secondes sur la scene, on la supprime
        QTimer::singleShot(6000, pTriForce, [pTriForce]() {
            pTriForce->deleteLater();
        });
    }
}

void Ennemy::removeEnnemyFromScene() {
    parentScene()->removeSpriteFromScene(this);
    delete(this);
}

