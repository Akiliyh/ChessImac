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

void Chessboard::load_board_from_fen(const std::string& fen)
{
    std::map<char, std::function<Piece*()>> pieceCorrespondance = {
        {'r', []() { return new Rook(-1, -1, Black); }},
        {'p', []() { return new Pawn(-1, -1, Black); }},
        {'k', []() { return new King(-1, -1, Black); }},
        {'n', []() { return new Knight(-1, -1, Black); }},
        {'q', []() { return new Queen(-1, -1, Black); }},
        {'b', []() { return new Bishop(-1, -1, Black); }},
    };

    std::string space        = " ";
    std::string positionData = fen.substr(0, fen.find(space)); // without castling and who to play
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

        Piece* piece         = pieceCorrespondance[std::tolower(pieceChar)]();
        piece->m_position    = position;
        board_data[position] = piece;

        if (isupper(pieceChar))
        {
            board_data[position]->m_color = White;
            board_data[position]->m_label = pieceChar;
        }
    }
}

int Chessboard::get_size() const
{
    return m_board_size;
}

Chessboard::Chessboard() : board_data(64, nullptr)
{}

void Chessboard::move_piece(Piece* active_square, int dest_position)
{
    std::vector<int> legal_moves = active_square->get_moves(this->board_data);

    // we check if the move is legal
    if (std::find(legal_moves.begin(), legal_moves.end(), dest_position) != legal_moves.end())
    {
        this->board_data[active_square->m_position] = (nullptr);
        this->board_data[dest_position]             = (active_square);
        active_square->m_position                   = dest_position;

        if (active_square->first_move)
        {
            active_square->first_move = false;
        }
    }
    else
    {
        std::cout << "Illegal move!" << std::endl;
    }
}