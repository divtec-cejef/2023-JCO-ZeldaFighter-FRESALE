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
#include "ennemifactory.h"
#include "EnnemiLeever.h"
#include "ennemileeverrouge.h"
#include "ennemioctopus.h"
#include "decor.h"

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

    // Crée un nouveau joueur
    m_pPlayer = new Player();
    m_pScene->addSpriteToScene(m_pPlayer);

    // Centre le joueur au millieu de la scène.
    m_pPlayer->setOffset(-m_pPlayer->sceneBoundingRect().width()/2, -m_pPlayer->sceneBoundingRect().width()/2);
    m_pPlayer->setScale(PLAYER_SCALE_FACTOR);
    m_pPlayer->setPos(m_pScene->width()/2.0, m_pScene->height()/2.0);

    // Rend le joueur invisible au début du jeu
    m_pPlayer->setVisible(false);

    // Création des ennemis grâce à la classe EnnemiFactory
    // EnnemiFactory* ennemifactory = new EnnemiFactory(m_pScene, m_pPlayer);
    // ennemifactory->createWave(20,20,20);

    // Démarre le tick pour que les animations qui en dépendent fonctionnent correctement.
    // Attention : il est important que l'enclenchement du tick soit fait vers la fin de cette fonction,
    // sinon le temps passé jusqu'au premier tick (ElapsedTime) peut être élevé et provoquer de gros
    // déplacements, surtout si le déboggueur est démarré.
    m_pGameCanvas->startTick();

    m_gameMode = START;

    // Affiche les levels et le titre du jeu au joueur
    displayLevelInformation();

    // Affiche les items et leur utilité au joueur
    displayItemsInformation();

    // Affiche les ennemis au joueur
    displayEnnemyInformation();

    // Affiche le meilleur score du joueur
    displayBestScore();
}

//! Destructeur de GameCore : efface les scènes
GameCore::~GameCore() {
    delete m_pScene;
    m_pScene = nullptr;
}

