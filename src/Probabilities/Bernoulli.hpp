#pragma once
#include <random>

class Bernoulli {
  private:
    double                                 probability_p;
    std::mt19937                           generator;
    std::bernoulli_distribution            std_bernoulli;
    std::uniform_real_distribution<double> uniform_dist;

  public:
    // Constructor
    Bernoulli(double p);

    // Methods
    bool generate_std();
    bool generate_scratch();
    void compare(int n);
};

// Test avec une probabilité p = 0.3 (30% de chance de succès)
// double    p = 0.3;
// Bernoulli sim(p);

// Comparaison avec différentes tailles d'échantillons
// sim.compare(100);     // Petit échantillon (forte variance attendue)
// sim.compare(10000);   // Échantillon moyen
// sim.compare(1000000); // Grand échantillon (loi des grands nombres bien visible)