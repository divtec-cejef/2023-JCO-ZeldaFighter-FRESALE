#ifndef ENNEMILEEVER_H
#define ENNEMILEEVER_H

#include "sprite.h"
#include <QtCore>
#include <QObject>

class EnnemiLeever : public Sprite
{
public:
    EnnemiLeever();
    void tick(long long elapsedTimeInMilliseconds);

private:
    static constexpr float LEEVER_SCALE_FACTOR = 4;
};





#endif // ENNEMILEEVER_H
