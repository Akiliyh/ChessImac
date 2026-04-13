#include "WeibullEvolution.hpp"
#include <cmath>
#include <iomanip>
#include <iostream>
#include <random>


// Constructeur
WeibullEvolution::WeibullEvolution(double shapeParam, double scaleParam)
{
    shape = shapeParam;
    scale = scaleParam;
}

// Methode 1: Utilisation de std::weibull_distribution (Empirique)
double WeibullEvolution::getProbabilityLibrary(int moves, int samples)
{
    if (moves <= 0)
        return 0.0;

    // Initialisation du generateur de nombres aleatoires
    std::random_device                rd;
    std::mt19937                      gen(rd());
    std::weibull_distribution<double> weibull_dist(shape, scale);

    int evolvedCount = 0;

    // On simule "samples" pieces
    for (int i = 0; i < samples; ++i)
    {
        // Si le seuil d'evolution genere est inferieur ou egal au nombre de cases parcourues,
        // alors la piece a evolue
        if (weibull_dist(gen) <= static_cast<double>(moves))
        {
            evolvedCount++;
        }
    }

    // Retourne la frequence d'evolution (probabilite empirique)
    return static_cast<double>(evolvedCount) / samples;
}

// Methode 2: From scratch en utilisant la fonction de repartition (CDF)
double WeibullEvolution::getProbabilityScratch(int moves)
{
    if (moves <= 0)
        return 0.0;

    // Formule mathematique exacte : 1 - exp(-(x/lambda)^k)
    double x = static_cast<double>(moves);
    return 1.0 - std::exp(-std::pow(x / scale, shape));
}

// Methode 3: Comparaison des deux methodes
void WeibullEvolution::compareMethods(int maxMoves, int samples)
{
    std::cout << "\nComparaison des methodes Weibull (Shape: " << shape << ", Scale: " << scale
              << ")\n";
    std::cout << "------------------------------------------------------------\n";
    std::cout << "Cases | Scratch (Exact) | Library (Monte Carlo) | Ecart\n";
    std::cout << "------------------------------------------------------------\n";

    for (int moves = 1; moves <= maxMoves; ++moves)
    {
        double probScratch = getProbabilityScratch(moves);
        double probLib     = getProbabilityLibrary(moves, samples);
        double diff        = std::abs(probScratch - probLib);

        std::cout << std::setw(5) << moves << " | " << std::fixed << std::setprecision(6)
                  << probScratch << "        | " << probLib << "               | " << diff << "\n";
    }
    std::cout << "------------------------------------------------------------\n";
}