#pragma once

class WeibullEvolution {
  private:
    double shape; // Parametre k (forme) - Doit etre superieur a 1 pour ton besoin
    double scale; // Parametre lambda (echelle)

  public:
    // Constructeur
    WeibullEvolution(double shapeParam, double scaleParam);

    // Methode utilisant la librairie standard (Monte Carlo avec std::weibull_distribution)
    double getProbabilityLibrary(int moves, int samples = 100000);

    // Methode from scratch (Formule mathematique exacte CDF)
    double getProbabilityScratch(int moves);

    // Methode pour comparer l'ecart entre les deux approches sur n iterations
    void compareMethods(int maxMoves, int samples = 100000);
};

// k = 2.0 (la probabilite augmente a chaque deplacement)
// lambda = 15.0 (une piece evoluera tres probablement autour de 15 cases)
// WeibullEvolution chessEvolution(2.0, 15.0);

// On compare sur 20 cases de deplacement
// chessEvolution.compareMethods(20);