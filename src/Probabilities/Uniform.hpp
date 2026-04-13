#pragma once

#include <map>
#include <random>
#include <vector>

class Uniform {
  private:
    int min_val;
    int max_val;

    std::mt19937                       mt_engine;
    std::uniform_int_distribution<int> dist;

    unsigned long long              lcg_state;
    static const unsigned long long LCG_A = 1664525;
    static const unsigned long long LCG_C = 1013904223;
    static const unsigned long long LCG_M = 4294967296; // 2^32

  public:
    // Constructeur par defaut (intervalle de base 0 a 1)
    Uniform(int min = 0, int max = 1);

    // Nouvelle methode pour changer l'intervalle dynamiquement
    void setRange(int min, int max);

    int  generateWithLibrary();
    int  generateFromScratch();
    void compareMethods(int iterations);
};

// enum class PieceType { PION, FOU, CAVALIER, TOUR };

//     1. Definition des pools d'evolution
//     std::map<PieceType, std::vector<std::string>> evolutionPools;

//     Le pion a 4 choix
//     evolutionPools[PieceType::PION] = {"Reine", "Fou", "Cavalier", "Tour"};

//     Le fou a 2 choix (exemples de pieces inventees)
//     evolutionPools[PieceType::FOU] = {"Cavalier", "Tour", "Reine"};

//     Le cavalier a 3 choix
//     evolutionPools[PieceType::CAVALIER] = {"Fou", "Tour", "Reine"};

//     La tour a 2 choix
//     evolutionPools[PieceType::TOUR] = {"Reine"};

//     2. Creation de notre generateur unique
//     Uniform rng;

//     3. Simulation : Un fou evolue
//     PieceType pieceQuiEvolue = PieceType::FOU;

//     On recupere les choix possibles pour le fou
//     const auto& choixPossibles = evolutionPools[pieceQuiEvolue];

//     On ajuste dynamiquement notre classe Uniform a la taille du pool
//     Si le fou a 2 choix, l'intervalle sera [0, 1]
//     rng.setRange(0, choixPossibles.size() - 1);

//     On tire un index au hasard (avec la methode robuste)
//     int indexGagnant = rng.generateWithLibrary();

//     std::cout << "Le fou evolue en : " << choixPossibles[indexGagnant] << "\n";