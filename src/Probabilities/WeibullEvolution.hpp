#pragma once

class WeibullEvolution {
  private:
    double m_shape; // Parametre k (forme) - Doit etre superieur a 1 pour ton besoin
    double m_scale; // Parametre lambda (echelle)

  public:
    WeibullEvolution(double shape_param, double scale_param)
        : m_shape(shape_param), m_scale(scale_param) {};

    // Monte Carlo with std::weibull_distribution
    double get_probability_std(int moves, int samples = 100000);

    // Formule mathematique exacte CDF
    double get_probability_scratch(int moves) const;

    void compare_methods(int max_moves, int samples = 100000);
};

// k = 2.0 (la probabilite augmente a chaque deplacement)
// lambda = 15.0 (une piece evoluera tres probablement autour de 15 cases)
// WeibullEvolution chessEvolution(2.0, 15.0);

// On compare sur 20 cases de deplacement
// chessEvolution.compare_methods(20);