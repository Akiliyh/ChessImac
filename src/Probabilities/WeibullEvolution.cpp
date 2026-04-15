#include "WeibullEvolution.hpp"
#include <cmath>
#include <iomanip>
#include <iostream>
#include <random>

// Implementation using std::weibull_distribution
double WeibullEvolution::get_probability_std(int moves, int samples)
{
    if (moves <= 0)
        return 0.0;

    std::random_device                rd;
    std::mt19937                      gen(rd());
    std::weibull_distribution<double> weibull_dist(m_shape, m_scale);

    int evolvedCount = 0;

    for (int i = 0; i < samples; ++i)
    {
        if (weibull_dist(gen) <= static_cast<double>(moves))
        {
            evolvedCount++;
        }
    }

    // (probabilite empirique)
    return static_cast<double>(evolvedCount) / samples;
}

// Implementation from scratch la fonction de repartition (CDF)
double WeibullEvolution::get_probability_scratch(int moves) const
{
    if (moves <= 0)
        return 0.0;

    // 1 - exp(-(x/lambda)^k)
    auto x = static_cast<double>(moves);
    return 1.0 - std::exp(-std::pow(x / m_scale, m_shape));
}

void WeibullEvolution::compare_methods(int max_moves, int samples)
{
    std::cout << "\nComparaison des methodes Weibull (Shape: " << m_shape << ", Scale: " << m_scale
              << ")\n";
    std::cout << "------------------------------------------------------------\n";
    std::cout << "Cases | Scratch (Exact) | Library (Monte Carlo) | Ecart\n";
    std::cout << "------------------------------------------------------------\n";

    for (int moves = 1; moves <= max_moves; ++moves)
    {
        double probScratch = get_probability_scratch(moves);
        double probLib     = get_probability_std(moves, samples);
        double diff        = std::abs(probScratch - probLib);

        std::cout << std::setw(5) << moves << " | " << std::fixed << std::setprecision(6)
                  << probScratch << "        | " << probLib << "               | " << diff << "\n";
    }
    std::cout << "------------------------------------------------------------\n";
}