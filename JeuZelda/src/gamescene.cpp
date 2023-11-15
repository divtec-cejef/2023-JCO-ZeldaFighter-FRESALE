/**
  \file
  \brief    Définition de la classe GameScene.
  \author   JCO
  \date     janvier 2014
*/
#include "gamescene.h"

#include <cstdlib>
#include <QApplication>
#include <QBrush>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QKeyEvent>
#include <QPainter>
#include <QPen>

#include "gamecore.h"
#include "resources.h"
#include "sprite.h"

//! Construit la scène de jeu avec une taille par défaut et un fond noir.
//! \param pParent  Objet propriétaire de cette scène.
GameScene::GameScene(QObject* pParent) : QGraphicsScene(pParent) {
    init();
}

//! Construit la scène de jeu avec la taille donnée et un fond noir.
//! \param rSceneRect   Taille de la scène.
//! \param pParent      Objet propriétaire de cette scène.
GameScene::GameScene(const QRectF& rSceneRect, QObject* pParent) : QGraphicsScene(rSceneRect, pParent) {
    init();
}

//! Construit la scène de jeu avec la taille donnée et un fond noir.
//! \param X        Position X de la scène.
//! \param Y        Position Y de la scène.
//! \param width    Largeur de la scène.
//! \param height   Hauteur de la scène.
//! \param pParent  Objet propriétaire de cette scène.
GameScene::GameScene(qreal X, qreal Y, qreal width, qreal height, QObject* pParent) : QGraphicsScene(X, Y, width, height, pParent) {
    init();
}

//! Destruction de la scène.
GameScene::~GameScene()  {
    // Plutôt que de laisser QGraphicsScene détruire tous les sprites dont elle
    // est propriétaire, on retire manuellement chaque sprite afin que le code
    // puisse faire les éventuelles étapes de nettoyages correctement.
    QList<Sprite*> sprites = this->sprites();
    for(Sprite* pSprite : sprites) {
        removeSpriteFromScene(pSprite);
        delete pSprite;
    }
    sprites.clear();

    delete m_pBackgroundImage;
    m_pBackgroundImage = nullptr;
}

//! Ajoute le sprite à la scène.
//! La scène prend possession du sprite et se chargera de l'effacer.
//! \param pSprite Pointeur sur le sprite à ajouter à la scène.
void GameScene::addSpriteToScene(Sprite* pSprite)
{
    Q_ASSERT(pSprite != nullptr);

    this->addItem(pSprite);
    pSprite->setParentScene(this);

    connect(pSprite, &Sprite::spriteDestroyed, this, &GameScene::onSpriteDestroyed);
    emit spriteAddedToScene(pSprite);
}

//! Ajoute le sprite à la scène, à la position donnée.
//! La scène prend possession du sprite et se chargera de l'effacer.
//! \param pSprite Pointeur sur le sprite à ajouter à la scène.
//! \param posX    Position X du sprite
//! \param posY    Position Y du sprite
void GameScene::addSpriteToScene(Sprite* pSprite, double posX, double posY)
{
    addSpriteToScene(pSprite);
    pSprite->setPos(posX, posY);
}

//! Retire le sprite de la scène.
//! La scène n'est plus propriétaire du sprite et ne se chargera pas de l'effacer.
//! \param pSprite Pointeur sur le sprite à enlever de la scène.
void GameScene::removeSpriteFromScene(Sprite* pSprite)
{
    removeItem(pSprite);

    disconnect(pSprite, &Sprite::spriteDestroyed, this, &GameScene::onSpriteDestroyed);

    m_registeredForTickSpriteList.removeAll(pSprite);

    emit spriteRemovedFromScene(pSprite);
}

//! Construit la liste de tous les sprites en collision avec le sprite donné en
//! paramètre.
//! Si la scène contient de nombreux sprites, cette méthode peut prendre du temps.
//! \param pSprite Sprite pour lequel les collisions doivent être vérifiées.
//! \return une liste de sprites en collision. Si aucun autre sprite ne collisionne
//! le sprite donné, la liste retournée est vide.
QList<Sprite*> GameScene::collidingSprites(const Sprite* pSprite) const {
    QList<Sprite*> spriteList;
    const auto collidingItems = pSprite->collidingItems();
    for(QGraphicsItem* pItem : collidingItems) {
        if (pItem->type() == Sprite::SpriteItemType)
            spriteList << static_cast<Sprite*>(pItem);
    }
    return spriteList;
}

