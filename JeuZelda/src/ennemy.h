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


protected:
    int m_Hp;
};

#endif // ENNEMY_H
