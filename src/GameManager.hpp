#pragma once

#include "Pieces.hpp"

struct GameManager {
  private:
    int m_turn{0};

  public:
    int  get_turn() const;
    void add_turn();
    void display_move() const;
    void display_who_is_next(PieceColor player_color) const;
    bool is_white_turn() const;
    bool is_player_move(const PieceColor& player_color) const;

    void play_game();
};