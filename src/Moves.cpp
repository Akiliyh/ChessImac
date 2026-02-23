#include "Moves.hpp"
#include <array>
#include <vector>
#include "Pieces.hpp"

void Moves::get_moves_vertical(
    const int m_position, const bool m_color, const int delta, const std::vector<std::unique_ptr<Piece>>& board,
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
            if (m_color != board[next_case]->get_color())
            {
                moves.push_back(next_case);
                break;
            }
            break;
        }
    }
};

void Moves::get_moves_horizontal(
    const int m_position, const bool m_color, const int delta, const std::vector<std::unique_ptr<Piece>>& board,
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
            if (board[i]->get_color() != m_color)
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
    const int m_position, const bool m_color, const int delta, const std::vector<std::unique_ptr<Piece>>& board,
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
            if (board[i]->get_color() != m_color)
                moves.push_back(i);
            break;
        }
        prev = i;
        i += delta;
    }
}

std::vector<int> Moves::bishop_moves(
    const int m_position, const bool m_color, std::vector<int>& free_case,
    const std::vector<std::unique_ptr<Piece>>& board
)
{
    std::array<int, 4> directions{-9, 9, -7, 7};

    for (auto direction : directions)
    {
        Moves::get_moves_diag(m_position, m_color, direction, board, free_case);
    }

    return free_case;
}

std::vector<int> Moves::rook_moves(
    const int m_position, const bool m_color, std::vector<int>& free_case,
    const std::vector<std::unique_ptr<Piece>>& board
)
{
    std::array<int, 4> directions{-8, 8, -1, 1};

    for (auto direction : directions)
    {
        if (direction == 8 || direction == -8)
        {
            Moves::get_moves_vertical(m_position, m_color, direction, board, free_case);
        }
        else
        {
            Moves::get_moves_horizontal(m_position, m_color, direction, board, free_case);
        }
    }

    return free_case;
}

std::vector<int> Moves::knight_moves(
    const int m_position, const bool m_color, std::vector<int>& free_case,
    const std::vector<std::unique_ptr<Piece>>& board
)
{
    int row{m_position / 8};
    int col{m_position % 8};

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
                if (m_color != board[next_case]->get_color())
                {
                    // board[m_position + 8] => color space taken;
                    free_case.push_back(next_case);
                }
            }
        }
    }

    return free_case;
};

std::vector<int> Moves::queen_moves(
    const int m_position, const bool m_color, std::vector<int>& free_case,
    const std::vector<std::unique_ptr<Piece>>& board
)
{
    Moves::rook_moves(m_position, m_color, free_case, board);
    Moves::bishop_moves(m_position, m_color, free_case, board);

    return free_case;
};

std::vector<int> Moves::king_moves(
    const int m_position, const bool m_color, std::vector<int>& free_case,
    const std::vector<std::unique_ptr<Piece>>& board
)
{
    int next_case{};

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

        // case vide ou capture possible
        if (board[i] == nullptr || board[i]->get_color() != m_color)
        {
            free_case.push_back(i);
        }
    }

    return free_case;
};

std::vector<int> Moves::pawn_moves(
    const int m_position, const bool m_color, std::vector<int>& free_case,
    const std::vector<std::unique_ptr<Piece>>& board, const bool first_move
)
{
    int next_case{};

    std::array<int, 3> moves_w{Piece::at(1, -1), Piece::at(0, -1), Piece::at(-1, -1)};
    std::array<int, 3> moves_b{Piece::at(-1, 1), Piece::at(0, 1), Piece::at(1, 1)};

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
            if (board[i] != nullptr && board[i]->get_color() != m_color
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
            if (board[i] != nullptr && board[i]->get_color() != m_color
                && (delta == moves_b[0] || delta == moves_b[2]))
            {
                free_case.push_back(i);
            }
        }
    }
    return free_case;
};