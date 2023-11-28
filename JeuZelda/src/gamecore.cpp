/**
  Fichier qui contient toute la logique du jeu.
  
  @author   JCO
  @date     Février 2014
 */
#include "gamecore.h"

#include <cmath>

#include <QDebug>
#include <QSettings>
#include <QMovie>

#include "gamescene.h"
#include "gamecanvas.h"
#include "resources.h"
#include "utilities.h"
#include "sprite.h"
#include "player.h"
#include "EnnemiLeever.h"

//! Initialise le contrôleur de jeu.
//! \param pGameCanvas  GameCanvas pour lequel cet objet travaille.
//! \param pParent      Pointeur sur le parent (afin d'obtenir une destruction automatique de cet objet).
GameCore::GameCore(GameCanvas* pGameCanvas, QObject* pParent) : QObject(pParent) {
    
    m_playerSpeed = 10;

    // Mémorise l'accès au canvas (qui gère le tick et l'affichage d'une scène)
    m_pGameCanvas = pGameCanvas;
    
    // Créé la scène de base et indique au canvas qu'il faut l'afficher.
    m_pScene = pGameCanvas->createScene(0, 0, SCENE_WIDTH, SCENE_WIDTH / GameFramework::screenRatio());
    pGameCanvas->setCurrentScene(m_pScene);
    
    // Trace un rectangle blanc tout autour des limites de la scène.
    m_pScene->addRect(m_pScene->sceneRect(), QPen(Qt::white));
    
    // Instancier et initialiser les sprite ici :
    m_pPlayer = new Player();
    m_pScene->addSpriteToScene(m_pPlayer);
    m_pPlayer->initializeHearts();


    // Nouvelle ennemi
    m_pEnnemiLeever = new EnnemiLeever();
    m_pScene->addSpriteToScene(m_pEnnemiLeever);
    m_pEnnemiLeever->setData(SPRITE_TYPE_KEY, ENNEMI);
    m_pEnnemiLeever->setPos(500, 500);

    // Création des décors
    Sprite* pBush1 = new Sprite(GameFramework::imagesPath() + "JeuZelda/Bush.png");
    m_pScene->addSpriteToScene(pBush1);
    pBush1->setPos(100, 200);
    pBush1->setScale(DECOR_SCALE_FACTOR);
    pBush1->setData(SPRITE_TYPE_KEY, SpriteType::DECOR);
    m_pBush1 = pBush1;


    Sprite* pBush2 = new Sprite(GameFramework::imagesPath() + "JeuZelda/Bush.png");
    m_pScene->addSpriteToScene(pBush2);
    pBush2->setPos(300, 550);
    pBush2->setScale(DECOR_SCALE_FACTOR);
    pBush2->setData(SPRITE_TYPE_KEY, SpriteType::DECOR);
    m_pBush2 = pBush2;

    Sprite* pRock1 = new Sprite(GameFramework::imagesPath() + "JeuZelda/Rock.png");
    m_pScene->addSpriteToScene(pRock1);
    pRock1->setPos(1000, 200);
    pRock1->setScale(DECOR_SCALE_FACTOR);
    pRock1->setData(SPRITE_TYPE_KEY, SpriteType::DECOR);
    m_pBush1 = pRock1;

    // Création du point d'eau
    //QPixmap brickImage = QPixmap(GameFramework::imagesPath() + "JeuZelda/Water.png");
    //int brickWidth = brickImage.width() + 1;
    //int brickHeight = brickImage.height() + 1;

    //for (int row = 0; row < 5; row++) {
    //    for (int col = 0; col < 1; col++) {
    //        Sprite* pBrick1 = new Sprite(brickImage);
    //        pBrick1->setData(SPRITE_TYPE_KEY, WATER);
    //        pBrick1->setScale(DECOR_SQUALE_FACTOR);
    //        m_pScene->addSpriteToScene(pBrick1, 1160 + col * brickWidth, 0 + row * brickHeight);
    //    }
    //}

    // Center le joueur au millieu de la scène.
    m_pPlayer->setOffset(-m_pPlayer->sceneBoundingRect().width()/2, -m_pPlayer->sceneBoundingRect().width()/2);
    m_pPlayer->setScale(PLAYER_SCALE_FACTOR);
    m_pPlayer->setPos(m_pScene->width()/2.0, m_pScene->height()/2.0);

    // Fond d'écran de la scène.
    m_pScene->setBackgroundColor(QColor(252, 216, 168));

    // Démarre le tick pour que les animations qui en dépendent fonctionnent correctement.
    // Attention : il est important que l'enclenchement du tick soit fait vers la fin de cette fonction,
    // sinon le temps passé jusqu'au premier tick (ElapsedTime) peut être élevé et provoquer de gros
    // déplacements, surtout si le déboggueur est démarré.
    m_pGameCanvas->startTick();

}

//! Destructeur de GameCore : efface les scènes
GameCore::~GameCore() {
    delete m_pScene;
    m_pScene = nullptr;
}

