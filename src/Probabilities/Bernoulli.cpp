#include "bernoulli.hpp"
#include <cmath>
#include <iomanip>
#include <iostream>

Bernoulli::Bernoulli(double p) : m_probability_var(p), m_std_bernoulli(p), m_uniform_dist(0.0, 1.0)
{
    std::random_device rd;
    m_generator = std::mt19937(rd());
}

// Implementation using std
bool Bernoulli::generate_std()
{
    return m_std_bernoulli(m_generator);
}

// Implementation from scratch
bool Bernoulli::generate_scratch()
{
    double u = m_uniform_dist(m_generator);
    return u < m_probability_var;
}

// Compare implementations
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

    double error_std     = std::abs(p_empirical_std - m_probability_var);
    double error_scratch = std::abs(p_empirical_scratch - m_probability_var);

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "=== Comparaison sur " << n << " iterations (p = " << m_probability_var
              << ") ===\n";

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

    double theoretical_std_dev = std::sqrt((m_probability_var * (1.0 - m_probability_var)) / n);
    std::cout << "   (Rappel : l'ecart-type theorique attendu est d'environ " << theoretical_std_dev
              << ")\n";
    std::cout << "========================================================\n\n";
}