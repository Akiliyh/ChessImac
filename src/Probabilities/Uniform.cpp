#include "Uniform.hpp"

std::mt19937& Uniform::m_generator()
{
    static std::random_device rd;
    static std::mt19937       gen(rd());
    return gen;
}

int Uniform::generate_int(int min, int max)
{
    std::uniform_int_distribution<int> distrib(min, max);
    return distrib(m_generator());
}

double Uniform::generate_real(double min, double max)
{
    std::uniform_real_distribution<double> distrib(min, max);
    return distrib(m_generator());
}