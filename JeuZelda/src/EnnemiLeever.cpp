#include "EnnemiLeever.h"
#include "resources.h"
#include "utilities.h"
#include "gamecore.h"
#include "gamescene.h"
#include <cstdlib>

EnnemiLeever::EnnemiLeever(): Ennemy(GameFramework::imagesPath() + "JeuZelda/Ennemi1_1.gif")
{
    // Animation de l'ennemi
    addAnimationFrame(GameFramework::imagesPath() + "JeuZelda/Ennemi1_1.gif");
    addAnimationFrame(GameFramework::imagesPath() + "JeuZelda/ennemi1_2.gif");
    startAnimation(100);
    setScale(LEEVER_SCALE_FACTOR);
    m_Hp = 1;
}

EnnemiLeever::~EnnemiLeever() {

}

void EnnemiLeever::tick(long long elapsedTimeInMilliseconds) {
    // Définis une constante pour la fréquence de mouvement souhaitée (une chance sur 3 chaque seconde)
    const int movementFrequency = 3000;  // en millisecondes (1000 ms = 1 seconde)

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
            if (y() - LEEVER_RANGE >= 0) {
                moveBy(0, -LEEVER_RANGE);
            }
            break;
        case 1:
            // Déplace vers le bas
            if (y() + LEEVER_RANGE <= parentScene()->height() - height()) {
                moveBy(0, LEEVER_RANGE);
            }
            break;
        case 2:
            // Déplace vers la gauche
            if (x() - LEEVER_RANGE >= 0) {
                moveBy(-LEEVER_RANGE, 0);
            }
            break;
        case 3:
            // Déplace vers la droite
            if (x() + LEEVER_RANGE <= parentScene()->width() - width()) {
                moveBy(LEEVER_RANGE, 0);
            }
            break;
        }
    }
}

void EnnemiLeever::damage() {
    m_Hp--;
    if(m_Hp <= 0) {
        createCloudOnDeath(pos());
        createItemOnDeath(pos(), CHANCE_TO_SPAWN_HEART, CHANCE_TO_SPAWN_BLUE_RING, CHANCE_TO_SPAWN_TRIFORCE);
        removeEnnemyFromScene();
    }
}