//! Construit la liste de tous les sprites en collision avec le rectangle donné
//! en paramètre.
//! Si la scène contient de nombreux sprites, cette méthode peut prendre du temps.
//! \param rRect Rectangle avec lequel il faut tester les collisions.
//! \return une liste de sprites en collision.
QList<Sprite*> GameScene::collidingSprites(const QRectF &rRect) const  {
    QList<Sprite*> collidingSpriteList;
    for(Sprite* pSprite : sprites())  {
        QRectF globalBBox = pSprite->globalBoundingRect();
        if (globalBBox.intersects(rRect)) {
            collidingSpriteList << pSprite;
        }
    }
    return collidingSpriteList;
}

//! Construit la liste de tous les sprites en collision avec la forme donnée
//! en paramètre.
//! Si la scène contient de nombreux sprites, cette méthode peut prendre du temps.
//! \param rShape Forme avec laquelle il faut tester les collisions.
//! \return une liste de sprites en collision.
QList<Sprite*> GameScene::collidingSprites(const QPainterPath& rShape) const {
    QList<Sprite*> collidingSpriteList;
    auto spriteList = collidingSprites(rShape.boundingRect());
    for(Sprite* pSprite : spriteList)  {
        if (pSprite->globalShape().intersects(rShape)) {
            collidingSpriteList << pSprite;
        }
    }
    return collidingSpriteList;
}

//!
//! \return la liste des sprites de cette scène (y compris ceux qui ne sont pas visibles).
//!
QList<Sprite*> GameScene::sprites() const  {
    QList<Sprite*> spriteList;
    auto allItems = this->items();
    for(QGraphicsItem* pItem : allItems) {
        if (pItem->type() == Sprite::SpriteItemType)
            spriteList << static_cast<Sprite*>(pItem);
    }
    return spriteList;
}

//! Récupère le sprite visible le plus en avant se trouvant à la position donnée.
//! \return un pointeur sur le sprite trouvé, ou null si aucun sprite ne se trouve à cette position.
Sprite* GameScene::spriteAt(const QPointF& rPosition) const {
    QGraphicsItem* pTopMostVisibleItem = this->itemAt(rPosition, QTransform());
    if (pTopMostVisibleItem && pTopMostVisibleItem->type() == Sprite::SpriteItemType)
        return static_cast<Sprite*>(pTopMostVisibleItem);

    return nullptr;
}

//!
//! Affiche un texte sur la scène.
//! Si ce texte n'est pas détruit manuellement, il le sera automatiquement
//! lorsque la scène sera détruite.
//! \param initialPosition  Position initiale du texte.
//! \param rText            Texte à afficher.
//! \param size             Taille (en pixels) du texte.
//! \param color            Couleur du texte.
//! \return un pointeur sur l'élément graphique textuel.
//!
QGraphicsSimpleTextItem* GameScene::createText(QPointF initialPosition, const QString& rText, int size, QColor color) {
    QGraphicsSimpleTextItem* pText = this->addSimpleText(rText);
    pText->setPos(initialPosition);
    QFont textFont = pText->font();
    textFont.setPixelSize(size);
    pText->setFont(textFont);
    pText->setBrush(QBrush(color));
    return pText;
}

//! Défini l'image de fond à utiliser pour cette scène.
void GameScene::setBackgroundImage(const QImage& rImage)  {
    if (m_pBackgroundImage)
        delete m_pBackgroundImage;
    m_pBackgroundImage = new QImage(rImage);
}

//! Défini la couleur de fond de cette scène.
void GameScene::setBackgroundColor(QColor color) {
    if (m_pBackgroundImage) {
        delete m_pBackgroundImage;
        m_pBackgroundImage = nullptr;
    }

    this->setBackgroundBrush(QBrush(color));
}

//! Change la largeur de la scène.
//! \param sceneWidth   Largeur de la scène en pixels.
void GameScene::setWidth(int sceneWidth)  {
    setSceneRect(0,0, sceneWidth, height());
}

//! Change la hauteur de la scène.
//! \param sceneHeight   Hauteur de la scène, en pixels.
void GameScene::setHeight(int sceneHeight)  {
    setSceneRect(0,0, width(), sceneHeight);
}

