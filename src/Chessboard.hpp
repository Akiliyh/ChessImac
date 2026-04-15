#pragma once
#include <memory>
#include <string>
#include <vector>
#include "Pieces.hpp"

class GameManager;
class Chessboard {
  private:
    int                                 m_board_size{8}; // 8 so 8x8 = 64 squares
    std::vector<std::unique_ptr<Piece>> board_data{};
    std::vector<std::unique_ptr<Piece>> dead_pieces{};

  public:
    int                                  get_size() const;
    std::vector<std::unique_ptr<Piece>>& get_board_data();
    std::unique_ptr<Piece>&              get_board_data(int i);
    std::vector<std::unique_ptr<Piece>>& get_dead_pieces();
    void                                 load_board_from_fen(const std::string& positionData);
    bool move_piece(std::unique_ptr<Piece>& active_square, int dest_position, GameManager& game);
    std::string to_alg_position(int index) const;

    Chessboard();
};