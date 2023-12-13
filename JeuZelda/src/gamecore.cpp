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
#include <QFontDatabase>

#include "gamescene.h"
#include "gamecanvas.h"
#include "resources.h"
#include "utilities.h"
#include "sprite.h"
#include "player.h"
#include "EnnemiLeever.h"
#include "ennemifactory.h"
#include "ennemileeverrouge.h"
#include "ennemioctopus.h"

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

    // Création des ennemis grâce à la classe EnnemiFactory
    // EnnemiFactory* ennemifactoy = new EnnemiFactory(m_pScene);
    // ennemifactoy->createWave(5,0,0);

    // Création des décors
    Sprite* pBush1 = new Sprite(GameFramework::imagesPath() + "JeuZelda/Bush.png");
    m_pScene->addSpriteToScene(pBush1);
    pBush1->setPos(200, 200);
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
    m_pRock1 = pRock1;

    // boucle qui permet de générer le bord de l'eau en bas de la scène
    for(int i = 0; i < m_pScene->width(); i += 50) {
        Sprite* pWater = new Sprite(GameFramework::imagesPath() + "JeuZelda/WaterBorderUp.png");
        m_pScene->addSpriteToScene(pWater);
        pWater->setPos(i, m_pScene->height() - 50);
        pWater->setScale(WATER_SCALE_FACTOR);
        pWater->setData(SPRITE_TYPE_KEY, SpriteType::DECOR);
    }

    // boucle qui permet de générer le bord de l'eau en haut de la scène
    for(int i = 0; i < m_pScene->width(); i += 50) {
        Sprite* pWater = new Sprite(GameFramework::imagesPath() + "JeuZelda/WaterBorderDown.png");
        m_pScene->addSpriteToScene(pWater);
        pWater->setPos(i, 0);
        pWater->setScale(WATER_SCALE_FACTOR);
        pWater->setData(SPRITE_TYPE_KEY, SpriteType::DECOR);
    }



    // Center le joueur au millieu de la scène.
    m_pPlayer->setOffset(-m_pPlayer->sceneBoundingRect().width()/2, -m_pPlayer->sceneBoundingRect().width()/2);
    m_pPlayer->setScale(PLAYER_SCALE_FACTOR);
    m_pPlayer->setPos(m_pScene->width()/2.0, m_pScene->height()/2.0);

    // Initialise les coeurs du joueur
    m_pPlayer->initializeHearts();

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
        if (!m_pressedKeys.contains(key)) {
            m_pressedKeys.prepend(key);
        }
        break;
    case Qt::Key_Right:
        isRightKeyPressed = true;
        if(!m_pressedKeys.contains(key)) {
            m_pressedKeys.prepend(key);
        }
        break;
    case Qt::Key_Up:
        isUpKeyPressed = true;
        if(!m_pressedKeys.contains(key)) {
            m_pressedKeys.prepend(key);
        }
        break;
    case Qt::Key_Down:
        isDownKeyPressed = true;
        if(!m_pressedKeys.contains(key)) {
            m_pressedKeys.prepend(key);
        }
        break;
    case Qt::Key_W:
        isWKeyPressed = true;
        if(!hasWAnimationPlayed) {
            m_playerSpeed = 0;
            hasWAnimationPlayed = true;
        }
        break;

    case Qt::Key_A:
        isAKeyPressed = true;
        if(!hasAAnimationPlayed) {
            m_playerSpeed = 0;
            hasAAnimationPlayed = true;
        }
        break;

    case Qt::Key_S:
        isSKeyPressed = true;
        if(!hasSAnimationPlayed) {
            m_playerSpeed = 0;
            hasSAnimationPlayed = true;
        }
        break;

    case Qt::Key_D:
        isDKeyPressed = true;
        if(!hasDAnimationPlayed) {
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
    if(key == Qt::Key_Space) {
        switch (m_gameMode) {
        case RUNNING:
            if (m_pGameCanvas->isTicking()) {
                m_pGameCanvas->stopTick();
                displayInformation("Press space to continue");
            } else {
                clearInformation();
                m_pGameCanvas->startTick();
            }
            break;
        case PAUSE:
            m_gameMode = RUNNING;
            clearInformation();
            break;

        case ENDED_LOSE:
            restartGame();
            break;
        }

    }
    // Réinitialiser l'état de la touche correspondante
    switch (key) {
    case Qt::Key_Left:
        isLeftKeyPressed = false;
        m_pPlayer->clearAnimations();
        m_pressedKeys.removeAll(key);
        break;
    case Qt::Key_Right:
        isRightKeyPressed = false;
        m_pPlayer->clearAnimations();
        m_pressedKeys.removeAll(key);
        break;
    case Qt::Key_Up:
        isUpKeyPressed = false;
        m_pPlayer->clearAnimations();
        m_pressedKeys.removeAll(key);
        break;
    case Qt::Key_Down:
        isDownKeyPressed = false;
        m_pPlayer->clearAnimations();
        m_pressedKeys.removeAll(key);
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

    auto children = m_pScene->items();

    for(auto child: children) {
        if (Ennemy* ennemi = dynamic_cast<Ennemy*>(child)) {
            ennemi->tick(elapsedTimeInMilliseconds);
        }
    }

    if(m_pPlayer->m_pHearts.length() == 1) {
        m_pPlayer->blinkRed();
    }

    countEnnemies();
    generateEnemyWave();
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

            // Ajuster la position du joueur en fonction de la direction de la collision
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
            // Le joueur prend des dégâts
            m_pPlayer->damage();
            if(m_pPlayer->isDead) {
                m_gameMode = ENDED_LOSE;
                if (m_pGameCanvas->isTicking()) {
                    m_pGameCanvas->stopTick();
                }
                displayInformation("Game Over !");
                qDebug() << "Le joueur est mort";
            }
        } else if (pCollisionned->data(SPRITE_TYPE_KEY).toInt() == HEARTDROP) {
            // Ajoute un coeur au joueur si il en a moin de MAX_HEARTH
            if(m_pPlayer->m_pHearts.length() < MAX_HEARTH) {
                m_pPlayer->addHeart();
            }
            // supprime le coeur de la scène une fois que le joueur l'a touchée
            m_pScene->removeSpriteFromScene(pCollisionned);
            delete pCollisionned;
        } else if(pCollisionned->data(SPRITE_TYPE_KEY).toInt() == BLUE_RING) {
            // Le projectile (épée) du joueur va 2 fois plus vite pendant 5 secondes
            qDebug() << "Blue Ring touché";
                        m_pPlayer->swordVitesse *= 2;

            QTimer::singleShot(5000, m_pPlayer, [this]() {
                m_pPlayer->swordVitesse = 550.0;
            });
            // supprime le Blue Ring de la scène une fois que le joueur l'a touchée
            m_pScene->removeSpriteFromScene(pCollisionned);
            delete pCollisionned;
        } else if(pCollisionned->data(SPRITE_TYPE_KEY).toInt() == TRIFORCE) {
            // Tous les ennemis de la scène perde 1 hp
            auto children = m_pScene->items();
            for(auto child: children) {
                if (Ennemy* ennemi = dynamic_cast<Ennemy*>(child)) {
                    ennemi->damage();
                }
            }
            // supprime la triforce de la scène une fois que le joueur l'a touchée
            m_pScene->removeSpriteFromScene(pCollisionned);
            delete pCollisionned;
        }
    }

    if (!isCollidingWithDecor) {

        if (isInWater) {
            m_playerSpeed = 5;
        } else {
            m_playerSpeed = 10;
        }
    }
}

