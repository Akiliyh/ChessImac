#pragma once

#include <random>

class ExpoLaw {
  private:
    std::random_device m_random;
    std::mt19937       m_generator;

  public:
    ExpoLaw();

    void simulateExpoLaw(int iterations, double lambda);

    // Nouvelle méthode pour obtenir un temps borné
    double get_bounded_time(double min_time, double max_time, double lambda);
};