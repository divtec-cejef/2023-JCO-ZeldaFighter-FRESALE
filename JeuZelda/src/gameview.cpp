/**
  Déclaration de la classe GameView, qui affiche une scène.

  @author   JCO
  @date     Janvier 2015
 */
#include "gameview.h"

#include <QDebug>
#include <QMouseEvent>

//! Construit une fenêtre de visualisation de la scène de jeu.
//! \param pParent  Widget parent.
GameView::GameView(QWidget* pParent) : QGraphicsView(pParent) {
    init();
}

//! Construit une fenêtre de visualisation de la scène de jeu.
//! \param pScene   Scène affichée par cette vue.
//! \param pParent  Widget parent.
GameView::GameView(QGraphicsScene* pScene, QWidget* pParent) : QGraphicsView(pScene, pParent){
    init();
}

//! Détruit la vue, qui est propriétaire du HUD.
GameView::~GameView() {
    if (m_pHudScene) {
        delete m_pHudScene;
        m_pHudScene = nullptr;
    }
}

//! Enclenche ou déclenche l'ajustage automatique de l'affichage de la scène à
//! la taille de la fenêtre.
//! \param fitToScreenEnabled  Indique si l'ajustage automatique est enclenché (true) ou
//!                            déclanché (false).
void GameView::setFitToScreenEnabled(bool fitToScreenEnabled) {
    m_fitToScreen = fitToScreenEnabled;
}

//! \return un booléen indiquant si l'ajustage automatique de l'affichage est
//!         enclenché (true) ou non (false).
bool GameView::isFitToScreenEnabled() const {
    return m_fitToScreen;
}

//! Enclenche ou déclenche le clipping de la scène.
//! \param clipSceneEnabled  Indique si le clipping est enclenché (true) ou
//!                          déclanché (false).
void GameView::setClipSceneEnabled(bool clipSceneEnabled) {
    m_clipScene = clipSceneEnabled;
}

//! \return un booléen indiquant si le clipping de la scène est enclenché ou non.
bool GameView::isClipSceneEnabled() const {
    return m_clipScene;
}

//! Détermine la scène qui sera affichée comme HUD.
//! GameView prend possession de cette scène et se chargera
//! de la détruire.
//! \param pHudScene Scène à afficher comme HUD.
void GameView::setHudScene(QGraphicsScene* pHudScene) {
    if (pHudScene == m_pHudScene)
        return;

    if (m_pHudScene) {
        delete m_pHudScene;
        m_pHudScene = nullptr;
    }
    m_pHudScene = pHudScene;
}

//! \return la scène utilisée comme HUD.
QGraphicsScene* GameView::hudScene() const {
    return m_pHudScene;
}

//! Met à jour, si nécessaire, la taille d'affichage de la scène.
void GameView::updateSceneDisplaySize() {
    if (scene() && m_fitToScreen) {
        fitInView(sceneRect(), Qt::KeepAspectRatio);
    }
}

//! Gère le redimensionnement de l'affichage.
//! \param pEvent   Evénement de redimensionnement reçu.
void GameView::resizeEvent(QResizeEvent* pEvent) {
    QGraphicsView::resizeEvent(pEvent);
    m_clippingRectUpToDate = false;
    updateSceneDisplaySize();
}

//! Dessine le HUD (s'il existe) au premier plan.
//! Par défaut, la scène du HUD est rendue de sorte qu'elle utilise la surface d'affichage
//! de cette vue (viewport()->rect()). Il est possible de changer ce comportement, par
//! exemple en donnant la surface de la scène de jeu (sceneRect()).
//! Si la scène doit être clippée, dessine en avant-plan des rectangles permettant
//! de cacher les marges de la scène, car il n'y a pas de méthodes propres à Qt le permettant,
//! étant donné que chaque QGraphicsItem est responsable de se dessiner.
//! \param pPainter     Painter à utiliser pour dessiner.
//! \param rRect        Zone à dessiner.
void GameView::drawForeground(QPainter* pPainter, const QRectF& rRect) {
    // Affichage du HUD
    // Pour que le HUD s'affiche en position absolue, indépendamment du
    // viewport, il faut annuler toute transformation du painter, puis les
    // rétablir pour le reste des opérations de dessin.
    if (m_pHudScene) {
        // Ici, il faudrait peut-être tenir compte du flag "fitToScreen" pour
        // ne pas désactiver les transformations s'il est enclenché.
        pPainter->save();
        pPainter->resetTransform();
        //m_pHudScene->render(pPainter, sceneRect()); // dessine le hud sur la surface complète de la scène
        m_pHudScene->render(pPainter, viewport()->rect()); // dessin le hud sur la surface visible
        pPainter->restore();
    }

    if (!m_clipScene)
        return;

    if (!m_clippingRectUpToDate) {
        m_clippingRect[0] = QRectF(rRect.left(), rRect.top(), rRect.width(), sceneRect().top() - rRect.top());
        m_clippingRect[1] = QRectF(rRect.left(), sceneRect().top(), sceneRect().left() - rRect.left(), sceneRect().height());
        m_clippingRect[2] = QRectF(sceneRect().right(), sceneRect().top(), rRect.right() - sceneRect().right(), sceneRect().height());
        m_clippingRect[3] = QRectF(rRect.left(), sceneRect().bottom(), rRect.width(), rRect.bottom() - sceneRect().bottom());
        m_clippingRectUpToDate = true;
    }

    for (int i = 0; i < 4; ++i)
        pPainter->fillRect(m_clippingRect[i], scene()->backgroundBrush());
}

//! Initialise cette affichage.
void GameView::init() {
    m_fitToScreen = false;
    m_clipScene = false;
    m_clippingRectUpToDate = false;

    setFrameShape(QFrame::NoFrame);

    // Pour aligner la scène tout à gauche plutôt qu'au centre.
    //setAlignment(Qt::AlignLeft);

    // Nécessaire de faire un FullViewportUpdate si le HUD est utilisé.
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
}