int GameCore::countEnnemies() {
    int nbreEnnemi = 0;
    auto children = m_pScene->items();

    for(auto child: children) {
        if (Ennemy* ennemi = dynamic_cast<Ennemy*>(child)) {
            nbreEnnemi++;
        }
    }
    return nbreEnnemi;
}

//! Génère une nouvelle vague d'ennemis si la scène est vide.
//! La vague est composée d'un nombre d'ennemis égal au numéro de la vague actuelle.
//! Le numéro de la vague actuelle est incrémenté à chaque fois.
void GameCore::generateEnemyWave() {
    // Vérifier s'il y a déjà des ennemis sur la scène
    if (countEnnemies() == 0) {
        // Créer une nouvelle vague d'ennemis
        EnnemiFactory* ennemiFactory = new EnnemiFactory(m_pScene);

        int nbreEnnemiLeever = 0;
        int nbreEnnemiLeeverRouge = 0;
        int nbreEnnemiOctopus = 0;

        for(int i = 0; i < m_currentWave; i++) {
            // Générer un nombre aléatoire entre 0 et 5 inclus
            int random = QRandomGenerator::global()->bounded(0, 5);
            // Si le nombre est 0, créer un ennemi Leever Rouge
            if (random == 0 && m_currentWave > 2) {
                nbreEnnemiLeeverRouge++;
                // Si le nombre est 1, créer un ennemi Octopus
            } else if(random == 1 && m_currentWave > 5) {
                nbreEnnemiOctopus++;
            } else {
                // Sinon créer un ennemi Leever
                nbreEnnemiLeever++;
            }
        }
        ennemiFactory->createWave(nbreEnnemiLeever, nbreEnnemiLeeverRouge, nbreEnnemiOctopus);

        // Afficher le numéro de la vague
        displayWaves(m_currentWave);
        // Incrémenter le numéro de vague actuel
        m_currentWave++;
        delete ennemiFactory;
    }
}

