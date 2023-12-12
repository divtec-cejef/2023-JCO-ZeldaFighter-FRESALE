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
        PROJECTIL,
        HEARTDROP,
        BLUE_RING
    };

    enum GameMode {
        RUNNING,
        PAUSE,
        ENDED_LOSE
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
    void playerPosition();
    void generateEnemyWave();

    void displayInformation(const QString& rMessage);
    void displayWaves(int waveNumber);
    void clearInformation();

    GameMode m_gameMode = RUNNING;

    static constexpr int SCENE_WIDTH = 1280;
    static constexpr float PLAYER_SCALE_FACTOR = 4;
    static constexpr float DECOR_SCALE_FACTOR = 5;
    static constexpr float HEARTH_DROP_SCALE_FACTOR = 3.8;
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
    QGraphicsSimpleTextItem* m_pDisplayedNbreVagues;
    QList<int> m_pressedKeys;

private slots:

};


#endif // GAMECORE_H
