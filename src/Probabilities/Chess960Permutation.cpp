#include "Chess960Permutation.hpp"
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <map>
#include <numeric>

Chess960Permutation::Chess960Permutation()
{
    std::random_device rd;
    m_random.seed(rd());
}

bool Chess960Permutation::is_valid_chess960(const std::vector<char>& rank) const
{
    int bishop1 = -1;
    int bishop2 = -1;
    int rook1   = -1;
    int king    = -1;
    int rook2   = -1;

    for (int i = 0; i < 8; ++i)
    {
        if (rank[i] == 'B')
        {
            if (bishop1 == -1)
                bishop1 = i;
            else
                bishop2 = i;
        }
        else if (rank[i] == 'R')
        {
            if (rook1 == -1)
                rook1 = i;
            else
                rook2 = i;
        }
        else if (rank[i] == 'K')
        {
            king = i;
        }
    }
    if ((bishop1 % 2) == (bishop2 % 2))
        return false;
    if (king < rook1 || king > rook2)
        return false;
    return true;
}

std::string Chess960Permutation::build_fen(const std::vector<char>& back_rank) const
{
    std::string fen = "";
    for (char c : back_rank)
        fen += std::tolower(c);
    fen += "/pppppppp/8/8/8/8/PPPPPPPP/";
    for (char c : back_rank)
        fen += c;
    fen += " w KQkq - 0 1";
    return fen;
}

std::string Chess960Permutation::generate_std()
{
    std::vector<char> currentRank = m_base_pieces;

    while (true)
    {
        std::shuffle(currentRank.begin(), currentRank.end(), m_random);
        if (is_valid_chess960(currentRank))
        {
            break; // Exit when position is valid
        }
    }

    return build_fen(currentRank);
}

std::string Chess960Permutation::generate_scratch()
{
    std::vector<char> currentRank = m_base_pieces;

    while (true)
    {
        custom_fisher_yates_shuffle(currentRank);
        if (is_valid_chess960(currentRank))
        {
            break; // Exit when position is valid
        }
    }

    return build_fen(currentRank);
}

void Chess960Permutation::custom_fisher_yates_shuffle(std::vector<char>& vec)
{
    for (int i = vec.size() - 1; i > 0; --i)
    {
        std::uniform_int_distribution<int> dist(0, i);
        int                                j = dist(m_random);
        std::swap(vec[i], vec[j]);
    }
}

void Chess960Permutation::compare_methods(int iterations)
{
    std::map<std::string, int> libCounts;
    std::map<std::string, int> srcCounts;

    std::cout << "Analyse statistique sur " << iterations << " iterations...\n";

    for (int i = 0; i < iterations; ++i)
    {
        libCounts[generate_std()]++;
        srcCounts[generate_scratch()]++;
    }

    double expected = static_cast<double>(iterations) / 960.0;

    auto computeStats = [&](const std::map<std::string, int>& counts, std::string label) {
        double totalAbsError = 0;
        int    maxAbsError   = 0;

        for (auto const& [fen, count] : counts)
        {
            int err = std::abs(count - expected);
            totalAbsError += err;
            if (err > maxAbsError)
                maxAbsError = err;
        }

        // MAE: Mean Absolute Error
        double mae = totalAbsError / 960.0;
        // Max relative error %
        double maxRelError = (maxAbsError / expected) * 100.0;
        // Mean error %
        double avgRelError = (mae / expected) * 100.0;

        std::cout << "[" << label << "]\n";
        std::cout << "- Positions trouvees      : " << counts.size() << " / 960\n";
        std::cout << "- Erreur moyenne          : " << std::fixed << std::setprecision(2) << mae
                  << " tirages (" << avgRelError << "%)\n";
        std::cout << "- Erreur maximale         : " << maxAbsError << " tirages (" << maxRelError
                  << "%)\n";
        std::cout << "- Ecart type (Z-Score max): "
                  << (maxAbsError / std::sqrt(iterations * (1.0 / 960.0) * (959.0 / 960.0)))
                  << " Sigma\n\n";
    };

    std::cout << "\n--- Resultats de la Mise a Jour ---\n";
    std::cout << "Frequence cible : " << expected << "\n\n";
    computeStats(libCounts, "std::shuffle");
    computeStats(srcCounts, "Fisher-Yates From Scratch");

    std::cout << "Note : L'erreur relative (%) doit diminuer quand les iterations augmentent.\n";
    std::cout
        << "Le score Sigma est affiche pour reference mais reste naturellement entre 3 et 5.\n";
}