//! Si un message est déjà affiché, il est remplacé par ce nouveau message.
//! \param rMessage Message à afficher.
void GameCore::displayInformation(const QString& rMessage) {
    clearInformation();

    // Charger la police personnalisée (police du jeu de base Zelda (NES))
    int id = QFontDatabase::addApplicationFont("C:\\Users\\fresale\\JeuZelda\\res\\fonts\\PixelEmulator-xq08.ttf");
    QString policeZelda = QFontDatabase::applicationFontFamilies(id).at(0);

    // Créer la police
    QFont customFont(policeZelda);

    // Agrandit le texte
    customFont.setPointSize(30);

    // Créer l'élément texte avec la police personnalisée
    QGraphicsSimpleTextItem* pText = m_pScene->createText(QPointF(0,0), rMessage, 50, Qt::red);
    pText->setFont(customFont);

    // Centrer le texte
    pText->setPos((m_pScene->width() - pText->boundingRect().width()) / 2,
                  m_pScene->height()/2);

    m_pDisplayedInformation = pText;
}

void GameCore::displayWaves(int waveNumber) {
    // Construire le message avec le numéro de la vague
    QString message = "Vague " + QString::number(waveNumber);

    // Charger la police personnalisée (police du jeu de base Zelda (NES))
    int id = QFontDatabase::addApplicationFont("C:\\Users\\fresale\\JeuZelda\\res\\fonts\\PixelEmulator-xq08.ttf");
    QString policeZelda = QFontDatabase::applicationFontFamilies(id).at(0);

    // Créer la police personnalisée
    QFont customFont(policeZelda);

    // Vérifier si le texte existe déjà, sinon le créer
    if (!m_pDisplayedNbreVagues) {
        // Affichage du message en gras avec la police personnalisée.
        m_pDisplayedNbreVagues = m_pScene->createText(QPointF(0, 0), message, 50, Qt::red);

        // Agrandit le texte
        customFont.setPointSize(30);

        // Place le texte en haut à droite de l'écran
        m_pDisplayedNbreVagues->setX(m_pScene->width() - m_pDisplayedNbreVagues->boundingRect().width() - 80);

        // Applique la police mise à jour
        m_pDisplayedNbreVagues->setFont(customFont);
    } else {
        // Mettre à jour le texte existant avec la nouvelle police
        m_pDisplayedNbreVagues->setText(message);

        // Agrandit le texte
        customFont.setPointSize(30);

        // Applique la police mise à jour
        m_pDisplayedNbreVagues->setFont(customFont);
    }
}

