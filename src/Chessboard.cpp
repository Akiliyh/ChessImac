#include "Chessboard.hpp"
#include <algorithm>
#include <cctype>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <utility>
#include <vector>
#include "GameManager.hpp"
#include "Pieces.hpp"

void Chessboard::init_board()
{
    // do smth here;
}

// for now we load the board from fen
// WE REALLY NEED TO USE UNIQUE PTR NOW LOL
// Lilian : I'm not sure now

void Chessboard::load_board_from_fen(const std::string& positionData)
{
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

bool Chessboard::move_piece(std::unique_ptr<Piece>& active_square, int dest_position)
{
    std::vector<int>        legal_moves   = active_square->get_moves(this->board_data);

        // we check if the move is legal
        if (std::find(legal_moves.begin(), legal_moves.end(), dest_position) != legal_moves.end())
        {
            active_square->update_position(dest_position);

            if (active_square->is_first_move())
            {
                active_square->update_first_move(false);
            }

            this->board_data[dest_position] = std::move(active_square);

            return true;
        }
        else
        {
            std::cout << "Illegal move!" << '\n';
            return false;
        }
}

std::string Chessboard::to_alg_position(int index) const
{
    int x = index % m_board_size;
    int y = index / m_board_size;

    char file = 'a' + x;
    char rank = '1' + ((m_board_size - 1) - y);

    return std::string{file, rank};
}