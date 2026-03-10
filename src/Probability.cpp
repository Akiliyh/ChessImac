#include "Probability.hpp"
#include <cmath> // Pour std::log (logarithme népérien)
#include <iostream>
#include <random>

void Probability::simulateExpoLaw(int iter, double lambda)
{
    // Generate rand()
    static std::random_device              rd;
    static std::mt19937                    gen(rd());
    std::uniform_real_distribution<double> distrib(0.0, 1.0);

    double somme = 0.0; // LIGNE 1 : On crée une variable pour stocker le total avant la boucle

    // Interations
    for (int i{0}; i < iter; i++)
    {
        // 1. Tirage uniforme de U (entre 0 et 1)
        double U = distrib(gen);

        // Sécurité : ln(0) est indéfini, on s'assure que U n'est pas pile 0
        if (U == 0.0)
            U = 0.00001;

        // 2. Inversion de la fonction de répartition : F^-1(U)
        double X = -std::log(U) / lambda;

        // Résultat => temps qui s'est écoulé entre deux évènements
        // Exemple => 1er nombre (3.63729) : Vous ouvrez le magasin. Il s'écoule 3,6 minutes avant
        // que le tout premier client ne franchisse la porte. 2ème nombre (0.0976772) : À peine le
        // premier client entré, le deuxième client arrive presque immédiatement, seulement 0,09
        // minute (environ 5 secondes) plus tard !
        // 11ème nombre (9.57203) : Ici, c'est une grosse période de calme. Il s'est écoulé 9,5
        // minutes sans que personne n'entre.
        std::cout << X << '\n';

        somme += X; // LIGNE 2 : On ajoute la valeur générée au total à chaque tour
    }

    // LIGNE 3 : En dehors de la boucle, on calcule et on affiche la moyenne finale
    std::cout << "\n---> Moyenne des " << iter << " tirages : " << somme / iter << '\n';
}

void Probability::simulateUniformLaw(int iter)
{
    // Generate rand()
    static std::random_device              rd;
    static std::mt19937                    gen(rd());
    std::uniform_real_distribution<double> distrib(0.0, 1.0);

    double somme = 0.0; // LIGNE 1 : On crée une variable pour stocker le total avant la boucle

    // Interations
    for (int i{0}; i < iter; i++)
    {
        // 1. Tirage uniforme de U (entre 0 et 1)
        double U = distrib(gen);

        // 2. Inversion de la fonction de répartition : F^-1(U)
        double X = 1-U;

        std::cout << X << '\n';

        somme += X; // LIGNE 2 : On ajoute la valeur générée au total à chaque tour
    }

    // LIGNE 3 : En dehors de la boucle, on calcule et on affiche la moyenne finale
    std::cout << "\n---> Moyenne des " << iter << " tirages : " << somme / iter << '\n';
}