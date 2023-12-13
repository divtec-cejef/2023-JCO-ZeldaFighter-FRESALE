#include "projectile.h"
#include "player.h"
#include "gamecanvas.h"
#include "gamecore.h"
#include "resources.h"
#include "utilities.h"
#include "sprite.h"
#include "gamescene.h"
#include "ennemy.h"
#include "ennemioctopus.h"
#include <math.h>

Projectile::Projectile(qreal speed, QPointF direction, const QString &rImagePath, Sprite* pOwner, QGraphicsItem* pParent) : Sprite(rImagePath, pParent)
{
    m_speed = speed;
    m_direction = direction;
    m_pOwner = pOwner;
    // Permet d'identifier le projectile comme étant une épée (SWORD).
    setData(GameCore::SPRITE_TYPE_KEY, GameCore::PROJECTIL);
    // setDebugModeEnabled(true);
    // Permet de centrer l'image du projectile.
    setOffset(sceneBoundingRect().width() / -2.0, sceneBoundingRect().height() / -2.0);
    // Permet de faire tourner l'image du projectile dans la bonne direction.
    setRotation(atan2(m_direction.y(), m_direction.x()) * 180 / M_PI);
}

void Projectile::tick(long long elapsedTimeMs) {
    setPos(pos() + m_direction * m_speed * elapsedTimeMs / 1000.0);
    if(bottom() < 0 ||
        top() > parentScene()->height() ||
        right() < 0 ||
        left() > parentScene()->width()) {
        // Le projectile est sorti de la scène, on le supprime.
        if(Player* player = dynamic_cast<Player*>(m_pOwner)) {
            player->removeSword();
        }
        if(EnnemiOctopus* pEnnemy = dynamic_cast<EnnemiOctopus*>(m_pOwner)) {
            pEnnemy->removeProjectile();
        }
    }

    QList<Sprite*> collisions = parentScene()->collidingSprites(this);
    if(!collisions.isEmpty()) {
        Sprite* pCollisionned = collisions.at(0);
        if(pCollisionned->data(GameCore::SPRITE_TYPE_KEY) == GameCore::ENNEMI && m_pOwner->data(GameCore::SPRITE_TYPE_KEY) == GameCore::PLAYER) {
            // Le projectile a touché un ennemi, on utilise la fonction damage de l'ennemi
            Ennemy* pEnnemy = dynamic_cast<Ennemy*>(pCollisionned);
            pEnnemy->damage();
            if(Player* player = dynamic_cast<Player*>(m_pOwner)) {
                player->removeSword();
            }
        } else if(pCollisionned->data(GameCore::SPRITE_TYPE_KEY) == GameCore::DECOR) {
            // Le projectile a touché un décor, on supprime le projectile.
            if(Player* player = dynamic_cast<Player*>(m_pOwner)) {
                player->removeSword();
            }
        } else if(pCollisionned->data(GameCore::SPRITE_TYPE_KEY) == GameCore::PLAYER && m_pOwner->data(GameCore::SPRITE_TYPE_KEY) == GameCore::ENNEMI) {
            // Le projectile a touché le joueur, on utilise la fonction damage du joueur.
            Player* pPlayer = dynamic_cast<Player*>(pCollisionned);
            pPlayer->damage();
            if(EnnemiOctopus* pEnnemy = dynamic_cast<EnnemiOctopus*>(m_pOwner)) {
                pEnnemy->removeProjectile();
            }
            // si un projectil du joueur touche le projectil d'un ennemi, on supprime le projectil de l'ennemi
        } else if(pCollisionned->data(GameCore::SPRITE_TYPE_KEY) == GameCore::PROJECTIL) {
            if(EnnemiOctopus* pEnnemy = dynamic_cast<EnnemiOctopus*>(m_pOwner)) {
                pEnnemy->removeProjectile();
            }
        }
    }
}
