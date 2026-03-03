#include "GameManager.hpp"
#include <iostream>
#include "Pieces.hpp"

void GameManager::move_piece(int from_position, int dest_position)
{
    std::unique_ptr<Piece>& active_square = board.get_board_data()[from_position];
    if (is_player_move(active_square->get_color()))
    {
        bool piece_moved = board.move_piece(active_square, dest_position);
        if (piece_moved)
        {
            add_turn();
            display_move();
        }
    }
};

int GameManager::get_turn() const
{
    return m_turn;
};

void GameManager::add_turn()
{
    m_turn++;
}

void GameManager::display_move() const
{
    if (m_turn % 2 == 0)
    {
        std::cout << "Move " << m_turn / 2 << '\n';
    }
}

bool GameManager::is_white_turn() const
{
    return m_turn % 2 == 0;
}

void GameManager::play_game()
{
    int test_move{0};
    while (this->get_turn() < 100)
    {
        if (this->is_white_turn())
        {
            std::cout << "White move :" << '\n';
            std::cout << "turn :" << this->get_turn() << '\n';
            this->add_turn();
        }
        else
        {
            std::cout << "Black move :" << '\n';
            std::cout << "turn :" << this->get_turn() << '\n';
            this->add_turn();
        }
    }
}

bool GameManager::is_player_move(const PieceColor& player_color) const
{
    return (player_color == White && is_white_turn())
           || (player_color == Black && !is_white_turn());
};