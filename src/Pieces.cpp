#include "Pieces.hpp"
#include <array>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <utility>
#include <vector>

void Moves::get_moves_vertical(
    const int m_position, const bool m_color, const int delta, const std::vector<Piece*>& board,
    std::vector<int>& moves
)
{
    for (int i = m_position + delta; i >= 0 && i < 64; i += delta)
    {
        int next_case{i};

        if (board[next_case] == nullptr)
        {
            moves.push_back(next_case);
        }
        else
        {
            if (m_color != board[next_case]->m_color)
            {
                moves.push_back(next_case);
                break;
            }
            break;
        }
    }
};

void Moves::get_moves_horizontal(
    const int m_position, const bool m_color, const int delta, const std::vector<Piece*>& board,
    std::vector<int>& moves
)
{
    int i{};

    i = m_position + delta;

    while (i >= 0 && i < 64 && (i / 8) == (m_position / 8))
    {
        if (board[i] == nullptr)
        {
            moves.push_back(i);
        }
        else
        {
            if (board[i]->m_color != m_color)
            {
                moves.push_back(i);
                break;
            }

            break;
        }
        i += delta;
    }
}

void Moves::get_moves_diag(
    const int m_position, const bool m_color, const int delta, const std::vector<Piece*>& board,
    std::vector<int>& moves
)
{
    int i    = m_position + delta;
    int prev = m_position;

    while (i >= 0 && i < 64 && abs((i % 8) - (prev % 8)) == 1)
    {
        if (!board[i])
        {
            moves.push_back(i);
        }
        else
        {
            if (board[i]->m_color != m_color)
                moves.push_back(i);
            break;
        }
        prev = i;
        i += delta;
    }
}

Rook::Rook(const int x, const int y, const PieceColor color) : Piece(x, y, color)
{
    m_label = color == White ? 'R' : 'r';
};

Rook::Rook(const std::string& alg_notation, const PieceColor color) : Piece(alg_notation, color)
{
    m_label = color == White ? 'R' : 'r';
};

std::vector<int> Rook::get_moves(std::vector<Piece*>& board)
{
    std::vector<int> free_case;

    int top{-8};
    int bottom{8};
    int left{-1};
    int right{1};

    Moves::get_moves_vertical(m_position, m_color, top, board, free_case);
    Moves::get_moves_vertical(m_position, m_color, bottom, board, free_case);
    Moves::get_moves_horizontal(m_position, m_color, left, board, free_case);
    Moves::get_moves_horizontal(m_position, m_color, right, board, free_case);

    return free_case;
}

Bishop::Bishop(const int x, const int y, const PieceColor color) : Piece(x, y, color)
{
    m_label = color == White ? 'B' : 'b';
};

Bishop::Bishop(const std::string& alg_notation, const PieceColor color) : Piece(alg_notation, color)
{
    m_label = color == White ? 'B' : 'b';
};

std::vector<int> Bishop::get_moves(std::vector<Piece*>& board)
{
    std::vector<int> free_case;

    int top_left{-9};
    int top_right{-7};
    int bottom_left{9};
    int bottom_right{7};

    Moves::get_moves_diag(m_position, m_color, top_left, board, free_case);
    Moves::get_moves_diag(m_position, m_color, top_right, board, free_case);
    Moves::get_moves_diag(m_position, m_color, bottom_left, board, free_case);
    Moves::get_moves_diag(m_position, m_color, bottom_right, board, free_case);

    return free_case;
};

Queen::Queen(const int x, const int y, const PieceColor color) : Piece(x, y, color)
{
    m_label = color == White ? 'Q' : 'q';
};

Queen::Queen(const std::string& alg_notation, const PieceColor color) : Piece(alg_notation, color)
{
    m_label = color == White ? 'Q' : 'q';
};

std::vector<int> Queen::get_moves(std::vector<Piece*>& board)
{
    std::vector<int> free_case;

    int top{-8};
    int bottom{8};
    int left{-1};
    int right{1};

    Moves::get_moves_vertical(m_position, m_color, top, board, free_case);
    Moves::get_moves_vertical(m_position, m_color, bottom, board, free_case);
    Moves::get_moves_horizontal(m_position, m_color, left, board, free_case);
    Moves::get_moves_horizontal(m_position, m_color, right, board, free_case);

    int top_left{-9};
    int top_right{-7};
    int bottom_left{9};
    int bottom_right{7};

    Moves::get_moves_diag(m_position, m_color, top_left, board, free_case);
    Moves::get_moves_diag(m_position, m_color, top_right, board, free_case);
    Moves::get_moves_diag(m_position, m_color, bottom_left, board, free_case);
    Moves::get_moves_diag(m_position, m_color, bottom_right, board, free_case);

    return free_case;
};

