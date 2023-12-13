#ifndef ENNEMY_H
#define ENNEMY_H

#include "sprite.h"

class Ennemy : public Sprite
{
public:
    Ennemy(QString imagePath);
    virtual ~Ennemy() {}
    virtual void tick(long long elapsedTimeInMilliseconds) = 0;
    virtual void damage() = 0;
    void CreateCloudOndeath(QPointF pos);
    void ChanceToSpawnItems(QPointF pos, int chanceToSpawnHearth, int chanceToSpawnBlueRing, int ChanceToSpawnTriforce);
    void removeEnnemyFromScene();

    constexpr static int CLOUD_SCALE_FACTOR = 5;

protected:
    int m_Hp;
};

#endif // ENNEMY_H
