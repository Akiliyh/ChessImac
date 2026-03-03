#pragma once

#include "Chessboard.hpp"
#include "Pieces.hpp"


struct GameManager {
  private:
    int m_turn{0};

  public:
    Chessboard board{}; // let's put it in private once it works
    int        get_board() const;
    int        get_turn() const;
    void       add_turn();
    void       display_move() const;
    void       display_who_is_next(PieceColor player_color) const;
    bool       is_white_turn() const;
    bool       is_player_move(const PieceColor& player_color) const;
    void       move_piece(int from_position, int dest_position);

    void play_game();

    GameManager()
    {
        board.load_board_from_fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    }
};