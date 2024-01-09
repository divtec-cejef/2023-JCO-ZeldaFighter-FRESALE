#ifndef ENNEMIOCTOPUS_H
#define ENNEMIOCTOPUS_H

#include "ennemy.h"
#include "projectile.h"

class EnnemiOctopus : public Ennemy
{
public:
    EnnemiOctopus();

    void tick(long long elapsedTimeInMilliseconds) override;
    void damage() override;
    void attack(QPointF direction);
    void removeProjectile();

private:
    Projectile* m_pProjectil = nullptr;

    static constexpr int OCTOPUS_SCALE_FACTOR = 3.5;
    static constexpr int CHANCE_TO_SPAWN_HEART = 8;
    static constexpr int CHANCE_TO_SPAWN_BLUE_RING = 16;
    static constexpr int CHANCE_TO_SPAWN_TRIFORCE = 50;
};

#endif // ENNEMIOCTOPUS_H