void GameCore::keyPressed(int key) {
    // Mettre à jour l'état de la touche correspondante
    switch (key) {
    case Qt::Key_Left:
        isLeftKeyPressed = true;
        m_pPlayer->addAnimationFrame(GameFramework::imagesPath() + "JeuZelda/LeftLink_2.gif");
        m_pPlayer->addAnimationFrame(GameFramework::imagesPath() + "JeuZelda/LeftLink_1.gif");
        m_pPlayer->startAnimation(100);
        break;
    case Qt::Key_Right:
        isRightKeyPressed = true;
        m_pPlayer->addAnimationFrame(GameFramework::imagesPath() + "JeuZelda/RightLink_2.gif");
        m_pPlayer->addAnimationFrame(GameFramework::imagesPath() + "JeuZelda/RightLink_1.gif");
        m_pPlayer->startAnimation(100);
        break;
    case Qt::Key_Up:
        isUpKeyPressed = true;
        m_pPlayer->addAnimationFrame(GameFramework::imagesPath() + "JeuZelda/UpLink_2.gif");
        m_pPlayer->addAnimationFrame(GameFramework::imagesPath() + "JeuZelda/UpLink_1.gif");
        m_pPlayer->startAnimation(100);
        break;
    case Qt::Key_Down:
        isDownKeyPressed = true;
        m_pPlayer->addAnimationFrame(GameFramework::imagesPath() + "JeuZelda/DownLink_2.gif");
        m_pPlayer->addAnimationFrame(GameFramework::imagesPath() + "JeuZelda/DownLink_1.gif");
        m_pPlayer->startAnimation(100);
        break;
    case Qt::Key_W:
        isWKeyPressed = true;
        if(!hasWAnimationPlayed) {
           /*
            m_pPlayer->addAnimationFrame(GameFramework::imagesPath() + "JeuZelda/UpLink_1.gif");
            m_pPlayer->addAnimationFrame(GameFramework::imagesPath() + "JeuZelda/UpLinkSword.png");
            m_pPlayer->startAnimation(100);*/
            m_playerSpeed = 0;
            hasWAnimationPlayed = true;
        }
        break;

    case Qt::Key_A:
        isAKeyPressed = true;
        if(!hasAAnimationPlayed) {/*
            m_pPlayer->addAnimationFrame(GameFramework::imagesPath() + "JeuZelda/LeftLink_1.gif");
            m_pPlayer->addAnimationFrame(GameFramework::imagesPath() + "JeuZelda/LeftLinkSword.png");
            m_pPlayer->startAnimation(100);*/
            m_playerSpeed = 0;
            hasAAnimationPlayed = true;
        }
        break;

    case Qt::Key_S:
        isSKeyPressed = true;
        if(!hasSAnimationPlayed) {/*
            m_pPlayer->addAnimationFrame(GameFramework::imagesPath() + "JeuZelda/DownLink_1.gif");
            m_pPlayer->addAnimationFrame(GameFramework::imagesPath() + "JeuZelda/DownLinkSword.png");
            m_pPlayer->startAnimation(100);*/
            m_playerSpeed = 0;
            hasSAnimationPlayed = true;
        }
        break;

    case Qt::Key_D:
        isDKeyPressed = true;
        if(!hasDAnimationPlayed) {/*
            m_pPlayer->addAnimationFrame(GameFramework::imagesPath() + "JeuZelda/RightLink_1.gif");
            m_pPlayer->addAnimationFrame(GameFramework::imagesPath() + "JeuZelda/RightLinkSword.png");
            m_pPlayer->startAnimation(100);*/
            m_playerSpeed = 0;
            hasDAnimationPlayed = true;
        }
        break;
    default:
        //
        break;
    }
}


//! Traite le relâchement d'une touche.
//! \param key Numéro de la touche (voir les constantes Qt)
void GameCore::keyReleased(int key) {
    // Réinitialiser l'état de la touche correspondante
    switch (key) {
    case Qt::Key_Left:
        isLeftKeyPressed = false;
        m_pPlayer->stopAnimation();
        m_pPlayer->clearAnimations();
        break;
    case Qt::Key_Right:
        isRightKeyPressed = false;
        m_pPlayer->stopAnimation();
        m_pPlayer->clearAnimations();
        break;
    case Qt::Key_Up:
        isUpKeyPressed = false;
        m_pPlayer->stopAnimation();
        m_pPlayer->clearAnimations();
        break;
    case Qt::Key_Down:
        isDownKeyPressed = false;
        m_pPlayer->stopAnimation();
        m_pPlayer->clearAnimations();
        break;
    case Qt::Key_W:
        isWKeyPressed = false;
        m_pPlayer->stopAnimation();
        m_pPlayer->clearAnimations();
        break;
    case Qt::Key_A:
        isAKeyPressed = false;
        m_pPlayer->stopAnimation();
        m_pPlayer->clearAnimations();
        hasAAnimationPlayed = false;
        break;
    case Qt::Key_S:
        isSKeyPressed = false;
        m_pPlayer->stopAnimation();
        m_pPlayer->clearAnimations();
        hasSAnimationPlayed = false;
        break;
    case Qt::Key_D:
        isDKeyPressed = false;
        m_pPlayer->stopAnimation();
        m_pPlayer->clearAnimations();
        hasDAnimationPlayed = false;
        break;
    default:
        //
        break;
    }
}