void GameCore::keyPressed(int key) {
    // Mettre à jour l'état de la touche correspondante
    switch (key) {
    case Qt::Key_1:
        // Création du niveau "Field of Hyrule"
        if(m_gameMode == START) {
            m_gameMode = RUNNING;

            // Création des décors
            Decor* pBush1 = new Decor(GameFramework::imagesPath() + "JeuZelda/Bush.png", 200, 200);
            m_pScene->addSpriteToScene(pBush1);

            Decor* pBush2 = new Decor(GameFramework::imagesPath() + "JeuZelda/Bush.png", 300, 550);
            m_pScene->addSpriteToScene(pBush2);

            Decor* pRock1 = new Decor(GameFramework::imagesPath() + "JeuZelda/Rock.png", 1000, 200);
            m_pScene->addSpriteToScene(pRock1);

            // Fond d'écran de la scène.
            m_pScene->setBackgroundColor(QColor(252, 216, 168));

            // Initialise les coeurs du joueur
            m_pPlayer->initializeHearts();

            qDebug() << "Level 1 crée";
        }
        break;
    case Qt::Key_2:
        // Création du niveau "Riverside"
        if(m_gameMode == START) {
            m_gameMode = RUNNING;

            // Création des décors
            Decor* pBush1 = new Decor(GameFramework::imagesPath() + "JeuZelda/Bush.png", 200, 200);
            m_pScene->addSpriteToScene(pBush1);

            Decor* pBush2 = new Decor(GameFramework::imagesPath() + "JeuZelda/Bush.png", 300, 550);
            m_pScene->addSpriteToScene(pBush2);

            Decor* pBush3 = new Decor(GameFramework::imagesPath() + "JeuZelda/Bush.png", 900, 400);
            m_pScene->addSpriteToScene(pBush3);

            Decor* pRock1 = new Decor(GameFramework::imagesPath() + "JeuZelda/Rock.png", 1000, 200);
            m_pScene->addSpriteToScene(pRock1);

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

            // Fond d'écran de la scène.
            m_pScene->setBackgroundColor(QColor(252, 216, 168));

            // Initialise les coeurs du joueur
            m_pPlayer->initializeHearts();

            qDebug() << "Level 2 crée";
        }
        break;
    case Qt::Key_3:
        // création du niveau "Death Mountain"
        if(m_gameMode == START) {
            m_gameMode = RUNNING;

            // Création des décors
            Decor* pRock1 = new Decor(GameFramework::imagesPath() + "JeuZelda/WhiteRock.png", 200, 200);
            m_pScene->addSpriteToScene(pRock1);

            Decor* pRock2 = new Decor(GameFramework::imagesPath() + "JeuZelda/WhiteRock.png", 650, 500);
            m_pScene->addSpriteToScene(pRock2);

            Decor* pBush = new Decor(GameFramework::imagesPath() + "JeuZelda/WhiteBush.png", 1010, 350);
            m_pScene->addSpriteToScene(pBush);

            // Création du feu
            Sprite* pFire = new Sprite(GameFramework::imagesPath() + "JeuZelda/Fire_1.gif");
            pFire->addAnimationFrame(GameFramework::imagesPath() + "JeuZelda/Fire_1.gif");
            pFire->addAnimationFrame(GameFramework::imagesPath() + "JeuZelda/Fire_2.gif");
            pFire->setAnimationSpeed(100);
            pFire->startAnimation();
            m_pScene->addSpriteToScene(pFire);
            pFire->setScale(DECOR_SCALE_FACTOR);
            pFire->setPos(100, 100);
            pFire->setData(SPRITE_TYPE_KEY, SpriteType::FIRE);

            Sprite* pFire2 = new Sprite(GameFramework::imagesPath() + "JeuZelda/Fire_1.gif");
            pFire2->addAnimationFrame(GameFramework::imagesPath() + "JeuZelda/Fire_1.gif");
            pFire2->addAnimationFrame(GameFramework::imagesPath() + "JeuZelda/Fire_2.gif");
            pFire2->setAnimationSpeed(100);
            pFire2->startAnimation();
            m_pScene->addSpriteToScene(pFire2);
            pFire2->setScale(DECOR_SCALE_FACTOR);
            pFire2->setPos(700, 180);
            pFire2->setData(SPRITE_TYPE_KEY, SpriteType::FIRE);

            Sprite* pFire3 = new Sprite(GameFramework::imagesPath() + "JeuZelda/Fire_1.gif");
            pFire3->addAnimationFrame(GameFramework::imagesPath() + "JeuZelda/Fire_1.gif");
            pFire3->addAnimationFrame(GameFramework::imagesPath() + "JeuZelda/Fire_2.gif");
            pFire3->setAnimationSpeed(100);
            pFire3->startAnimation();
            m_pScene->addSpriteToScene(pFire3);
            pFire3->setScale(DECOR_SCALE_FACTOR);
            pFire3->setPos(1000, 70);
            pFire3->setData(SPRITE_TYPE_KEY, SpriteType::FIRE);

            Sprite* pFire4 = new Sprite(GameFramework::imagesPath() + "JeuZelda/Fire_1.gif");
            pFire4->addAnimationFrame(GameFramework::imagesPath() + "JeuZelda/Fire_1.gif");
            pFire4->addAnimationFrame(GameFramework::imagesPath() + "JeuZelda/Fire_2.gif");
            pFire4->setAnimationSpeed(100);
            pFire4->startAnimation();
            m_pScene->addSpriteToScene(pFire4);
            pFire4->setScale(DECOR_SCALE_FACTOR);
            pFire4->setPos(900, 550);
            pFire4->setData(SPRITE_TYPE_KEY, SpriteType::FIRE);

            Sprite* pFire5 = new Sprite(GameFramework::imagesPath() + "JeuZelda/Fire_1.gif");
            pFire5->addAnimationFrame(GameFramework::imagesPath() + "JeuZelda/Fire_1.gif");
            pFire5->addAnimationFrame(GameFramework::imagesPath() + "JeuZelda/Fire_2.gif");
            pFire5->setAnimationSpeed(100);
            pFire5->startAnimation();
            m_pScene->addSpriteToScene(pFire5);
            pFire5->setScale(DECOR_SCALE_FACTOR);
            pFire5->setPos(300, 400);
            pFire5->setData(SPRITE_TYPE_KEY, SpriteType::FIRE);

            // Fond d'écran de la scène.
            m_pScene->setBackgroundColor(QColor(120, 116, 116));

            // Initialise les coeurs du joueur
            m_pPlayer->initializeHearts();

            qDebug() << "Level 3 crée";
        }
        break;
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
        break;

    case Qt::Key_A:
        isAKeyPressed = true;
        break;

    case Qt::Key_S:
        isSKeyPressed = true;
        break;

    case Qt::Key_D:
        isDKeyPressed = true;
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
        case RUNNING :
            // on presse sur space, le jeu se met en pause
            if(m_pGameCanvas->isTicking()) {
                m_pGameCanvas->stopTick();
                m_gameMode = PAUSE;
                qDebug() << "Jeu en pause";
                displayInformation("Presse space to continue \n"
                                   "Presse escape to restart");
            }
            break;
        case PAUSE:
            // on presse sur space, le jeu reprend
            m_gameMode = RUNNING;
            clearDisplayInformation();
            m_pGameCanvas->startTick();
            break;
        case ENDED_LOSE:
            restartGame();
            break;
        }
    }

    if(key == Qt::Key_Escape) {
        switch (m_gameMode) {
        case PAUSE:
            qDebug() << "Le joueur quitte la partie";
            restartGame();
            break;
        }
    }

    // Réinitialiser l'état de la touche correspondante
    switch (key) {
    case Qt::Key_Left:
        isLeftKeyPressed = false;
        m_pressedKeys.removeAll(key);
        break;
    case Qt::Key_Right:
        isRightKeyPressed = false;
        m_pressedKeys.removeAll(key);
        break;
    case Qt::Key_Up:
        isUpKeyPressed = false;
        m_pressedKeys.removeAll(key);
        break;
    case Qt::Key_Down:
        isDownKeyPressed = false;
        m_pressedKeys.removeAll(key);
        break;
    case Qt::Key_W:
        isWKeyPressed = false;
        break;
    case Qt::Key_A:
        isAKeyPressed = false;
        break;
    case Qt::Key_S:
        isSKeyPressed = false;
        break;
    case Qt::Key_D:
        isDKeyPressed = false;
        break;
    default:
        // default
        break;
    }
}


