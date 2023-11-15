#ifndef PLAYER_H
#define PLAYER_H

#include "sprite.h"
#include <QtCore>
#include <QObject>

class Player : public Sprite
{
public:
    Player();
    ~Player();
    void initializeHearts();
    void tick(int elapsedMs);
    void damage();
    void addHeart();

private:
    static constexpr int ESPACE_ENTRE_COEURS = 55;

    QList<Sprite*> m_pHearts = {};

    int m_invincibleCooldown = 0;
};

#endif // PLAYER_H
