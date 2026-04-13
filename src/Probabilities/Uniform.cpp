#include "Uniform.hpp"
#include <chrono>
#include <cmath>
#include <iomanip>
#include <iostream>

Uniform::Uniform(int min, int max) : min_val(min), max_val(max)
{
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    mt_engine     = std::mt19937(seed);
    dist          = std::uniform_int_distribution<int>(min_val, max_val);
    lcg_state     = seed;
}

void Uniform::setRange(int min, int max)
{
    min_val = min;
    max_val = max;
    // On met a jour la distribution de la librairie standard
    dist.param(std::uniform_int_distribution<int>::param_type(min_val, max_val));
}

int Uniform::generateWithLibrary()
{
    return dist(mt_engine);
}

int Uniform::generateFromScratch()
{
    lcg_state = (LCG_A * lcg_state + LCG_C) % LCG_M;
    int range = max_val - min_val + 1;
    return min_val + (lcg_state % range);
}

void Uniform::compareMethods(int iterations)
{
    std::map<int, int> results_lib;
    std::map<int, int> results_scratch;
    int                range = max_val - min_val + 1;

    // Initialisation des compteurs a zero
    for (int i = min_val; i <= max_val; i++)
    {
        results_lib[i]     = 0;
        results_scratch[i] = 0;
    }

    // Boucle de simulation sur n iterations
    for (int i = 0; i < iterations; i++)
    {
        results_lib[generateWithLibrary()]++;
        results_scratch[generateFromScratch()]++;
    }

    // En theorie des probabilites, chaque piece doit apparaitre iterations / range fois
    double expected = static_cast<double>(iterations) / range;

    std::cout << "=== Comparaison sur " << iterations << " iterations ===\n";
    std::cout << "Esperance theorique par piece : " << expected << " apparitions\n\n";

    double mae_lib     = 0.0;
    double mae_scratch = 0.0;

    for (int i = min_val; i <= max_val; i++)
    {
        double diff_lib     = std::abs(results_lib[i] - expected);
        double diff_scratch = std::abs(results_scratch[i] - expected);

        mae_lib += diff_lib;
        mae_scratch += diff_scratch;

        std::cout << "Piece ID " << i << " :\n";
        std::cout << "  - Librairie : " << results_lib[i] << " fois (Ecart absolu: " << diff_lib
                  << ")\n";
        std::cout << "  - Scratch   : " << results_scratch[i]
                  << " fois (Ecart absolu: " << diff_scratch << ")\n\n";
    }

    // Calcul de l'erreur absolue moyenne (Mean Absolute Error)
    mae_lib /= range;
    mae_scratch /= range;

    std::cout << "--- Bilan de l'ecart (Erreur Absolue Moyenne) ---\n";
    std::cout << "Plus le score est proche de 0, plus la distribution est parfaite.\n";
    std::cout << "Score Librairie : " << std::fixed << std::setprecision(2) << mae_lib << "\n";
    std::cout << "Score Scratch   : " << std::fixed << std::setprecision(2) << mae_scratch << "\n";
}