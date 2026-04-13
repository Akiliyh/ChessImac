#pragma once
#include <random>
#include <string>
#include <vector>

class Chess960Permutation {
  private:
    std::mt19937            rng;
    const std::vector<char> basePieces = {'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'};

    bool        isValidChess960(const std::vector<char>& rank) const;
    std::string buildFen(const std::vector<char>& backRank) const;
    void        customFisherYatesShuffle(std::vector<char>& vec);

  public:
    Chess960Permutation();
    std::string generateWithLibrary();
    std::string generateFromScratch();

    // Nouvelle version de la comparaison
    void compareMethods(int iterations);
};

// Chess960Permutation generator;

// std::cout << "Generation via Standard Library :\n";
// std::cout << generator.generateWithLibrary() << "\n\n";

// std::cout << "Generation via From Scratch :\n";
// std::cout << generator.generateFromScratch() << "\n\n";

// Lance une comparaison statistique lourde (ex: 1 million de parties)
// Cela prendra quelques secondes mais te prouvera la qualite de ta permutation.
// generator.compareMethods(1000000);