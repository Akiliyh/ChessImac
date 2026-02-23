#include "Pieces.hpp"
#include <vector>
#include "Moves.hpp"

//=============================================================================
// ROOK FUNCTIONS
//=============================================================================

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
    Moves::rook_moves(m_position, m_color, free_case, board);
    return free_case;
}

//=============================================================================
// BISHOP FUNCTIONS
//=============================================================================

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
    Moves::bishop_moves(m_position, m_color, free_case, board);
    return free_case;
};

//=============================================================================
// QUEEN FUNCTIONS
//=============================================================================

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
    Moves::queen_moves(m_position, m_color, free_case, board);
    return free_case;
};

//=============================================================================
// KNIGHT FUNCTIONS
//=============================================================================

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
    Moves::knight_moves(m_position, m_color, free_case, board);
    return free_case;
}

//=============================================================================
// KING FUNCTIONS
//=============================================================================

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
    Moves::king_moves(m_position, m_color, free_case, board);
    return free_case;
}

//=============================================================================
// PAWN FUNCTIONS
//=============================================================================

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
    Moves::pawn_moves(m_position, m_color, free_case, board, first_move);
    return free_case;
};