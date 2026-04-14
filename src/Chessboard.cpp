#include "Chessboard.hpp"
#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstddef>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <utility>
#include <vector>
#include "GameManager.hpp"
#include "Pieces.hpp"
#include "Probabilities/Bernoulli.hpp"

void Chessboard::init_board()
{
    // do smth here;
}

// for now we load the board from fen
// WE REALLY NEED TO USE UNIQUE PTR NOW LOL
// Lilian : I'm not sure now

void Chessboard::load_board_from_fen(const std::string& positionData)
{
    // first we reset the board here
    board_data.clear();
    board_data.resize(64);
    dead_pieces.clear();

    std::map<char, std::function<std::unique_ptr<Piece>()>> pieceCorrespondance = {
        {'r', []() { return std::make_unique<Rook>(-1, -1, Black); }},
        {'p', []() { return std::make_unique<Pawn>(-1, -1, Black); }},
        {'k', []() { return std::make_unique<King>(-1, -1, Black); }},
        {'n', []() { return std::make_unique<Knight>(-1, -1, Black); }},
        {'q', []() { return std::make_unique<Queen>(-1, -1, Black); }},
        {'b', []() { return std::make_unique<Bishop>(-1, -1, Black); }},
    };

    std::vector<std::pair<int, char>> piecePositions{};
    int                               index{};

    for (int i = 0; i < positionData.length(); i++)
    {
        if (std::isdigit(positionData[i]))
        {
            index += (positionData[i] - '0'); // shenanigan to get the correct int
        }
        else
        {
            if (positionData[i] != '/')
            {
                piecePositions.emplace_back(index, positionData[i]);
                index++;
            }
        }
    }

    for (int i = 0; i < piecePositions.size(); i++)
    {
        char pieceChar = piecePositions[i].second;
        int  position  = piecePositions[i].first;

        // Piece* piece         = pieceCorrespondance[std::tolower(pieceChar)]();
        std::unique_ptr<Piece> piece =
            pieceCorrespondance[static_cast<char>(std::tolower(pieceChar))]();
        piece->update_position(position);
        board_data[position] = std::move(piece);

        if (isupper(pieceChar))
        {
            board_data[position]->update_color(White);
            board_data[position]->update_label(pieceChar);
        }
    }
}

int Chessboard::get_size() const
{
    return m_board_size;
}

std::vector<std::unique_ptr<Piece>>& Chessboard::get_board_data()
{
    return board_data;
}

std::unique_ptr<Piece>& Chessboard::get_board_data(int i)
{
    return board_data[i];
}

Chessboard::Chessboard() : board_data(64)
{}

std::vector<std::unique_ptr<Piece>>& Chessboard::get_dead_pieces()
{
    return dead_pieces;
}

bool Chessboard::move_piece(
    std::unique_ptr<Piece>& active_square, int dest_position, GameManager& game
)
{
    std::vector<int> legal_moves = active_square->get_moves(this->board_data);

    // On vérifie si le mouvement est légal
    if (std::find(legal_moves.begin(), legal_moves.end(), dest_position) != legal_moves.end())
    {
        if (game.get_mode() == GameMode::Classic)
        {
            active_square->update_position(dest_position);

            if (active_square->is_first_move())
            {
                active_square->update_first_move(false);
            }

            // On vérifie si une pièce ennemie est à la destination
            if (this->board_data[dest_position] != nullptr)
            {
                // On la sauvegarde dans dead_pieces
                dead_pieces.push_back(std::move(this->board_data[dest_position]));
            }

            this->board_data[dest_position] = std::move(active_square);

            return true;
        }

        if (game.get_mode() == GameMode::Chaos)
        {
            // 1. On gère la collision s'il y a un ennemi
            if (this->board_data[dest_position] != nullptr)
            {
                double    p = 0.8; // 80% de chance que l'attaque réussisse
                Bernoulli random_dodge(p);

                if (!random_dodge.generate_scratch()) // Si ça renvoie false, la pièce a esquivé
                {
                    std::cout << "Esquive ! Pas de chance." << std::endl;
                    // On retourne true car l'action a été validée (le tour est joué),
                    // mais la pièce attaquante reste à sa place d'origine.
                    return true;
                }

                // Si l'attaque a réussi, on capture la pièce ennemie
                dead_pieces.push_back(std::move(this->board_data[dest_position]));
            }

            // 2. On met à jour les états internes de la pièce AVANT le std::move
            active_square->update_position(dest_position);

            if (active_square->is_first_move())
            {
                active_square->update_first_move(false);
            }

            // 3. On déplace finalement la pièce sur le plateau
            // (Ceci s'exécute si l'attaque a réussi OU si la case était vide)
            this->board_data[dest_position] = std::move(active_square);

            return true;
        }
    }
    else
    {
        std::cout << "Illegal move!" << '\n';
        return false;
    }

    return false; // Sécurité de fin de fonction
}

std::string Chessboard::to_alg_position(int index) const
{
    int x = index % m_board_size;
    int y = index / m_board_size;

    char file = 'a' + x;
    char rank = '1' + ((m_board_size - 1) - y);

    return std::string{file, rank};
}