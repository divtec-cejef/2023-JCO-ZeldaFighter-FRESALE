#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "sprite.h"

class Projectile : public Sprite
{
public:
    Projectile(qreal speed, QPointF direction, const QString& rImagePath, Sprite* pOwner, QGraphicsItem* pParent = nullptr);
    void tick(long long elapsedTimeMs);
    void CreateCloudOndeath(QPointF pos);
    int m_nbreEnnemiDeath = 0;

private:
    qreal m_speed;
    QPointF m_direction;
    Sprite* m_pOwner = nullptr;

signals:
    void hasHit(Sprite* collided);
};

#endif // PROJECTILE_H
