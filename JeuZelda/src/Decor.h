#ifndef DECOR_H
#define DECOR_H

#include "sprite.h"

class Decor : public Sprite
{
public:
    Decor(QString imagePath, int posX, int posY);
};

#endif // DECOR_H
