#include "GameManager.hpp"
#include <iostream>
#include "Chessboard.hpp"
#include "Pieces.hpp"

void GameManager::move_piece(int from_position, int dest_position)
{
    std::unique_ptr<Piece>& active_square = board.get_board_data()[from_position];
    if (is_player_move(active_square->get_color()))
    {
        bool piece_moved = board.move_piece(active_square, dest_position);
        if (piece_moved)
        {
            add_move();
            display_full_move();

            std::string dest_alg_position = board.to_alg_position(dest_position);

            // we add this to the move history
            m_move_history.emplace_back(
                board.get_board_data()[dest_position]->get_label(), dest_alg_position
            );
        }
    }
};

int GameManager::get_move() const
{
    return m_move;
};

std::vector<std::pair<char, std::string>> GameManager::get_move_history() const
{
    return m_move_history;
};

void GameManager::add_move()
{
    m_move++;
    if (m_move % 2 == 0)
    {
        m_full_move++;
    }
}

int GameManager::get_full_move() const
{
    return m_full_move;
};

void GameManager::display_full_move() const
{
    std::cout << "Move " << m_full_move << '\n';
}

bool GameManager::is_white_turn() const
{
    return m_move % 2 == 0;
}

void GameManager::play_game()
{
    // int test_move{0};
    // while (this->get_move() < 100)
    // {
    //     if (this->is_white_turn())
    //     {
    //         std::cout << "White move :" << '\n';
    //         std::cout << "turn :" << this->get_move() << '\n';
    //         this->add_move();
    //     }
    //     else
    //     {
    //         std::cout << "Black move :" << '\n';
    //         std::cout << "turn :" << this->get_move() << '\n';
    //         this->add_move();
    //     }
    // }

    // if (one_king_dead())
    // {
    //     game_win(who_king_dead());
    // }
}

// Pass the actual game board into the function
bool GameManager::is_king_dead()
{
    for (const auto& piece : board.get_dead_pieces())
    {
        if (piece->get_label() == 'k' || piece->get_label() == 'K')
        {
            return true;
        }
    }
    return false;
}

bool GameManager::is_player_move(const PieceColor& player_color) const
{
    return (player_color == White && is_white_turn())
           || (player_color == Black && !is_white_turn());
};

void GameManager::load_game_from_fen(const std::string& fen) {
    std::string space        = " ";
    std::string positionData = fen.substr(0, fen.find(space)); // without castling and who to play
    // if it's black to play we start with an odd m_move
    if (fen.substr(fen.find(space) + 1, 1) == "b")
        m_move++;
    board.load_board_from_fen(positionData);
};