#include "bernoulli.hpp"
#include <cmath>
#include <iomanip>
#include <iostream>

// Constructor implementation
Bernoulli::Bernoulli(double p) : probability_p(p), std_bernoulli(p), uniform_dist(0.0, 1.0)
{
    std::random_device rd;
    generator = std::mt19937(rd());
}

// Method 1 implementation
bool Bernoulli::generate_std()
{
    return std_bernoulli(generator);
}

// Method 2 implementation
bool Bernoulli::generate_scratch()
{
    double u = uniform_dist(generator);
    return u < probability_p;
}

// Method 3 implementation
void Bernoulli::compare(int n)
{
    int success_std     = 0;
    int success_scratch = 0;

    for (int i = 0; i < n; ++i)
    {
        if (generate_std())
            success_std++;
        if (generate_scratch())
            success_scratch++;
    }

    double p_empirical_std     = static_cast<double>(success_std) / n;
    double p_empirical_scratch = static_cast<double>(success_scratch) / n;

    double error_std     = std::abs(p_empirical_std - probability_p);
    double error_scratch = std::abs(p_empirical_scratch - probability_p);

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "=== Comparaison sur " << n << " iterations (p = " << probability_p << ") ===\n";

    std::cout << "\n1. Methode std::bernoulli_distribution :\n";
    std::cout << "   - Succes obtenus : " << success_std << "\n";
    std::cout << "   - Probabilite empirique : " << p_empirical_std << "\n";
    std::cout << "   - Ecart avec p absolu : " << error_std << "\n";

    std::cout << "\n2. Methode From Scratch :\n";
    std::cout << "   - Succes obtenus : " << success_scratch << "\n";
    std::cout << "   - Probabilite empirique : " << p_empirical_scratch << "\n";
    std::cout << "   - Ecart avec p absolu : " << error_scratch << "\n";

    std::cout << "\nDifference de probabilite empirique entre les deux generateurs : "
              << std::abs(p_empirical_std - p_empirical_scratch) << "\n";

    double theoretical_std_dev = std::sqrt((probability_p * (1.0 - probability_p)) / n);
    std::cout << "   (Rappel : l'ecart-type theorique attendu est d'environ " << theoretical_std_dev
              << ")\n";
    std::cout << "========================================================\n\n";
}