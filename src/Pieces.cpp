#include "Pieces.hpp"
#include <array>
#include <cstddef>
#include <cstdlib>
#include <utility>
#include <vector>
#include <iostream>

Rook::Rook(const int position, const bool white) : Piece(position, white) {};

std::vector<int> Rook::get_moves(std::vector<Piece*>& board)
{
    std::vector<int> free_case;
    // std::vector<int> take_case;

    // std::vector<std::vector<int>> moves;

    for (int i = m_position + 8; i < 64; i += 8)
    {
        int next_case{i};

        if (board[next_case] == nullptr)
        {
            // board[m_position + 8] => color space free;
            free_case.push_back(next_case);
        }
        else
        {
            if (this->m_white != board[next_case]->m_white)
            {
                // board[m_position + 8] => color space taken;
                free_case.push_back(next_case);
            }
        }
    }

    for (int i = m_position - 8; i >= 0; i -= 8)
    {
        int next_case{i};
        if (board[next_case] == nullptr)
        {
            // board[m_position - 8] => color space free;
            free_case.push_back(next_case);
        }
        else
        {
            if (this->m_white != board[next_case]->m_white)
            {
                // board[m_position - 8] => color space taken;
                free_case.push_back(next_case);
            }
        }
    }

    // for (int i = m_position + 1 ; i < 64 ; i += 1) {
    //     int next_case {i};
    //     if (board[next_case] == nullptr) {
    //         // board[m_position + 1] => color space free;
    //         free_case.push_back(next_case);
    //     }
    //     else
    //     {
    //         if (this->m_white != board[next_case]->m_white) {
    //             // board[m_position + 1] => color space taken;
    //         free_case.push_back(next_case);
    //         }
    //     }
    // }

    int i = m_position + 1;
    while (i < 64 && (i / 8) == (m_position / 8))
    {
        if (board[i] == nullptr)
        {
            free_case.push_back(i);
        }
        else
        {
            if (board[i]->m_white != m_white)
            {
                free_case.push_back(i); // capture
            }
            break; // on s'arrête dans tous les cas
        }
        i++;
    }

    // for (int i = m_position - 1 ; i >= 0 ; i -= 1) {
    // int next_case {i};
    //     if (board[next_case] == nullptr) {
    //         // board[m_position - 1] => color space free;
    //         free_case.push_back(next_case);
    //     }
    //     else
    //     {
    //         if (this->m_white != board[next_case]->m_white) {
    //             // board[m_position - 1] => color space taken;
    //         free_case.push_back(next_case);
    //         }
    //     }
    // }

    i = m_position - 1;
    while (i >= 0 && (i / 8) == (m_position / 8))
    {
        if (board[i] == nullptr)
        {
            free_case.push_back(i);
        }
        else
        {
            if (board[i]->m_white != m_white)
            {
                free_case.push_back(i);
            }
            break;
        }
        i--;
    }

    // moves.push_back(free_case);
    // moves.push_back(take_case);

    return free_case;
}

// std::vector<std::vector<int>> Bishop::get_moves(std::vector<Piece*>& board){

// };

// std::vector<int> Bishop::get_moves(const Board& board){

// };

Knight::Knight(const int position, const bool white) : Piece(position, white) {};

std::vector<int> Knight::get_moves(std::vector<Piece*>& board)
{
    std::vector<int> free_case;
    // std::vector<int> take_case;

    // std::vector<std::vector<int>> moves;
    int row {m_position / 8};
    int col {m_position % 8};
    std::array <std::pair<int, int>, 8> knight_orientation{{{-2,1}, {-1,2}, {1,2}, {2,1}, {-2,-1}, {-1,-2}, {1,-2}, {2,-1}}};
    // explaination for 36 we want to have an L shape
    // -1 row and +2 col makes this and so on
    //         ///
    //        o/

    int next_case {};
    for (auto i : knight_orientation)
    {
        int new_row = row + i.first;
        int new_col = col + i.second;
        // we remove the uncoherent cols and rows
        if (new_row < 0 || new_row >= 8 || new_col < 0 || new_col >= 8) {
            continue;
        }

        next_case = new_row * 8 + new_col;
        if (next_case < 64 && next_case >= 0)
        {
            if (board[next_case] == nullptr)
            {
                free_case.push_back(next_case);
            }
            else
            {
                if (this->m_white != board[next_case]->m_white)
                {
                    // board[m_position + 8] => color space taken;
                    free_case.push_back(next_case);
                }
            }
        }
    }

    // moves.push_back(free_case);
    // moves.push_back(take_case);

    return free_case;
}