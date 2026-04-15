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
#include "Probabilities/WeibullEvolution.hpp"

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

    // Is the move legal ?
    if (std::find(legal_moves.begin(), legal_moves.end(), dest_position) != legal_moves.end())
    {
        if (game.get_mode() == GameMode::Classic)
        {
            active_square->update_position(dest_position);
            if (active_square->is_first_move())
                active_square->update_first_move(false);
            if (this->board_data[dest_position] != nullptr)
            {
                dead_pieces.push_back(std::move(this->board_data[dest_position]));
            }
            this->board_data[dest_position] = std::move(active_square);
            return true;
        }

        if (game.get_mode() == GameMode::Chaos)
        {
            // Is there an enemy ?
            if (this->board_data[dest_position] != nullptr)
            {
                double    p = 0.75; // 75% to hit the enemy
                Bernoulli random_dodge(p);

                if (!random_dodge.generate_scratch())
                {
                    std::cout << "Esquive ! Pas de chance." << std::endl;
                    return true;
                }

                // Attack success
                dead_pieces.push_back(std::move(this->board_data[dest_position]));
            }

            active_square->update_position(dest_position);

            if (active_square->is_first_move())
            {
                active_square->update_first_move(false);
            }

            active_square->up_move_count();

            // Is the Piece mutating ?
            WeibullEvolution chessEvolution(2.0, 3.0);
            Bernoulli        random_evolution(
                chessEvolution.get_probability_scratch(active_square->get_move_count())
            );
            bool will_evolve = random_evolution.generate_scratch();

            this->board_data[dest_position] = std::move(active_square);

            if (will_evolve)
            {
                std::cout << "Mutation Chaos ! La piece evolue !" << std::endl;

                PieceColor current_color = this->board_data[dest_position]->get_color();

                int dest_x = dest_position % 8;
                int dest_y = dest_position / 8;

                int random_type = std::rand() % 4; // Un nombre entre 0 et 3

                switch (random_type)
                {
                case 0:
                    this->board_data[dest_position] =
                        std::make_unique<Queen>(dest_x, dest_y, current_color);
                    break;
                case 1:
                    this->board_data[dest_position] =
                        std::make_unique<Rook>(dest_x, dest_y, current_color);
                    break;
                case 2:
                    this->board_data[dest_position] =
                        std::make_unique<Bishop>(dest_x, dest_y, current_color);
                    break;
                case 3:
                    this->board_data[dest_position] =
                        std::make_unique<Knight>(dest_x, dest_y, current_color);
                    break;
                }
            }

            return true;
        }
    }
    else
    {
        std::cout << "Illegal move!" << '\n';
        return false;
    }

    return false;
}

std::string Chessboard::to_alg_position(int index) const
{
    int x = index % m_board_size;
    int y = index / m_board_size;

    char file = 'a' + x;
    char rank = '1' + ((m_board_size - 1) - y);

    return std::string{file, rank};
}