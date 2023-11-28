#include "projectile.h"
#include "player.h"
#include "gamecanvas.h"
#include "gamecore.h"
#include "resources.h"
#include "utilities.h"
#include "sprite.h"
#include "gamescene.h"
#include <math.h>

Projectile::Projectile(qreal speed, QPointF direction, const QString &rImagePath, Sprite* pOwner, QGraphicsItem* pParent) : Sprite(rImagePath, pParent)
{
    m_speed = speed;
    m_direction = direction;
    m_pOwner = pOwner;
    // Permet d'identifier le projectile comme étant une épée (SWORD).
    setData(GameCore::SPRITE_TYPE_KEY, GameCore::SWORD);
    setDebugModeEnabled(true);
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
        if(Player* player = dynamic_cast<Player*>(m_pOwner)) {
            player->removeSword();
        }
    }
}