//! \param elapsedTimeInMilliseconds Le temps écoulé depuis le dernier appel à cette fonction.
//! Cette fonction est appelée à chaque tick du jeu.
void GameCore::tick(long long elapsedTimeInMilliseconds) {
    m_pPlayer->tick(static_cast<int>(elapsedTimeInMilliseconds));

    auto children = m_pScene->items();

    // Boucle qui permet de faire bouger les ennemies grâce au tick
    for(auto child: children) {
        if (Ennemy* ennemi = dynamic_cast<Ennemy*>(child)) {
            ennemi->tick(elapsedTimeInMilliseconds);
        }
    }
    // Appel de la fonction qui compte le nombre d'ennemi encore en vie sur la scène
    countEnnemies();
    // Appel de la fonction qui génère une nouvelle vague d'ennemis si aucun ennemi n'est présent sur la scène
    generateEnemyWave();

    // Si le joueur est en train de jouer, on efface les informations de l'écran de début
    if(m_gameMode == RUNNING) {
        // Rend le joueur visible au lancement de la partie
        m_pPlayer->setVisible(true);
        // Permet au joueur de se déplacer
        updatePlayer();
        // clear toutes les informations du menu principal
        clearLevelInformation();
        clearItemsInformation();
        clearEnnemyInformation();
        clearBestScoreInformation();
    } else {
        // Rend le joueur invisible s'il ne se trouve pas dans une partie (RUNNING)
        m_pPlayer->setVisible(false);
    }

    QList<Sprite*> collisions = m_pScene->collidingSprites(m_pPlayer);
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
        } else if (pCollisionned->data(SPRITE_TYPE_KEY).toInt() == ENNEMI || pCollisionned->data(SPRITE_TYPE_KEY).toInt() == FIRE) {
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
                qDebug() << "Coeur ajouté";
            }
            // supprime le coeur de la scène une fois que le joueur l'a touchée
            m_pScene->removeSpriteFromScene(pCollisionned);
            delete pCollisionned;
        } else if(pCollisionned->data(SPRITE_TYPE_KEY).toInt() == BLUE_RING) {
            // Le projectile (épée) du joueur va 2 fois plus vite pendant 5 secondes
            qDebug() << "Blue Ring touché";
            // Si le joueur n'a pas déjà l'effet d'un Blue Ring, alors il gagne l'effet
                if(m_pPlayer->swordSpeed == 550.0) {
                m_pPlayer->swordSpeed = 1000.0;
            }

            // Après 5 secondes, la vitesse de l'épée du joueur revient à la normale
            QTimer::singleShot(5000, m_pPlayer, [this]() {
                m_pPlayer->swordSpeed = 550.0;
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
}

//! Fonction qui compte le nombre d'ennemi sur la scène
//! \return le nombre d'ennemi sur la scène
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
        EnnemiFactory* ennemiFactory = new EnnemiFactory(m_pScene, m_pPlayer);

        int nbreEnnemiLeever = 0;
        int nbreEnnemiLeeverRouge = 0;
        int nbreEnnemiOctopus = 0;

        if(m_gameMode == RUNNING) {
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
            // Créer la vague d'ennemis
            ennemiFactory->createWave(nbreEnnemiLeever, nbreEnnemiLeeverRouge, nbreEnnemiOctopus);

            // Afficher le numéro de la vague
            displayWaves(m_currentWave);

            // Incrémenter le numéro de vague actuel
            m_currentWave++;
            delete ennemiFactory;
        }
    }
}

