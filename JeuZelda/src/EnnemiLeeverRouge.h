#ifndef ENNEMILEEVERROUGE_H
#define ENNEMILEEVERROUGE_H

#include "sprite.h"
#include <QtCore>
#include <QObject>
#include "ennemy.h"

class EnnemiLeeverRouge : public Ennemy
{
public:
    EnnemiLeeverRouge();
    ~EnnemiLeeverRouge() override;
    void tick(long long elapsedTimeInMilliseconds) override;
    void damage();

private:
    static constexpr float LEEVER_ROUGE_SCALE_FACTOR = 5.2;
    static constexpr int LEEVER_ROUGE_RANGE = 120;
    int m_Hp = 1;
};

#endif // ENNEMILEEVERROUGE_H
