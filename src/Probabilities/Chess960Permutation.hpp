#pragma once
#include <random>
#include <string>
#include <vector>

class Chess960Permutation {
  private:
    std::mt19937            m_random;
    const std::vector<char> m_base_pieces = {'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'};

    bool        is_valid_chess960(const std::vector<char>& rank) const;
    std::string build_fen(const std::vector<char>& back_rank) const;
    void        custom_fisher_yates_shuffle(std::vector<char>& vec);

  public:
    Chess960Permutation();
    std::string generate_std();
    std::string generate_scratch();

    void compare_methods(int iterations);
};

// Chess960Permutation generator;

// std::cout << "Generation via Standard Library :\n";
// std::cout << generator.generate_std() << "\n\n";

// std::cout << "Generation via From Scratch :\n";
// std::cout << generator.generate_scratch() << "\n\n";

// Lance une comparaison statistique lourde (ex: 1 million de parties)
// Cela prendra quelques secondes mais te prouvera la qualite de ta permutation.
// generator.compare_methods(1000000);