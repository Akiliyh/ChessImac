#include "FishLaw.hpp"
#include <cmath>

FishLaw::FishLaw(double lambda, unsigned int seed)
    : m_lambda(lambda), m_generator(seed), m_dist(0.0, 1.0)
{}

// Knuth Algorithm
int FishLaw::generate()
{
    double L = std::exp(-m_lambda);
    double p = 1.0;
    int    k = 0;

    p *= m_dist(m_generator);

    while (p > L)
    {
        ++k;
        p *= m_dist(m_generator);
    }

    return k;
}

void FishLaw::set_lambda(double lambda)
{
    m_lambda = lambda;
}

double FishLaw::get_lambda() const
{
    return m_lambda;
}