//! Si un message est déjà affiché, il est remplacé par ce nouveau message.
//! \param rMessage Message à afficher.
void GameCore::displayInformation(const QString& rMessage) {
    clearDisplayInformation();

    // Charger la police personnalisée (police du jeu de base Zelda (NES))
    int id = QFontDatabase::addApplicationFont("C:\\Users\\fresale\\JeuZelda\\res\\fonts\\PixelEmulator-xq08.ttf");
    QString ZeldaFont = QFontDatabase::applicationFontFamilies(id).at(0);

    // Créer la police
    QFont customFont(ZeldaFont);

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

//! \brief GameCore::displayWaves
//! \param waveNumber Le numéro de la vague actuel
//! Affiche le numéro de la vague actuel
void GameCore::displayWaves(int waveNumber) {
    // Construire le message avec le numéro de la vague
    QString message = "Wave " + QString::number(waveNumber);

    // Charger la police personnalisée (police du jeu de base Zelda (NES))
    int id = QFontDatabase::addApplicationFont("C:\\Users\\fresale\\JeuZelda\\res\\fonts\\PixelEmulator-xq08.ttf");
    QString ZeldaFont = QFontDatabase::applicationFontFamilies(id).at(0);

    // Créer la police personnalisée
    QFont customFont(ZeldaFont);

    // Vérifier si le texte existe déjà, sinon le créer
    if (!m_pDisplayedNumberWaves) {
        // Affichage du message en gras avec la police personnalisée.
        m_pDisplayedNumberWaves = m_pScene->createText(QPointF(0, 0), message, 50, Qt::red);

        // Agrandit le texte
        customFont.setPointSize(30);

        // Place le texte en haut à droite de l'écran
        m_pDisplayedNumberWaves->setX(m_pScene->width() - m_pDisplayedNumberWaves->boundingRect().width() - 80);

        // Applique la police mise à jour
        m_pDisplayedNumberWaves->setFont(customFont);
    } else {
        // Mettre à jour le texte existant avec la nouvelle police
        m_pDisplayedNumberWaves->setText(message);

        // Agrandit le texte
        customFont.setPointSize(30);

        // Applique la police mise à jour
        m_pDisplayedNumberWaves->setFont(customFont);
    }
}

//! \brief GameCore::displayLevelInformation
//! Affiche les levels et le titre du jeu
void GameCore::displayLevelInformation() {
    // Charger la police personnalisée (police du jeu de base Zelda (NES))
    int id = QFontDatabase::addApplicationFont("C:\\Users\\fresale\\JeuZelda\\res\\fonts\\PixelEmulator-xq08.ttf");
    QString ZeldaFont = QFontDatabase::applicationFontFamilies(id).at(0);

    // Créer la police personnalisée
    QFont customFont(ZeldaFont);

    // Vérifier si le texte existe déjà, sinon le créer
    if (!m_pDisplayedLevelInformation) {
        // Affichage du message en gras avec la police personnalisée.
        m_pDisplayedLevelInformation = m_pScene->createText(QPointF(0, 0), "THE LEGEND OF ZELDA FIGHTER \n"
                                                                           "\n"
                                                                           "    1 - Field of Hyrule \n"
                                                                           "    2 - Riverside \n"
                                                                           "    3 - Death Mountain \n", 50, Qt::white);

        // Agrandit le texte
        customFont.setPointSize(24);

        // Place le texte en haut au milieu de l'écran
        m_pDisplayedLevelInformation->setX((m_pScene->width() - m_pDisplayedLevelInformation->boundingRect().width()) / 2);

        // Applique la police mise à jour
        m_pDisplayedLevelInformation->setFont(customFont);
    } else {
        // Agrandit le texte
        customFont.setPointSize(30);

        // Applique la police mise à jour
        m_pDisplayedLevelInformation->setFont(customFont);
    }
}

//! \brief GameCore::displayItemsInformation
//! Affiche les items et leur utilité
void GameCore::displayItemsInformation() {
    // Charger la police personnalisée (police du jeu de base Zelda (NES))
    int id = QFontDatabase::addApplicationFont("C:\\Users\\fresale\\JeuZelda\\res\\fonts\\PixelEmulator-xq08.ttf");
    QString ZeldaFont = QFontDatabase::applicationFontFamilies(id).at(0);

    // Créer la police personnalisée
    QFont customFont(ZeldaFont);

    // Affichage du message en gras avec la police personnalisée.
    m_pDisplayedtextHeart = m_pScene->createText(QPointF(0, 0), "Heart : gives to player an extra hearth", 50, Qt::white);
    m_pDisplayedtextBlueRing = m_pScene->createText(QPointF(0, 0), "Blue Ring : increases swords speed for a short time.", 50, Qt::white);
    m_pDisplayedtextTriforce = m_pScene->createText(QPointF(0, 0), "Triforce : causes all enemies in the wave to lose one hp", 50, Qt::white);

    // Agrandit le texte
    customFont.setPointSize(16);

    // Crée un Coeur
    Sprite* pHeart = new Sprite(GameFramework::imagesPath() + "JeuZelda/HearthOnGround1.gif");
    pHeart->addAnimationFrame(GameFramework::imagesPath() + "JeuZelda/HearthOnGround1.gif");
    pHeart->addAnimationFrame(GameFramework::imagesPath() + "JeuZelda/HearthOnGround2.gif");
    pHeart->setAnimationSpeed(200);
    pHeart->startAnimation();
    m_pScene->addSpriteToScene(pHeart);
    pHeart->setScale(ITEM_DROP_SCALE_FACTOR);
    pHeart->setPos(100, 300);
    m_pHeart = pHeart;

    // Place le texte juste a droite du coeur a la même hauteur
    m_pDisplayedtextHeart->setX(pHeart->x() + pHeart->boundingRect().width() + 40);
    m_pDisplayedtextHeart->setY(pHeart->y());

    // Applique la police mise à jour
    m_pDisplayedtextHeart->setFont(customFont);

    // Crée un Blue Ring
    Sprite* pBlueRing = new Sprite(GameFramework::imagesPath() + "JeuZelda/BlueRing.png");
    pBlueRing->addAnimationFrame(GameFramework::imagesPath() + "JeuZelda/BlueRing.png");
    pBlueRing->addAnimationFrame(GameFramework::imagesPath() + "JeuZelda/RedRing.png");
    pBlueRing->setAnimationSpeed(200);
    pBlueRing->startAnimation();
    m_pScene->addSpriteToScene(pBlueRing);
    pBlueRing->setScale(ITEM_DROP_SCALE_FACTOR);
    pBlueRing->setPos(100, 350);
    m_pBlueRing = pBlueRing;

    // Place le texte juste a droite du coeur a la même hauteur
    m_pDisplayedtextBlueRing->setX(pBlueRing->x() + pBlueRing->boundingRect().width() + 40);
    m_pDisplayedtextBlueRing->setY(pBlueRing->y());

    // Applique la police mise à jour
    m_pDisplayedtextBlueRing->setFont(customFont);

    // Crée une triforce
    Sprite* pTriforce = new Sprite(GameFramework::imagesPath() + "JeuZelda/Triforce1.gif");
    pTriforce->addAnimationFrame(GameFramework::imagesPath() + "JeuZelda/Triforce1.gif");
    pTriforce->addAnimationFrame(GameFramework::imagesPath() + "JeuZelda/Triforce2.gif");
    pTriforce->setAnimationSpeed(200);
    pTriforce->startAnimation();
    m_pScene->addSpriteToScene(pTriforce);
    pTriforce->setScale(ITEM_DROP_SCALE_FACTOR);
    pTriforce->setPos(100, 400);
    m_pTriforce = pTriforce;

    // Place le texte juste a droite de la tiforce a la même hauteur
    m_pDisplayedtextTriforce->setX(pTriforce->x() + pTriforce->boundingRect().width() + 40);
    m_pDisplayedtextTriforce->setY(pTriforce->y());

    // Applique la police mise à jour
    m_pDisplayedtextTriforce->setFont(customFont);
}

//! \brief GameCore::displayEnnemyInformation
//! Affiche les ennemis et leur nom
void GameCore::displayEnnemyInformation() {
    // Charger la police personnalisée (police du jeu de base Zelda (NES))
    int id = QFontDatabase::addApplicationFont("C:\\Users\\fresale\\JeuZelda\\res\\fonts\\PixelEmulator-xq08.ttf");
    QString ZeldaFont = QFontDatabase::applicationFontFamilies(id).at(0);

    // Créer la police personnalisée
    QFont customFont(ZeldaFont);

    // Affichage du message en gras avec la police personnalisée.
    m_pDisplayedtextLeever = m_pScene->createText(QPointF(0, 0), "Leever", 50, Qt::white);
    m_pDisplayedtextLeeverRouge = m_pScene->createText(QPointF(0, 0), "Red Leever", 50, Qt::white);
    m_pDisplayedtextOctopus = m_pScene->createText(QPointF(0, 0), "Octorock", 50, Qt::white);

    // Agrandit le texte
    customFont.setPointSize(16);

    // Crée un Sprite reprsentant un ennemi Leever
    Sprite* pLeever = new Sprite(GameFramework::imagesPath() + "JeuZelda/Ennemi1_1.gif");
    pLeever->addAnimationFrame(GameFramework::imagesPath() + "JeuZelda/Ennemi1_1.gif");
    pLeever->addAnimationFrame(GameFramework::imagesPath() + "JeuZelda/Ennemi1_2.gif");
    pLeever->setAnimationSpeed(200);
    pLeever->startAnimation();
    m_pScene->addSpriteToScene(pLeever);
    pLeever->setScale(START_ENNEMY_SCALE_FACTOR);
    pLeever->setData(SPRITE_TYPE_KEY, SpriteType::ENNEMI);
    pLeever->setPos(100, 500);
    m_pLeever = pLeever;

    // Place le texte juste a droite de l'ennemi a la même hauteur
    m_pDisplayedtextLeever->setX(pLeever->x() + pLeever->boundingRect().width() + 40);
    m_pDisplayedtextLeever->setY(pLeever->y());

    // Applique la police mise à jour
    m_pDisplayedtextLeever->setFont(customFont);

    // Crée un Sprite représentant un ennemi Leever Rouge
    Sprite* pLeeverRouge = new Sprite(GameFramework::imagesPath() + "JeuZelda/Ennemi2_1.gif");
    pLeeverRouge->addAnimationFrame(GameFramework::imagesPath() + "JeuZelda/Ennemi2_1.gif");
    pLeeverRouge->addAnimationFrame(GameFramework::imagesPath() + "JeuZelda/Ennemi2_2.gif");
    pLeeverRouge->setAnimationSpeed(200);
    pLeeverRouge->startAnimation();
    m_pScene->addSpriteToScene(pLeeverRouge);
    pLeeverRouge->setScale(START_ENNEMY_SCALE_FACTOR);
    pLeeverRouge->setData(SPRITE_TYPE_KEY, SpriteType::ENNEMI);
    pLeeverRouge->setPos(100, 560);
    m_pLeeverRouge = pLeeverRouge;

    // Place le texte juste a droite de l'ennemi a la même hauteur
    m_pDisplayedtextLeeverRouge->setX(pLeeverRouge->x() + pLeeverRouge->boundingRect().width() + 40);
    m_pDisplayedtextLeeverRouge->setY(pLeeverRouge->y());

    // Applique la police mise à jour
    m_pDisplayedtextLeeverRouge->setFont(customFont);

    // Crée une Sprite représentant un ennemi Octopus
    Sprite* pOctopus = new Sprite(GameFramework::imagesPath() + "JeuZelda/EnnemiOctopus_1.gif");
    pOctopus->addAnimationFrame(GameFramework::imagesPath() + "JeuZelda/EnnemiOctopus_1.gif");
    pOctopus->addAnimationFrame(GameFramework::imagesPath() + "JeuZelda/EnnemiOctopus_2.gif");
    pOctopus->setAnimationSpeed(200);
    pOctopus->startAnimation();
    m_pScene->addSpriteToScene(pOctopus);
    pOctopus->setData(SPRITE_TYPE_KEY, SpriteType::ENNEMI);
    pOctopus->setScale(START_ENNEMY_SCALE_FACTOR);
    pOctopus->setPos(100, 620);
    m_pOctopus = pOctopus;

    // Place le texte juste à droite de l'ennemi a la même hauteur
    m_pDisplayedtextOctopus->setX(pOctopus->x() + pOctopus->boundingRect().width() + 40);
    m_pDisplayedtextOctopus->setY(pOctopus->y());

    // Applique la police mise à jour
    m_pDisplayedtextOctopus->setFont(customFont);
}

//! \brief GameCore::displayBestScore
//! Affiche le meilleur score
//! Le meilleur score correspond à la vague maximum atteint par le joueur.
void GameCore::displayBestScore() {
    // Charger la police personnalisée (police du jeu de base Zelda (NES))
    int id = QFontDatabase::addApplicationFont("C:\\Users\\fresale\\JeuZelda\\res\\fonts\\PixelEmulator-xq08.ttf");
    QString ZeldaFont = QFontDatabase::applicationFontFamilies(id).at(0);

    // Créer la police personnalisée
    QFont customFont(ZeldaFont);

    // Affichage du message en gras avec la police personnalisée.
    m_pDisplayedBestScore = m_pScene->createText(QPointF(0, 0), "Best Score : " + QString::number(m_bestScore), 50, Qt::white);

    // Agrandit le texte
    customFont.setPointSize(16);

    // Place le texte en haut à droite de l'écran
    m_pDisplayedBestScore->setX(m_pScene->width() - m_pDisplayedBestScore->boundingRect().width() + 50);

    // Applique la police mise à jour
    m_pDisplayedBestScore->setFont(customFont);
}

//! Efface le message affiché.
//! Si aucun message n'est affiché, cette fonction ne fait rien.
void GameCore::clearDisplayInformation() {
    if (m_pDisplayedInformation != nullptr) {
        delete m_pDisplayedInformation;
        m_pDisplayedInformation = nullptr;
    }
}

//! Efface le numéro de la vague affiché.
void GameCore::clearLevelInformation() {
    if (m_pDisplayedLevelInformation != nullptr) {
        delete m_pDisplayedLevelInformation;
        m_pDisplayedLevelInformation = nullptr;
    }
}

//! Efface le numéro de la vague affiché.
void GameCore::clearWavesInformation() {
    if (m_pDisplayedNumberWaves != nullptr) {
        delete m_pDisplayedNumberWaves;
        m_pDisplayedNumberWaves = nullptr;
    }
}

//! Efface les informations sur les items affichées.
void GameCore::clearItemsInformation() {
    if (m_pDisplayedtextHeart != nullptr) {
        delete m_pDisplayedtextHeart;
        m_pDisplayedtextHeart = nullptr;
    }
    if (m_pDisplayedtextBlueRing != nullptr) {
        delete m_pDisplayedtextBlueRing;
        m_pDisplayedtextBlueRing = nullptr;
    }
    if (m_pDisplayedtextTriforce != nullptr) {
        delete m_pDisplayedtextTriforce;
        m_pDisplayedtextTriforce = nullptr;
    }
    if(m_pHeart != nullptr) {
        m_pScene->removeSpriteFromScene(m_pHeart);
        delete m_pHeart;
        m_pHeart = nullptr;
    }
    if(m_pBlueRing != nullptr) {
        m_pScene->removeSpriteFromScene(m_pBlueRing);
        delete m_pBlueRing;
        m_pBlueRing = nullptr;
    }
    if(m_pTriforce != nullptr) {
        m_pScene->removeSpriteFromScene(m_pTriforce);
        delete m_pTriforce;
        m_pTriforce = nullptr;
    }
}

//! Efface les informations sur les ennemis affichées.
void GameCore::clearEnnemyInformation() {
    if (m_pDisplayedtextLeever != nullptr) {
        delete m_pDisplayedtextLeever;
        m_pDisplayedtextLeever = nullptr;
    }
    if (m_pDisplayedtextLeeverRouge != nullptr) {
        delete m_pDisplayedtextLeeverRouge;
        m_pDisplayedtextLeeverRouge = nullptr;
    }
    if (m_pDisplayedtextOctopus != nullptr) {
        delete m_pDisplayedtextOctopus;
        m_pDisplayedtextOctopus = nullptr;
    }
    if(m_pLeever != nullptr) {
        m_pScene->removeSpriteFromScene(m_pLeever);
        delete m_pLeever;
        m_pLeever = nullptr;
    }
    if(m_pLeeverRouge != nullptr) {
        m_pScene->removeSpriteFromScene(m_pLeeverRouge);
        delete m_pLeeverRouge;
        m_pLeeverRouge = nullptr;
    }
    if(m_pOctopus != nullptr) {
        m_pScene->removeSpriteFromScene(m_pOctopus);
        delete m_pOctopus;
        m_pOctopus = nullptr;
    }
}

//! Efface le meilleur score affiché.
void GameCore::clearBestScoreInformation() {
    if (m_pDisplayedBestScore != nullptr) {
        delete m_pDisplayedBestScore;
        m_pDisplayedBestScore = nullptr;
    }
}

//! \brief GameCore::removeSpriteByType
//! \param spriteType Type du sprite à supprimer
//! Supprime un sprite de la scène en fonction de son type
void GameCore::removeSpriteByType(int spriteType) {
    auto children = m_pScene->items();
    for(auto child : children) {
        if(Sprite* sprite = dynamic_cast<Sprite*>(child)) {
            if(sprite->data(SPRITE_TYPE_KEY).toInt() == spriteType) {
                m_pScene->removeSpriteFromScene(sprite);
                delete sprite;
            }
        }
    }
}

//!
//! \brief GameCore::restartGame
//! Réinitialise le jeu
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

    // Supprime tous les items de la scène par type (coeur, blue ring, triforce, décor et feu)
    removeSpriteByType(HEARTDROP);
    removeSpriteByType(BLUE_RING);
    removeSpriteByType(TRIFORCE);
    removeSpriteByType(DECOR);
    removeSpriteByType(FIRE);

    // Regarde si le score à afficher dans le meilleur score doit changer
    if(m_currentWave > m_bestScore) {
        m_bestScore = m_currentWave - 1;
    }

    // remet le fond en noir
    m_pScene->setBackgroundColor(QColor(0, 0, 0));

    // Supprime l'épée du joueur si elle est encore présente au moment du Game Over
    m_pPlayer->removeSword();

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

    // Réinitialise le mode de jeu à START
    m_gameMode = START;

    // Efface le texte affiché
    clearDisplayInformation();

    // Efface le texte des vagues
    clearWavesInformation();

    // Affiche les levels et le titre du jeu au joueur
    displayLevelInformation();

    // Affiche les items et leurs descriptions au joueur
    displayItemsInformation();

    // Affiche les ennemis et leurs descriptions au joueur
    displayEnnemyInformation();

    // Affiche le meilleur score du joueur
    displayBestScore();

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
    // Oriente l'attaque du joueur en fonction de la touche appuyée (W, A, S ou D)
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

