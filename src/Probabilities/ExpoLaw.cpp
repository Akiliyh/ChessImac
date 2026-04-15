#include "ExpoLaw.hpp"
#include <cmath>
#include <iostream>


// Initialisation du générateur Mersenne Twister avec une graine aléatoire
ExpoLaw::ExpoLaw() : m_generator(m_random())
{}

void ExpoLaw::simulateExpoLaw(int iterations, double lambda)
{
    // Sécurité : on évite les divisions par zéro ou les boucles infinies/négatives
    if (iterations <= 0 || lambda <= 0.0)
    {
        std::cerr
            << "Erreur : le nombre d'itérations et lambda doivent être strictement positifs.\n";
        return;
    }

    std::uniform_real_distribution<double> distrib(0.0, 1.0);
    double                                 sum = 0.0;

    // Boucle d'itérations
    for (int i = 0; i < iterations; ++i)
    {
        // 1. Tirage uniforme.
        // distrib(m_generator) génère dans [0, 1).
        // 1.0 - distrib(m_generator) donne un résultat dans (0, 1].
        // Cela empêche U d'être strictement égal à 0 et évite le ln(0) indéfini.
        double u = 1.0 - distrib(m_generator);

        // 2. Inversion de la fonction de répartition : F^-1(U)
        double x = -std::log(u) / lambda;

        // Résultat => temps qui s'est écoulé entre deux évènements
        std::cout << x << '\n';

        // Ajout au total
        sum += x;
    }

    // Affichage de la moyenne finale
    std::cout << "\n---> Moyenne des " << iterations << " tirages : " << (sum / iterations) << '\n';
}