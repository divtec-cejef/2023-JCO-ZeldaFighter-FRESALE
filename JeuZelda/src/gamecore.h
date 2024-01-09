/**
  \file
  \brief    Déclaration de la classe GameCore.
  \author   JCO
  \date     septembre 2018
*/
#ifndef GAMECORE_H
#define GAMECORE_H

#include <QObject>
#include <QPointF>
#include <QMovie>

class GameCanvas;
class GameScene;
class Sprite;
class Player;
class EnnemiLeever;
class EnnemiFactory;
class QGraphicsItem;
class Projectile;
class QGraphicsSimpleTextItem;

//! \brief Classe qui gère la logique du jeu.
//!
//! Dans son état actuel, cette classe crée une scène vide, délimite
//! sa surface avec une ligne blanche puis démarre la cadence du jeu.
class GameCore : public QObject
{
    Q_OBJECT
public:
    enum SpriteType {
        DECOR,
        PLAYER,
        WATER,
        HEART,
        ENNEMI,
        FIRE,
        PROJECTIL,
        HEARTDROP,
        BLUE_RING,
        TRIFORCE
    };

    enum GameMode {
        RUNNING,
        PAUSE,
        ENDED_LOSE,
        START
    };

    enum SpriteDataKey {
        SPRITE_TYPE_KEY = 0
    };

    explicit GameCore(GameCanvas* pGameCanvas, QObject *parent = nullptr);
    ~GameCore() override;

    void keyPressed(int key);
    void keyReleased(int key);

    void mouseMoved(QPointF newMousePosition);
    void mouseButtonPressed(QPointF mousePosition, Qt::MouseButtons buttons);
    void mouseButtonReleased(QPointF mousePosition, Qt::MouseButtons buttons);

    void tick(long long elapsedTimeInMilliseconds);
    void updatePlayer();
    void ThrowSword();
    void restorePlayerOpacity();

    int countEnnemies();
    void generateEnemyWave();

    void displayInformation(const QString& rMessage);
    void displayWaves(int waveNumber);
    void displayLevelInformation();
    void displayItemsInformation();
    void displayEnnemyInformation();
    void displayBestScore();
    void removeSpriteByType(int spriteType);
    void restartGame();
    void removeItemsByType(int spriteType);
    void clearDisplayInformation();
    void clearLevelInformation();
    void clearWavesInformation();
    void clearItemsInformation();
    void clearEnnemyInformation();
    void clearBestScoreInformation();

    GameMode m_gameMode = RUNNING;

    static constexpr int SCENE_WIDTH = 1280;
    static constexpr float PLAYER_SCALE_FACTOR = 4;
    static constexpr float DECOR_SCALE_FACTOR = 5;
    static constexpr float WATER_SCALE_FACTOR = 4;
    static constexpr float ITEM_DROP_SCALE_FACTOR = 3.8;
    static constexpr float START_ENNEMY_SCALE_FACTOR = 3.2;
    static constexpr int MAX_HEARTH = 5;

signals:
    void notifyMouseMoved(QPointF newMousePosition);
    void notifyMouseButtonPressed(QPointF mousePosition, Qt::MouseButtons buttons);
    void notifyMouseButtonReleased(QPointF mousePosition, Qt::MouseButtons buttons);
    void notifyKeyPressed(int key);
    void notifyKeyReleased(int key);


private:

    GameCanvas* m_pGameCanvas = nullptr;
    GameScene* m_pScene = nullptr;
    Player*  m_pPlayer = nullptr;
    EnnemiLeever* m_pEnnemiLeever;
    EnnemiFactory* m_pEnnemifactory;
    Sprite* m_pBush1 = nullptr;
    Sprite* m_pBush2 = nullptr;
    Sprite* m_pRock1 = nullptr;
    Sprite* m_pRock2 = nullptr;
    int m_playerSpeed = 10;
    int m_currentWave = 0;
    int m_bestScore = 0;
    bool isLeftKeyPressed = false;
    bool isRightKeyPressed = false;
    bool isUpKeyPressed = false;
    bool isDownKeyPressed = false;
    bool isWKeyPressed = false;
    bool isAKeyPressed = false;
    bool isSKeyPressed = false;
    bool isDKeyPressed = false;
    bool hasWAnimationPlayed = false;
    bool hasAAnimationPlayed = false;
    bool hasSAnimationPlayed = false;
    bool hasDAnimationPlayed = false;
    QGraphicsSimpleTextItem* m_pDisplayedInformation;
    QGraphicsSimpleTextItem* m_pDisplayedNumberWaves;
    QGraphicsSimpleTextItem* m_pDisplayedLevelInformation;
    QGraphicsSimpleTextItem* m_pDisplayedtextHeart;
    QGraphicsSimpleTextItem* m_pDisplayedtextBlueRing;
    QGraphicsSimpleTextItem* m_pDisplayedtextTriforce;
    QGraphicsSimpleTextItem* m_pDisplayedtextLeever;
    QGraphicsSimpleTextItem* m_pDisplayedtextLeeverRouge;
    QGraphicsSimpleTextItem* m_pDisplayedtextOctopus;
    QGraphicsSimpleTextItem* m_pDisplayedBestScore;
    Sprite* m_pHeart = nullptr;
    Sprite* m_pBlueRing = nullptr;
    Sprite* m_pTriforce = nullptr;
    Sprite* m_pLeever = nullptr;
    Sprite* m_pLeeverRouge = nullptr;
    Sprite* m_pOctopus = nullptr;
    QList<int> m_pressedKeys;

private slots:

};


#endif // GAMECORE_H
