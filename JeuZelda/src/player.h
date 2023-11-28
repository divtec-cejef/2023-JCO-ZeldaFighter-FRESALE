#ifndef PLAYER_H
#define PLAYER_H

#include "sprite.h"
#include <QtCore>
#include <QObject>

class Projectile;

class Player : public Sprite
{
public:
    Player();
    ~Player();
    void initializeHearts();
    void tick(int elapsedMs);
    void damage();
    void addHeart();
    void displayInformation(const QString& rMessage);
    void attack(QPointF direction);
    void removeSword();

    static constexpr qreal SWORD_SCALE_FACTOR = 4.0;
    static constexpr qreal SWORD_VITESSE = 300.0;
private:
    static constexpr int ESPACE_ENTRE_COEURS = 55;

    QList<Sprite*> m_pHearts = {};

    int m_invincibleCooldown = 0;
    Projectile* m_pSword = nullptr;
};

#endif // PLAYER_H
