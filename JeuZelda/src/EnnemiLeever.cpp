#include "EnnemiLeever.h"
#include "resources.h"
#include "utilities.h"
#include "gamecore.h"
#include "gamescene.h"
#include <cstdlib>

EnnemiLeever::EnnemiLeever(): Sprite(GameFramework::imagesPath() + "JeuZelda/Ennemi1_1.gif")
{
    // Animation de l'ennemi
    addAnimationFrame(GameFramework::imagesPath() + "JeuZelda/Ennemi1_1.gif");
    addAnimationFrame(GameFramework::imagesPath() + "JeuZelda/ennemi1_2.gif");
    startAnimation(100);

    setScale(LEEVER_SCALE_FACTOR);
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
        int randomDirection = std::rand() % 4;

        // En fonction du nombre aléatoire, déplace l'ennemi dans une direction spécifique
        switch (randomDirection) {
        case 0:
            // Déplace vers le haut
            setY(y() - 80);
            break;
        case 1:
            // Déplace vers le bas
            setY(y() + 80);
            break;
        case 2:
            // Déplace vers la gauche
            setX(x() - 80);
            break;
        case 3:
            // Déplace vers la droite
            setX(x() + 80);
            break;
        }
    }
}