//! Le sprite donné sera informé du tick.
//! \param pSprite Sprite qui s'enregistre pour le tick.
void GameScene::registerSpriteForTick(Sprite* pSprite) {
    m_registeredForTickSpriteList.append(pSprite);
}

//! Le sprite donné se va plus être informé du tick.
//! \param pSprite Sprite qui démissionne du tick.
void GameScene::unregisterSpriteFromTick(Sprite* pSprite) {
    m_registeredForTickSpriteList.removeAll(pSprite);
}

//! Indique si le sprite donné est abonné au tick.
//! \param pSprite Sprite à vérifier.
//! \return un booléen à vrai si le sprite donné est abonné au tick.
bool GameScene::isRegisteredForTick(const Sprite* pSprite) const
{
    return m_registeredForTickSpriteList.contains(pSprite);
}

//! Vérifie si la position donnée fait partie de la scène.
//! \param rPosition Position à vérifier.
//! \return un booléen à vrai si la position fait partie de la scène, sinon
//! faux.
bool GameScene::isInsideScene(const QPointF& rPosition) const {
    return sceneRect().contains(rPosition);
}

//! Vérifie si le rectangle donné fait complètement partie de la scène.
//! Attention : si le rectangle a une largeur/hauteur de zéro, il est considéré
//! par Qt (QRectF::contains) comment invalide et non-contenu dans la scène, ce
//! qui à mon avis est un bug (mais ce n'est pas l'avis de Qt : https://bugreports.qt.io/browse/QTBUG-30456)
//! \param rRect Rectangle à vérifier
//! \return un booléen à vrai si le rectangle fait partie de la scène, sinon
//! faux.
bool GameScene::isInsideScene(const QRectF& rRect) const {
    return sceneRect().contains(rRect);
}

//! Centre la vue GameView sur le sprite donné.
//! \param pSprite Sprite sur lequel la vue doit être centrée.
void GameScene::centerViewOn(const Sprite* pSprite) {
    Q_ASSERT(!views().isEmpty());

    views().at(0)->centerOn(pSprite);
}

//! Centre la vue GameView sur la position donnée.
//! \param pos Position sur laquelle la vue doit être centrée.
void GameScene::centerViewOn(QPointF pos) {
    Q_ASSERT(!views().isEmpty());

    views().at(0)->centerOn(pos);
}

//! Cadence.
//! \param elapsedTimeInMilliseconds  Temps écoulé depuis le tick précédent.
void GameScene::tick(long long elapsedTimeInMilliseconds) {
    auto spriteListCopy = m_registeredForTickSpriteList; // On travaille sur une copie au cas où
                                        // la liste originale serait modifiée
                                        // lors de l'appel de tick auprès d'un sprite.
    for(Sprite* pSprite : spriteListCopy) {
        pSprite->tick(elapsedTimeInMilliseconds);
    }
}

//! Dessine le fond d'écran de la scène.
//! Si une image à été définie avec setBackgroundImage(), celle-ci est affichée.
//! Une autre méthode permet de définir une image de fond :
//! QGraphicsScene::setBackgroundBrush(QBrush(QPixmap(...))).
//! Cette deuxième méthode affiche cependant l'image comme un motif de tuile.
//! \see setBackgroundImage()
void GameScene::drawBackground(QPainter* pPainter, const QRectF& rRect)  {
    QGraphicsScene::drawBackground(pPainter, rRect);
    if (m_pBackgroundImage)
        pPainter->drawImage(0,0, *m_pBackgroundImage);
}

//! Initialise la scène
void GameScene::init() {
    m_pBackgroundImage = nullptr;

    this->setBackgroundBrush(QBrush(Qt::black));
    //setBackgroundImage(QImage(GameFramework::imagesPath("demo/landscape_background.jpg"));
    //this->setBackgroundBrush(QBrush(Qt::white)); // fond blanc

    // Eventuellement peut améliorer les performances lorsqu'il y a beaucoup d'animations.
    // setItemIndexMethod(QGraphicsScene::NoIndex);

}

//! Retire de la liste des sprite le sprite qui va être détruit.
void GameScene::onSpriteDestroyed(Sprite* pSprite) {
    m_registeredForTickSpriteList.removeAll(pSprite);
}