//! Efface le message affiché.
//! Si aucun message n'est affiché, cette fonction ne fait rien.
void GameCore::clearInformation() {
    if (m_pDisplayedInformation != nullptr) {
        delete m_pDisplayedInformation;
        m_pDisplayedInformation = nullptr;
    }
}

void GameCore::restartGame() {
    // Supprime tous les ennemi et les projectiles de la scène
    auto children = m_pScene->items();
    for(auto child: children) {
        if (Ennemy* ennemi = dynamic_cast<Ennemy*>(child)) {
            if(EnnemiOctopus* ennemiOctopus = dynamic_cast<EnnemiOctopus*>(child)) {
                ennemiOctopus->removeProjectile();
            }
            m_pScene->removeSpriteFromScene(ennemi);
            delete ennemi;
        }
    }

    // Supprime le joueur
    delete m_pPlayer;

    // Réinitialise le numéro de vague
    m_currentWave = 1;

    // Réinitialise le joueur
    m_pPlayer = new Player();
    m_pScene->addSpriteToScene(m_pPlayer);
    m_pPlayer->setOffset(-m_pPlayer->sceneBoundingRect().width()/2, -m_pPlayer->sceneBoundingRect().width()/2);
    m_pPlayer->setScale(PLAYER_SCALE_FACTOR);
    m_pPlayer->setPos(m_pScene->width()/2.0, m_pScene->height()/2.0);
    m_pPlayer->initializeHearts();

    // Réinitialise le mode de jeu
    m_gameMode = RUNNING;

    // Efface le texte affiché
    clearInformation();

    // Redémarre le tick arrêté au moment du Game Over
    m_pGameCanvas->startTick();
}

void GameCore::updatePlayer() {
    // Déplacer le personnage en fonction de l'état des touches
    if(!m_pressedKeys.isEmpty()) {
        int mostRecentKey = m_pressedKeys.first();
        if (mostRecentKey == Qt::Key_Left && m_pPlayer->x() - m_playerSpeed >= 0) {
            m_pPlayer->addAnimationFrame(GameFramework::imagesPath() + "JeuZelda/LeftLink_2.gif");
            m_pPlayer->addAnimationFrame(GameFramework::imagesPath() + "JeuZelda/LeftLink_1.gif");
            m_pPlayer->setX(m_pPlayer->x() - m_playerSpeed);
        }
        if (mostRecentKey == Qt::Key_Right && m_pPlayer->x() + m_playerSpeed <= m_pScene->width() - m_pPlayer->sceneBoundingRect().width()) {
            m_pPlayer->addAnimationFrame(GameFramework::imagesPath() + "JeuZelda/RightLink_2.gif");
            m_pPlayer->addAnimationFrame(GameFramework::imagesPath() + "JeuZelda/RightLink_1.gif");
            m_pPlayer->setX(m_pPlayer->x() + m_playerSpeed);
        }
        if (mostRecentKey == Qt::Key_Up && m_pPlayer->y() - m_playerSpeed >= 0) {
            m_pPlayer->addAnimationFrame(GameFramework::imagesPath() + "JeuZelda/UpLink_2.gif");
            m_pPlayer->addAnimationFrame(GameFramework::imagesPath() + "JeuZelda/UpLink_1.gif");
            m_pPlayer->setY(m_pPlayer->y() - m_playerSpeed);
        }
        if (mostRecentKey == Qt::Key_Down && m_pPlayer->y() + m_playerSpeed <= m_pScene->height() - m_pPlayer->sceneBoundingRect().height()) {
            m_pPlayer->addAnimationFrame(GameFramework::imagesPath() + "JeuZelda/DownLink_2.gif");
            m_pPlayer->addAnimationFrame(GameFramework::imagesPath() + "JeuZelda/DownLink_1.gif");
            m_pPlayer->setY(m_pPlayer->y() + m_playerSpeed);
        }
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

