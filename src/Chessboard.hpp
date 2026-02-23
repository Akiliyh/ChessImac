#pragma once
#include <vector>
#include "Pieces.hpp"

class Chessboard {
  private:
    int                 m_board_size{8}; // 8 so 8x8 = 64 squares
    std::vector<Piece*> board_data{};

  public:
    void                 get_board_date();
    int                  get_board_size() const;
    int                  get_size() const;
    std::vector<Piece*>& get_board_data();
    Piece*&              get_board_data(int i);
    void                 init_board();
    void                 load_board_from_fen(const std::string& fen);
    void                 move_piece(Piece* active_square, int dest_position);
    Chessboard();
};