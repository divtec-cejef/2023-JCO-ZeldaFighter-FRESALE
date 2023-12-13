#include "EnnemiLeeverRouge.h"
#include "resources.h"
#include "utilities.h"
#include "gamecore.h"
#include "gamescene.h"
#include <cstdlib>

EnnemiLeeverRouge::EnnemiLeeverRouge(): Ennemy(GameFramework::imagesPath() + "JeuZelda/Ennemi2_1.gif")
{
    // Animation de l'ennemi
    addAnimationFrame(GameFramework::imagesPath() + "JeuZelda/Ennemi2_1.gif");
    addAnimationFrame(GameFramework::imagesPath() + "JeuZelda/ennemi2_2.gif");
    startAnimation(100);
    setScale(LEEVER_ROUGE_SCALE_FACTOR);
    m_Hp = 2;
}

EnnemiLeeverRouge::~EnnemiLeeverRouge() {

}

void EnnemiLeeverRouge::tick(long long elapsedTimeInMilliseconds) {
    // Définis une constante pour la fréquence de mouvement souhaitée (une chance sur 3 chaque seconde)
    const int movementFrequency = 1500;  // en millisecondes (1000 ms = 1 seconde)

    // Utilise un compteur pour suivre le temps écoulé
    static long long timeCounter = 0;

    // Ajoute le temps écoulé depuis la dernière mise à jour
    timeCounter += elapsedTimeInMilliseconds;

    // Vérifie si le temps écoulé dépasse la fréquence de mouvement
    if (timeCounter >= movementFrequency) {
        // Réinitialise le compteur
        timeCounter = 0;

        // Génére un nombre aléatoire entre 0 et 2 inclus
        int randomDirection = QRandomGenerator::global()->bounded(0, 4);

        // En fonction du nombre aléatoire, déplace l'ennemi dans une direction spécifique
        switch (randomDirection) {
        case 0:
            // Déplace vers le haut
            if (y() - LEEVER_ROUGE_RANGE >= 0) {
                setY(y() - LEEVER_ROUGE_RANGE);
            }
            break;
        case 1:
            // Déplace vers le bas
            if (y() + LEEVER_ROUGE_RANGE <= parentScene()->height() - height()) {
                setY(y() + LEEVER_ROUGE_RANGE);
            }
            break;
        case 2:
            // Déplace vers la gauche
            if (x() - LEEVER_ROUGE_RANGE >= 0) {
                setX(x() - LEEVER_ROUGE_RANGE);
            }
            break;
        case 3:
            // Déplace vers la droite
            if (x() + LEEVER_ROUGE_RANGE <= parentScene()->width() - width()) {
                setX(x() + LEEVER_ROUGE_RANGE);
            }
            break;
        }
    }
}


void EnnemiLeeverRouge::damage() {
    m_Hp--;
    if(m_Hp <= 0) {
        CreateCloudOndeath(pos());
        ChanceToSpawnItems(pos(), 7, 14, 50);
        removeEnnemyFromScene();
    } else {
        // Change la couleur de l'ennemi pour indiquer qu'il a été touché.
        setPixmap(QPixmap(GameFramework::imagesPath() + "JeuZelda/Ennemi1_2.gif"));
        // Démarre un minuteur qui permet de revenir à la couleur initiale après 100 ms.
        QTimer::singleShot(100, this, [this]() {
            setPixmap(QPixmap(GameFramework::imagesPath() + "JeuZelda/Ennemi1_1.gif"));
        });
    }
}
