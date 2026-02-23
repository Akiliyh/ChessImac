#pragma once
#include <memory>
#include <vector>
#include "Pieces.hpp"

struct Moves {
  public:
    static void get_moves_vertical(
        int m_position, PieceColor m_color, int delta, const std::vector<std::unique_ptr<Piece>>& board,
        std::vector<int>& moves
    );
    static void get_moves_horizontal(
        int m_position, PieceColor m_color, int delta, const std::vector<std::unique_ptr<Piece>>& board,
        std::vector<int>& moves
    );
    static void get_moves_diag(
        int m_position, PieceColor m_color, int delta, const std::vector<std::unique_ptr<Piece>>& board,
        std::vector<int>& moves
    );
    static void get_moves_pawn(
        int m_position, PieceColor m_color, const std::vector<std::unique_ptr<Piece>>& board,
        std::vector<int>& moves, bool first_move
    );

    static std::vector<int> bishop_moves(
        int m_position, PieceColor m_color, std::vector<int>& free_case, const std::vector<std::unique_ptr<Piece>>& board
    );
    static std::vector<int> rook_moves(
        int m_position, PieceColor m_color, std::vector<int>& free_case, const std::vector<std::unique_ptr<Piece>>& board
    );
    static std::vector<int> knight_moves(
        int m_position, PieceColor m_color, std::vector<int>& free_case, const std::vector<std::unique_ptr<Piece>>& board
    );
    static std::vector<int> queen_moves(
        int m_position, PieceColor m_color, std::vector<int>& free_case, const std::vector<std::unique_ptr<Piece>>& board
    );
    static std::vector<int> king_moves(
        int m_position, PieceColor m_color, std::vector<int>& free_case, const std::vector<std::unique_ptr<Piece>>& board
    );
    static std::vector<int> pawn_moves(
        int m_position, PieceColor m_color, std::vector<int>& free_case, const std::vector<std::unique_ptr<Piece>>& board,
        bool first_move
    );
};