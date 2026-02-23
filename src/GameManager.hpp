#pragma once

#include "Pieces.hpp"

struct GameManager {
  private:
    int m_turn{0};

  public:
    int  get_turn() const;
    void add_turn();

    bool is_white_turn() const;

    void play_game();
    void player_move(const PieceColor& player_color);
};