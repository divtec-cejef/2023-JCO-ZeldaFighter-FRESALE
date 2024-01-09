#include "player.h"
#include "gamecanvas.h"
#include "resources.h"
#include "utilities.h"
#include "gamecore.h"
#include "gamescene.h"
#include "gamecanvas.h"
#include "sprite.h"
#include "projectile.h"

Player::Player(): Sprite(GameFramework::imagesPath() + "JeuZelda/DownLink_1.gif")
{
    //setDebugModeEnabled(true);
    setData(GameCore::SpriteDataKey::SPRITE_TYPE_KEY, GameCore::PLAYER);
}

//! Création des sprites de la vie du joueur (coeur).
void Player::initializeHearts() {
    // Création des sprites de la vie du joueur (coeur).
    for(int i = 0; i < NOMBRES_COEURS; i++) {
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

//! Fonction qui permet de retirer un coeur au joueur.
//! Si le joueur n'a plus de coeur, il meurt.
//! Si le joueur est invincible, il ne peut pas prendre de dégâts.
void Player::damage() {
    // Si le joueur est invincible, il ne peut pas prendre de dégâts, on sort donc de la fonction.
    if (m_invincibleCooldown > 0) {
        return;
    }

    m_invincibleCooldown = 2000;
    // Rend le joueur transparent
    setOpacity(0.5);
    // Retire un coeur au joueur
    parentScene()->removeSpriteFromScene(m_pHearts.last());
    delete m_pHearts.last();
    m_pHearts.remove(m_pHearts.length() - 1);

    // Si le joueur n'a plus de coeur, il meurt.
    if(m_pHearts.isEmpty()) {
        isDead = true;
    }
}

//! Fonction qui permet d'ajouter un coeur au joueur.
void Player::addHeart() {
    // Création du sprite du coeur.
    QPixmap HearthImage = QPixmap(GameFramework::imagesPath() + "JeuZelda/Coeur.png");
    int hearthWidth = HearthImage.width() + 1;

    // Ajout du coeur à la liste des coeurs du joueur.
    auto heart = new Sprite(HearthImage);
    m_pHearts.append(heart);
    // Ajout du coeur à la scène.
    GameScene* parentScene = this->parentScene();
    // Positionnement du coeur.
    heart->setScale(GameCore::DECOR_SCALE_FACTOR);
    heart->setData(GameCore::SpriteDataKey::SPRITE_TYPE_KEY, GameCore::HEART);
    int hearthX = m_pHearts.length() * (hearthWidth + ESPACE_ENTRE_COEURS) - 50;
    parentScene->addSpriteToScene(heart, hearthX, 20);
}

//! Fonction qui permet au joueur d'attaquer.
//! \param direction La direction dans laquelle le joueur attaque.
void Player::attack(QPointF direction) {
    // Si une épée est déjà présente sur la scène, on sort de la fonction.
    if(m_pSword != nullptr) {
        return;
    }
    // Création de l'épée.
    m_pSword = new Projectile(swordSpeed, direction, GameFramework::imagesPath() + "JeuZelda/ZeldaSpriteSword.png", this);
    m_pSword->setScale(SWORD_SCALE_FACTOR);
    m_pSword->setPos(scenePos());
    parentScene()->addSpriteToScene(m_pSword);
}

//! \brief Player::removeSword
//! Supprime l'épée du joueur.
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
