#pragma once // Protection moderne contre les inclusions multiples

#include <random>
#include "Probabilities/ExpoLaw.hpp"

class ExpoLaw {
  public:
    // Constructeur : initialise le générateur de nombres aléatoires
    ExpoLaw();

    // Méthode de simulation de la loi exponentielle
    void simulateExpoLaw(int iterations, double lambda);

  private:
    // Le générateur aléatoire est un membre de la classe (meilleure pratique)
    std::random_device m_random;
    std::mt19937       m_generator;
};