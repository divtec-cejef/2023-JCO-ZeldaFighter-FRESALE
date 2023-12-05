#include "ennemy.h"
#include "resources.h"
#include "utilities.h"
#include "gamecore.h"
#include "gamescene.h"

Ennemy::Ennemy(QString imagePath) : Sprite(imagePath)
{
    setData(GameCore::SpriteDataKey::SPRITE_TYPE_KEY, GameCore::ENNEMI);
}
