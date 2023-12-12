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

    static constexpr int OCTOPUS_SCALE_FACTOR = 3.5;

private:
    Projectile* m_pProjectil = nullptr;
};

#endif // ENNEMIOCTOPUS_H
