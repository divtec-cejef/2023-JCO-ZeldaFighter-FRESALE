/**
  \file
  \brief    Déclaration de la classe SpriteTickHandler.
  \author   JCO
  \date     octobre 2022
*/
#ifndef SPRITETICKHANDLER_H
#define SPRITETICKHANDLER_H

class GameScene;
class Sprite;

//! \brief Classe abstraite qui représente un gestionnaire de tick pour un sprite.
//!
//! Cette classe a pour but d'être spécialisée afin de donner à un sprite un comportement
//! à effectuer lors de chaque tick émis par la cadence.
//!
//! Lorsqu'un gestionnaire est affecté à un sprite (Sprite::setTickHandler), la
//! fonction init() du gestionnaire est appelée une unique fois, puis, lorsque
//! la cadence démarre, la fonction tick() du gestionnaire est automatiquement appelée.
//!
//! Depuis le gestionnaire, il est possible d'accéder au Sprite en question avec
//! l'attribut m_pParentSprite.
//!
class SpriteTickHandler
{
public:
    SpriteTickHandler(Sprite* pParentSprite = nullptr);
    virtual ~SpriteTickHandler();

public:
    virtual void setParentSprite(Sprite* pParentSprite);

    virtual void init() {}
    virtual void tick(long long elapsedTimeInMilliseconds) = 0;

protected:
    GameScene* parentScene() const;

    Sprite* m_pParentSprite;
};

#endif // SPRITETICKHANDLER_H
