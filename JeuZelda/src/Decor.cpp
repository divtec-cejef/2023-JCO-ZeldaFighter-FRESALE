#include "Decor.h"
#include "resources.h"
#include "utilities.h"
#include "gamecore.h"
#include "gamescene.h"

Decor::Decor(QString imagePath, int posX, int posY) : Sprite(imagePath)
{
    // Création du sprite "Decor" (arbre, rocher, ect...).
    new Sprite(GameFramework::imagesPath() + imagePath);
    setPos(posX, posY);
    setScale(GameCore::DECOR_SCALE_FACTOR);
    setData(GameCore::SPRITE_TYPE_KEY, GameCore::SpriteType::DECOR);
}