Knight::Knight(const int x, const int y, const PieceColor color) : Piece(x, y, color)
{
    m_label = color == White ? 'N' : 'n';
};

Knight::Knight(const std::string& alg_notation, const PieceColor color) : Piece(alg_notation, color)
{
    m_label = color == White ? 'N' : 'n';
};

std::vector<int> Knight::get_moves(std::vector<Piece*>& board)
{
    std::vector<int> free_case;
    // std::vector<int> take_case;

    // std::vector<std::vector<int>> moves;
    int                                row{m_position / 8};
    int                                col{m_position % 8};
    std::array<std::pair<int, int>, 8> knight_orientation{
        {{-2, 1}, {-1, 2}, {1, 2}, {2, 1}, {-2, -1}, {-1, -2}, {1, -2}, {2, -1}}
    };
    // explaination for 36 we want to have an L shape
    // -1 row and +2 col makes this and so on
    //         oo/
    //        o/

    int next_case{};
    for (auto i : knight_orientation)
    {
        int new_row = row + i.first;
        int new_col = col + i.second;
        // we remove the uncoherent cols and rows
        if (new_row < 0 || new_row >= 8 || new_col < 0 || new_col >= 8)
        {
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
                if (this->m_color != board[next_case]->m_color)
                {
                    // board[m_position + 8] => color space taken;
                    free_case.push_back(next_case);
                }
            }
        }
    }

    return free_case;
}

King::King(const int x, const int y, const PieceColor color) : Piece(x, y, color)
{
    m_label = color == White ? 'K' : 'k';
};

King::King(const std::string& alg_notation, const PieceColor color) : Piece(alg_notation, color)
{
    m_label = color == White ? 'K' : 'k';
};

std::vector<int> King::get_moves(std::vector<Piece*>& board)
{
    std::vector<int> free_case;

    int                next_case{};
    std::array<int, 8> moves{7, 8, 9, 1, -7, -8, -9, -1};

    for (int delta : moves)
    {
        int i = m_position + delta;

        // hors échiquier
        if (i < 0 || i >= 64)
            continue;

        // empêche le wrap horizontal
        if (abs((i % 8) - (m_position % 8)) > 1)
            continue;

        // case vide
        if (board[i] == nullptr)
        {
            free_case.push_back(i);
        }
        // capture possible
        else if (board[i]->m_color != m_color)
        {
            free_case.push_back(i);
        }
    }

    return free_case;
}

Pawn::Pawn(const int x, const int y, const PieceColor color) : Piece(x, y, color)
{
    m_label = color == White ? 'P' : 'p';
};

Pawn::Pawn(const std::string& alg_notation, const PieceColor color) : Piece(alg_notation, color)
{
    m_label = color == White ? 'P' : 'p';
};

std::vector<int> Pawn::get_moves(std::vector<Piece*>& board)
{
    std::vector<int> free_case;

    int                next_case{};
    std::array<int, 3> moves_w{at(1, -1), at(0, -1), at(-1, -1)};
    std::array<int, 3> moves_b{at(-1, 1), at(0, 1), at(1, 1)};

    if (m_color == White)
    {
        for (int delta : moves_w)
        {
            int i = m_position + delta;

            // hors échiquier
            if (i < 0 || i >= 64)
                continue;

            // empêche le wrap horizontal
            if (abs((i % 8) - (m_position % 8)) > 1)
                continue;

            // case vide
            if (board[i] == nullptr && delta == moves_w[1])
            {
                if (first_move)
                {
                    free_case.push_back(i);
                    free_case.push_back(i + moves_w[1]);
                }
                else
                {
                    free_case.push_back(i);
                }
            }
            // capture possible
            if (board[i] != nullptr && board[i]->m_color != m_color
                && (delta == moves_w[0] || delta == moves_w[2]))
            {
                free_case.push_back(i);
            }
        }
    }
    else
    {
        for (int delta : moves_b)
        {
            int i = m_position + delta;

            // hors échiquier
            if (i < 0 || i >= 64)
                continue;

            // empêche le wrap horizontal
            if (abs((i % 8) - (m_position % 8)) > 1)
                continue;

            // case vide
            if (board[i] == nullptr && delta == moves_b[1])
            {
                if (first_move)
                {
                    free_case.push_back(i);
                    free_case.push_back(i + moves_b[1]);
                }
                else
                {
                    free_case.push_back(i);
                }
            }
            // capture possible
            if (board[i] != nullptr && board[i]->m_color != m_color
                && (delta == moves_b[0] || delta == moves_b[2]))
            {
                free_case.push_back(i);
            }
        }
    }

    return free_case;
};