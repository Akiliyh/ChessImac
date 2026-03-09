#pragma once

#include <utility>
#include <vector>
#include "Chessboard.hpp"
#include "Pieces.hpp"

struct GameManager {
  private:
    int m_full_move{0}; // full move is both color played capice? mamma miaaa
    int m_move{0};
    std::vector<std::pair<char, int>>
        m_move_history{}; // we store the type of piece and the square of destination
    // maybe later we could store if it is a capture, promotion or check eventually

  public:
    Chessboard                        board{}; // let's put it in private once it works
    int                               get_board() const;
    int                               get_full_move() const;
    void                              add_move();
    int                               get_move() const;
    std::vector<std::pair<char, int>> get_move_history() const;
    void                              display_full_move() const;
    void                              display_who_is_next(PieceColor player_color) const;
    bool                              is_white_turn() const;
    bool                              is_player_move(const PieceColor& player_color) const;
    void                              move_piece(int from_position, int dest_position);
    bool                              is_king_dead();
    // void                              game_win(PieceColor winner_color);

    void play_game();

    GameManager()
    {
        board.load_board_from_fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    }
};