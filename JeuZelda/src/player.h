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
    void updatePlayer();
    void blinkRed();
    bool isDead = false;
    float swordVitesse = 550.0;
    Projectile* m_pSword = nullptr;

    int m_invincibleCooldown = 0;
    QList<Sprite*> m_pHearts = {};

    static constexpr float SWORD_SCALE_FACTOR = 4.0;

private:
    static constexpr int ESPACE_ENTRE_COEURS = 60;
    static constexpr int NOMBRES_COEURS = 3;


};

#endif // PLAYER_H
