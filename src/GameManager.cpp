#include "GameManager.hpp"
#include <iostream>
#include <optional>
#include <vector>
#include "Chessboard.hpp"
#include "Pieces.hpp"
#include "Probabilities/Chess960Permutation.hpp"

void GameManager::move_piece(int from_position, int dest_position)
{
    std::unique_ptr<Piece>& active_square = board.get_board_data()[from_position];
    if (is_player_move(active_square->get_color()) && !is_piece_promoting().has_value())
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
            last_move = Move{.from=from_position, .to=dest_position};
        }
    }
};

int GameManager::get_move() const
{
    return m_move;
};

std::optional<Move> GameManager::get_last_move() const {
    return last_move;
}

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
{}

// Return nothing if no king is dead
std::optional<PieceColor> GameManager::get_dead_king_color()
{
    for (const auto& piece : board.get_dead_pieces())
    {
        if (piece->get_label() == 'K')
            return White;
        if (piece->get_label() == 'k')
            return Black;
    }
    return std::nullopt;
}

bool GameManager::is_king_dead()
{
    return get_dead_king_color().has_value();
}

std::optional<int> GameManager::is_piece_promoting()
{
    for (const auto& piece : board.get_board_data())
    {
        if (piece != nullptr)
        {
            // Si la position est un entier entre 0 et 63
            int current_row = piece->get_position() / 8;

            if ((piece->get_label() == 'P') && (current_row == 0))
            {
                return piece->get_position();
            }
            else if ((piece->get_label() == 'p') && (current_row == 7))
            {
                return piece->get_position();
            }
        }
    }

    return std::nullopt;
}

void GameManager::promote_piece(int from_position, char promote_to)
{
    std::unique_ptr<Piece>& active_square = board.get_board_data()[from_position];

    if (active_square != nullptr)
    {
        // 1. On sauvegarde la couleur du pion avant de le détruire
        PieceColor piece_color = active_square->get_color();

        // 2. On calcule les coordonnées X et Y si votre constructeur les demande.
        int x = from_position % 8;
        int y = from_position / 8;

        // 3. On remplace le pointeur par la nouvelle pièce selon le choix du joueur
        switch (promote_to)
        {
        case 'Q':
        case 'q': active_square = std::make_unique<Queen>(x, y, piece_color); break;
        case 'R':
        case 'r': active_square = std::make_unique<Rook>(x, y, piece_color); break;
        case 'B':
        case 'b': active_square = std::make_unique<Bishop>(x, y, piece_color); break;
        case 'N':
        case 'n': active_square = std::make_unique<Knight>(x, y, piece_color); break;
        default:
            // Si l'entrée est invalide, on donne une Reine par défaut
            active_square = std::make_unique<Queen>(x, y, piece_color);
            break;
        }
    }
}

bool GameManager::is_player_move(const PieceColor& player_color) const
{
    return (player_color == White && is_white_turn())
           || (player_color == Black && !is_white_turn());
};

void GameManager::load_game_from_fen(const std::string& fen)
{
    std::string space        = " ";
    std::string positionData = fen.substr(0, fen.find(space)); // without castling and who to play
    // if it's black to play we start with an odd m_move
    if (fen.substr(fen.find(space) + 1, 1) == "b")
        m_move++;
    board.load_board_from_fen(positionData);
};

void GameManager::update_possible_moves(Piece*& current_square)
{
    if (current_square != nullptr)
    {
        possible_moves.clear();

        if (current_square->is_on_focus())
        {
            current_square->update_on_focus(false);
        }
        else
        {
            possible_moves = current_square->get_moves(board.get_board_data());
            current_square->update_on_focus(true);

            if (selected_square != nullptr)
            {
                selected_square->update_on_focus(false);
            }
        }

        selected_square = current_square;
    }
    else
    {
        if (selected_square != nullptr)
        {
            selected_square->update_on_focus(false);
        }
        possible_moves.clear();
    }
}

std::vector<int> GameManager::get_possible_moves()
{
    return possible_moves;
}
Piece* GameManager::get_selected_square()
{
    return selected_square;
}

void GameManager::clear_possible_moves()
{
    possible_moves.clear();
}

void GameManager::on_square_clicked(int i)
{
    // selected square is previously selected square and current_square is the one active just
    // now
    Piece* current_square = board.get_board_data(i).get();

    if (selected_square != nullptr)
    { // when we selected a piece

        // here we want to update the current square
        // only if it was eaten by the previous piece

        int old_position = selected_square->get_position();
        move_piece(old_position, i);
        if (old_position != selected_square->get_position())
        { // we check if position changed
            if (current_square != nullptr)
            { // if we select a piece we eat it with the previous
                current_square = selected_square;
            }
        }
        // we deselect everything after eating
        selected_square->update_on_focus(false);
        selected_square = nullptr;
        clear_possible_moves();
    }

    if (current_square != nullptr)
    {
        if ((current_square->get_color() == PieceColor::White) && is_white_turn()
                && !is_piece_promoting().has_value()
            || (current_square->get_color() == PieceColor::Black) && !is_white_turn()
                   && !is_piece_promoting().has_value())
        {
            update_possible_moves(current_square);
            selected_square = current_square;
        }
    }
    else
    {
        update_possible_moves(current_square);
    }
}

void GameManager::deselect_square()
{
    if (selected_square != nullptr)
    {
        selected_square->update_on_focus(false);
        selected_square = nullptr;
        clear_possible_moves();
    }
}

void GameManager::new_game(GameManager& game)
{
    clear_possible_moves();
    m_full_move = 0;
    m_move      = 0;
    m_move_history.clear();

    if (game.getMode() == GameMode::Classic)
    {
        load_game_from_fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    }

    if (game.getMode() == GameMode::Chaos)
    {
        Chess960Permutation generator;
        load_game_from_fen(generator.generateFromScratch());
    }
}