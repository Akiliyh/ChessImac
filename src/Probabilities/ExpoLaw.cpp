#include "ExpoLaw.hpp"
#include <cmath>
#include <iostream>

// Initialisation du générateur Mersenne Twister avec une graine aléatoire
ExpoLaw::ExpoLaw() : m_generator(m_random())
{}

double ExpoLaw::get_bounded_time(double min_time, double max_time, double lambda)
{
    if (min_time >= max_time || lambda <= 0.0)
    {
        std::cerr << "Erreur : bornes invalides ou lambda <= 0.\n";
        return min_time;
    }

    std::uniform_real_distribution<double> distrib(0.0, 1.0);
    double                                 x     = 0.0;
    double                                 range = max_time - min_time;

    // Méthode de rejet : on tire jusqu'à ce que la valeur x soit dans notre intervalle [0, range]
    do
    {
        double u = 1.0 - distrib(m_generator);
        x        = -std::log(u) / lambda;
    } while (x > range);

    // On ajoute le minimum pour décaler le résultat
    return min_time + x;
}

void ExpoLaw::simulateExpoLaw(int iterations, double lambda)
{
    if (iterations <= 0 || lambda <= 0.0)
    {
        std::cerr
            << "Erreur : le nombre d'itérations et lambda doivent être strictement positifs.\n";
        return;
    }

    std::uniform_real_distribution<double> distrib(0.0, 1.0);
    double                                 sum = 0.0;

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

    std::cout << "\n---> Moyenne des " << iterations << " tirages : " << (sum / iterations) << '\n';
}