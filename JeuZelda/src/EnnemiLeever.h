#ifndef ENNEMILEEVER_H
#define ENNEMILEEVER_H

#include "sprite.h"
#include <QtCore>
#include <QObject>
#include "ennemy.h"

class EnnemiLeever : public Ennemy
{
public:
    EnnemiLeever();
    ~EnnemiLeever() override;
    void tick(long long elapsedTimeInMilliseconds) override;
    void damage();

private:
    static constexpr float LEEVER_SCALE_FACTOR = 4;
    static constexpr int LEEVER_RANGE = 80;
    static constexpr int CHANCE_TO_SPAWN_HEART = 9;
    static constexpr int CHANCE_TO_SPAWN_BLUE_RING = 18;
    static constexpr int CHANCE_TO_SPAWN_TRIFORCE = 50;
    int m_Hp = 1;
};





#endif // ENNEMILEEVER_H
