#include "player.h"
#include "gamecanvas.h"
#include "resources.h"
#include "utilities.h"
#include "gamecore.h"
#include "gamescene.h"
#include "gamescene.h"
#include "gamecanvas.h"
#include "resources.h"
#include "utilities.h"
#include "sprite.h"
#include "projectile.h"

Player::Player(): Sprite(GameFramework::imagesPath() + "JeuZelda/DownLink_1.gif")
{
    setDebugModeEnabled(true);
}

void Player::initializeHearts() {
    // Création des sprites de la vie du joueur (coeur).
    for(int i = 0; i < 3; i++) {
        addHeart();
    }
}

void Player::tick(int elapsedMs) {
    if(m_invincibleCooldown > 0)
        m_invincibleCooldown -= elapsedMs;
    else
        setOpacity(1.0);  // Rétablir l'opacité normale

    if(m_pSword != nullptr) {
        m_pSword->tick(elapsedMs);
    }
}

void Player::damage() {
    if (m_invincibleCooldown > 0)
        return;

    m_invincibleCooldown = 2000;
    setOpacity(0.5);
    parentScene()->removeSpriteFromScene(m_pHearts.last());
    delete m_pHearts.last();
    m_pHearts.remove(m_pHearts.length() - 1);

    if(m_pHearts.length() < 1)
        return;
}

void Player::addHeart() {
    QPixmap CoeurImage = QPixmap(GameFramework::imagesPath() + "JeuZelda/Coeur.png");
    int coeurWidth = CoeurImage.width() + 1;

    auto heart = new Sprite(CoeurImage);
    m_pHearts.append(heart);
    GameScene* parentScene = this->parentScene();
    heart->setScale(GameCore::DECOR_SCALE_FACTOR);
    heart->setData(GameCore::SpriteDataKey::SPRITE_TYPE_KEY, GameCore::HEART);
    int coeurX = 60 + m_pHearts.length() * (coeurWidth + ESPACE_ENTRE_COEURS);
    parentScene->addSpriteToScene(heart, coeurX, 20);
}

void Player::attack(QPointF direction) {
    if(m_pSword != nullptr)
        return;
    m_pSword = new Projectile(SWORD_VITESSE, direction, GameFramework::imagesPath() + "JeuZelda/ZeldaSpriteSword.png", this);
    m_pSword->setScale(SWORD_SCALE_FACTOR);
    m_pSword->setPos(scenePos());
    parentScene()->addSpriteToScene(m_pSword);
}

void Player::removeSword() {
    parentScene()->removeSpriteFromScene(m_pSword);
    delete m_pSword;
    m_pSword = nullptr;
}

Player::~Player() {
    for(auto heart: m_pHearts) {
        parentScene()->removeSpriteFromScene(heart);
    }
}
