#include "GameManager.hpp"
#include <iostream>
#include <optional>
#include <vector>
#include "Chessboard.hpp"
#include "Pieces.hpp"
#include "Probabilities/Chess960Permutation.hpp"

GameManager::GameManager() : m_fish_law(0.20) // Occurences by "tick" %
{
    load_game_from_fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    m_last_spawn_check_time = std::chrono::steady_clock::now();
}

void GameManager::move_piece(int from_position, int dest_position, GameManager& game)
{
    std::unique_ptr<Piece>& active_square = board.get_board_data()[from_position];
    if (is_player_move(active_square->get_color()) && !is_piece_promoting().has_value())
    {
        bool piece_moved = board.move_piece(active_square, dest_position, game);
        if (piece_moved)
        {
            add_move();
            display_full_move();

            std::string dest_alg_position = board.to_alg_position(dest_position);

            // we add this to the move history
            m_move_history.emplace_back(
                board.get_board_data()[dest_position]->get_label(), dest_alg_position
            );
            m_last_move = Move{.from = from_position, .to = dest_position};
        }
    }
};

int GameManager::get_move() const
{
    return m_move;
};

std::optional<Move> GameManager::get_last_move() const
{
    return m_last_move;
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
            // Is position between 0 and 63 ?
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
        PieceColor piece_color = active_square->get_color();

        int x = from_position % 8;
        int y = from_position / 8;

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
        default: active_square = std::make_unique<Queen>(x, y, piece_color); break;
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

void GameManager::on_square_clicked(int i, GameManager& game)
{
    // selected square is previously selected square and current_square is the one active just
    // now
    Piece* current_square = board.get_board_data(i).get();

    if (selected_square != nullptr)
    { // when we selected a piece

        // here we want to update the current square
        // only if it was eaten by the previous piece

        int old_position = selected_square->get_position();
        move_piece(old_position, i, game);
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
    reset_turn_timer();

    if (game.get_mode() == GameMode::Classic)
    {
        load_game_from_fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    }

    if (game.get_mode() == GameMode::Chaos)
    {
        Chess960Permutation generator;
        load_game_from_fen(generator.generate_scratch());
    }
}

std::string GameManager::get_mutation_message()
{
    return m_mutation_message;
}

bool GameManager::get_show_mutation_popup() const
{
    return m_show_mutation_popup;
}

void GameManager::set_show_mutation_popup(bool is_mutation_showing)
{
    m_show_mutation_popup = is_mutation_showing;
};

void GameManager::trigger_mutation_popup(const std::string& message)
{
    m_mutation_message    = message;
    m_show_mutation_popup = true;
}

void GameManager::trigger_dodge_popup(const std::string& message)
{
    m_dodge_message    = message;
    m_show_dodge_popup = true;
};

void GameManager::set_show_dodge_popup(bool is_dodge_showing)
{
    m_show_dodge_popup = is_dodge_showing;
};

bool GameManager::get_show_dodge_popup() const
{
    return m_show_dodge_popup;
};

std::string GameManager::get_dodge_message()
{
    return m_dodge_message;
};

void GameManager::trigger_skip_popup()
{
    m_show_skip_popup = true;
}
void GameManager::set_show_skip_popup(bool is_skip_showing)
{
    m_show_skip_popup = is_skip_showing;
}
bool GameManager::get_show_skip_popup() const
{
    return m_show_skip_popup;
}

void GameManager::trigger_spawn_popup()
{
    m_show_spawn_popup = true;
}
void GameManager::set_show_spawn_popup(bool is_spawn_showing)
{
    m_show_spawn_popup = is_spawn_showing;
}
bool GameManager::get_show_spawn_popup() const
{
    return m_show_spawn_popup;
}

void GameManager::reset_turn_timer()
{
    m_turn_start_time  = std::chrono::steady_clock::now();
    m_accumulated_time = 0.0;
    m_is_paused        = false;

    if (m_current_mode == GameMode::Chaos)
    {
        m_current_turn_limit = m_expo_law.get_bounded_time(10.0, 30.0, 0.5);
    }
}

void GameManager::toggle_pause()
{
    if (!m_is_paused)
    {
        m_accumulated_time = get_current_turn_elapsed_time();
    }
    else
    {
        m_turn_start_time = std::chrono::steady_clock::now();
    }
    m_is_paused = !m_is_paused;
}

double GameManager::get_current_turn_elapsed_time() const
{
    if (m_is_paused)
    {
        return m_accumulated_time;
    }

    auto                          now     = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed = now - m_turn_start_time;
    return m_accumulated_time + elapsed.count();
}

bool GameManager::is_time_over() const
{
    return get_current_turn_elapsed_time() >= m_current_turn_limit;
}

void GameManager::skip_turn()
{
    m_move++;

    if (m_move % 2 == 0)
    {
        m_full_move++;
    }

    reset_turn_timer();
}

void GameManager::start_popup_timer(double duration_in_seconds)
{
    m_popup_duration   = duration_in_seconds;
    m_popup_start_time = std::chrono::steady_clock::now();

    if (!m_is_paused)
    {
        toggle_pause();
    }
}

double GameManager::get_popup_remaining_time() const
{
    auto                          now     = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed = now - m_popup_start_time;

    return std::max(0.0, m_popup_duration - elapsed.count());
}

bool GameManager::is_popup_time_over() const
{
    return get_popup_remaining_time() <= 0.0;
}

void GameManager::update_random_spawns()
{
    if (m_current_mode != GameMode::Chaos || m_is_paused)
    {
        return;
    }

    auto now = std::chrono::steady_clock::now();

    std::chrono::duration<double> elapsed = now - m_last_spawn_check_time;

    if (elapsed.count() >= m_spawn_check_interval)
    {
        m_last_spawn_check_time = now;

        // Dice with FisherLaw
        int pawns_to_spawn = m_fish_law.generate();

        // If FisherLaw gives 1, 2, 3... spawn Pawn
        for (int i = 0; i < pawns_to_spawn; ++i)
        {
            spawn_random_pawn();
        }
    }
}

void GameManager::spawn_random_pawn()
{
    std::vector<int> empty_squares;

    for (int i = 0; i < 64; ++i)
    {
        if (board.get_board_data(i) == nullptr)
        {
            empty_squares.push_back(i);
        }
    }

    if (empty_squares.empty())
    {
        return;
    }

    // 2. Random case
    std::random_device              rd;
    std::mt19937                    gen(rd());
    std::uniform_int_distribution<> dist(0, empty_squares.size() - 1);

    int random_index  = dist(gen);
    int target_square = empty_squares[random_index];

    // 3. Who is playing ?
    PieceColor current_color = is_white_turn() ? PieceColor::White : PieceColor::Black;

    int dest_x = target_square % board.get_size();
    int dest_y = target_square / board.get_size();

    board.get_board_data(target_square) = std::make_unique<Pawn>(dest_x, dest_y, current_color);

    trigger_spawn_popup();
}