void GameCore::tick(long long elapsedTimeInMilliseconds) {
    m_pPlayer->tick(static_cast<int>(elapsedTimeInMilliseconds));
    m_pEnnemiLeever->tick(static_cast<int>(elapsedTimeInMilliseconds));
    updatePlayer();

    QList<Sprite*> collisions = m_pScene->collidingSprites(m_pPlayer);
    bool isInWater = false;
    bool isCollidingWithDecor = false;

    if (!collisions.isEmpty()) {
        Sprite* pCollisionned = collisions.at(0);
        if (pCollisionned->data(SPRITE_TYPE_KEY).toInt() == DECOR) {
            isCollidingWithDecor = true;

            // Déterminer la direction de la collision
            int playerX = m_pPlayer->x();
            int playerY = m_pPlayer->y();
            int collisionX = pCollisionned->x();
            int collisionY = pCollisionned->y();
            int collisionWidth = pCollisionned->width();
            int collisionHeight = pCollisionned->height();

            int xOverlap = std::min(playerX + m_pPlayer->width(), collisionX + collisionWidth) - std::max(playerX, collisionX);
            int yOverlap = std::min(playerY + m_pPlayer->height(), collisionY + collisionHeight) - std::max(playerY, collisionY);

            // Adjust player position based on collision direction
            if (xOverlap < yOverlap) {
                if (playerX < collisionX) {
                    m_pPlayer->setX(m_pPlayer->x() - m_playerSpeed);
                } else {
                    m_pPlayer->setX(m_pPlayer->x() + m_playerSpeed);
                }
            } else {
                if (playerY < collisionY) {
                    m_pPlayer->setY(m_pPlayer->y() - m_playerSpeed);
                } else {
                    m_pPlayer->setY(m_pPlayer->y() + m_playerSpeed);
                }
            }
        } else if (pCollisionned->data(SPRITE_TYPE_KEY).toInt() == WATER) {
            isInWater = true;
        } else if (pCollisionned->data(SPRITE_TYPE_KEY).toInt() == ENNEMI) {
            // Si la touche W est presée et qua la colision haute du joueur touche l'ennemi
            if (isWKeyPressed && m_pPlayer->y() < pCollisionned->y()) {
                // L'ennemi prend des dégâts
                displayInformation("test");
            } else {
                // Le joueur prend des dégâts
                m_pPlayer->damage();
            }
        }
    }

    if (!isCollidingWithDecor) {
        // Adjust player speed based on whether the player is in water or not
        if (isInWater) {
            m_playerSpeed = 5;
        } else {
            m_playerSpeed = 10;
        }
    }
}

void GameCore::displayInformation(const QString& rMessage) {
    // Affichage du message en gras.
    QGraphicsSimpleTextItem* pText = m_pScene->createText(QPointF(0,0), rMessage, 50, Qt::red);
    QFont boldFont = pText->font();
    boldFont.setBold(true);
    pText->setFont(boldFont);

    // Centrage du texte
    pText->setPos((m_pScene->width() - pText->boundingRect().width()) / 2, m_pScene->height() / 2);
    m_pDisplayedInformation = pText;
}

void GameCore::updatePlayer() {
    // Déplacer le personnage en fonction de l'état des touches
    if (isLeftKeyPressed) {
        m_pPlayer->setX(m_pPlayer->x() - m_playerSpeed);
    }
    if (isRightKeyPressed) {
        m_pPlayer->setX(m_pPlayer->x() + m_playerSpeed);
    }
    if (isUpKeyPressed) {
        m_pPlayer->setY(m_pPlayer->y() - m_playerSpeed);
    }
    if (isDownKeyPressed) {
        m_pPlayer->setY(m_pPlayer->y() + m_playerSpeed);
    }
    if(isWKeyPressed) {
        m_pPlayer->attack(QPointF(0, -1));
    }
    if(isAKeyPressed) {
        m_pPlayer->attack(QPointF(-1, 0));
    }
    if(isSKeyPressed) {
        m_pPlayer->attack(QPointF(0, 1));
    }
    if(isDKeyPressed) {
        m_pPlayer->attack(QPointF(1, 0));
    }
}

//! La souris a été déplacée.
//! Pour que cet événement soit pris en compte, la propriété MouseTracking de GameView
//! doit être enclenchée avec GameCanvas::startMouseTracking().
void GameCore::mouseMoved(QPointF newMousePosition) {
    emit notifyMouseMoved(newMousePosition);
}

//! Traite l'appui sur un bouton de la souris.
void GameCore::mouseButtonPressed(QPointF mousePosition, Qt::MouseButtons buttons) {
    emit notifyMouseButtonPressed(mousePosition, buttons);
}

//! Traite le relâchement d'un bouton de la souris.
void GameCore::mouseButtonReleased(QPointF mousePosition, Qt::MouseButtons buttons) {
    emit notifyMouseButtonReleased(mousePosition, buttons);
}

