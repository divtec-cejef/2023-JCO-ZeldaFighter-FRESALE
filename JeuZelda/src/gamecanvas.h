/**
  \file
  \brief    Déclaration de la classe GameCanvas.
  \author   JCO
  \date     janvier 2015
*/
#ifndef GAMECANVAS_H
#define GAMECANVAS_H

#include <QObject>
#include <QPointer>
#include <QTime>
#include <QTimer>
#include <QElapsedTimer>

class GameCore;
class GameScene;
class GameView;
class QGraphicsScene;
class QGraphicsSceneMouseEvent;
class QGraphicsTextItem;
class QKeyEvent;

//! \brief Classe de gestion des scènes du jeu et de sa cadence.
//!
//! GameCanvas prend en charge les différentes scènes constituant le jeu.
//! Il est possible, au sein de GameCore, de créer et gérer plusieurs scènes (des instances de GameScene). Toutefois, une
//! seule scène ne peut être affichée à la fois.
//! Pour créer une scène, il faut utiliser la méthode createScene() de cette classe.
//!
//! La méthode setCurrentScene() permet d'indiquer quelle scène doit être affichée.
//! La méthode currentScene() retourne la scène qui est actuellement affichée.
//!
//! Par défaut, aucune scène n'est affichée. La méthode setCurrentScene() doit être au moins appelée une fois par GameCore.
//!
//! GameCanvas peut également utiliser une scène comme HUD (Heads-up Display), afin d'afficher
//! des informations sur l'état du jeu sans que celles-ci fassent partie de la zone de jeu.
//! La méthode setHudScene() prend en paramètre une instance de QGraphicsScene ou de GameScene
//! et l'utilise comme HUD.
//!
//! Cette classe implémente également le mécanisme de cadence du jeu (le tick).
//!
//! Pour démarrer le tick, utiliser la commande startTick(). Dès que le tick est démarré, la méthode GameCore::tick() est
//! appelée régulièrement, toutes les 20 millisecondes par défaut.
//!
//! Elle se charge alors d'appeler la méthode GameCore::tick() et GameScene::tick() de façon
//! à ce que ces classes puissent réagir à la cadence.
//!
//! Pour stopper le tick, utiliser la commande stopTick().
//!
//! GameCanvas permet également d'enclencher le suivi des déplacements de la souris (startMouseTracking() et de
//! le stopper (stopMouseTracking()).
//!
//! Si GameCanvas émet le signal requestToCloseApp(), cela provoque la fermeture de l'application.
class GameCanvas : public QObject
{
    Q_OBJECT
public:
    enum { KEEP_PREVIOUS_TICK_INTERVAL = -1  };

    explicit GameCanvas(GameView* pView, QObject* pParent = nullptr);
    ~GameCanvas() override;


    GameScene* createScene();
    GameScene* createScene(const QRectF& rSceneRect);
    GameScene* createScene(qreal x, qreal y, qreal width, qreal height);

    void setCurrentScene(GameScene* pScene);
    GameScene* currentScene() const;

    void setHudScene(QGraphicsScene* pHudScene);
    QGraphicsScene* hudScene() const;

    void startTick(int tickInterval = KEEP_PREVIOUS_TICK_INTERVAL);
    void stopTick();
    bool isTicking() const;

    void startMouseTracking();
    void stopMouseTracking();
    QPointF currentMousePosition() const;

    GameView* gameView() const { return m_pView; }

signals:
    void requestToCloseApp();

public slots:

protected:
    bool eventFilter(QObject* pObject, QEvent* pEvent) override;

private:
    void initDetailedInfos();

    void keyPressed(QKeyEvent* pKeyEvent);
    void keyReleased(QKeyEvent* pKeyEvent);

    void mouseMoved(QGraphicsSceneMouseEvent* pMouseEvent);
    void mouseButtonPressed(QGraphicsSceneMouseEvent* pMouseEvent);
    void mouseButtonReleased(QGraphicsSceneMouseEvent* pMouseEvent);

    GameView* m_pView;
    GameCore* m_pGameCore;
    QPointer<QGraphicsTextItem> m_pDetailedInfosItem; // Smart Pointer pour qu'il soit mis à zéro au cas où l'item est effacé par GameScene::clear()

    bool m_keepTicking;
    int m_tickInterval;

    QElapsedTimer m_lastUpdateTime;
    QTimer m_tickTimer;

#ifdef QT_DEBUG
    void resetStatistics();

    int m_tickCount;
    int m_totalElapsedTime;
    int m_maxTickDuration;
    int m_minTickDuration;
    int m_statsTrigger;
#endif

private slots:
    void onInit();
    void onTick();

};

#endif // GAMECANVAS_H
