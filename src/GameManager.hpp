#pragma once

#include <chrono>
#include <optional>
#include <utility>
#include <vector>
#include "Chessboard.hpp"
#include "Pieces.hpp"
#include "Probabilities/ExpoLaw.hpp"

enum class GameMode { Classic, Chaos };

// this for the animation we want to lerp between the previous position in the 1d array of the board
struct Move {
    int from;
    int to;
};

struct GameManager {
  private:
    std::optional<Move> m_last_move;
    int                 m_full_move{0}; // full move is both color played capice? mamma miaaa
    int                 m_move{0};
    std::vector<std::pair<char, std::string>>
        m_move_history{}; // we store the type of piece and the square of destination
    // maybe later we could store if it is a capture, promotion or check eventually

    Piece*           selected_square{nullptr};
    Piece*           selectable_square{nullptr};
    std::vector<int> possible_moves;

    GameMode m_current_mode{GameMode::Classic};

    std::string m_mutation_message{""};
    bool        m_show_mutation_popup{false};

    std::string m_dodge_message{""};
    bool        m_show_dodge_popup{false};

    bool m_show_skip_popup{false};

    std::chrono::steady_clock::time_point m_turn_start_time;
    bool                                  m_is_paused{false};
    double                                m_accumulated_time{0.0};
    ExpoLaw                               m_expo_law;
    double                                m_current_turn_limit{10.0};

    std::chrono::steady_clock::time_point m_popup_start_time;
    double                                m_popup_duration{0.0};

  public:
    Chessboard                                board{};
    int                                       get_board() const;
    int                                       get_full_move() const;
    void                                      add_move();
    int                                       get_move() const;
    std::vector<std::pair<char, std::string>> get_move_history() const;
    void                                      delete_move_history();
    void                                      display_full_move() const;
    void                                      display_who_is_next(PieceColor player_color) const;
    bool                                      is_white_turn() const;
    bool                                      is_player_move(const PieceColor& player_color) const;
    void                      move_piece(int from_position, int dest_position, GameManager& game);
    std::optional<PieceColor> get_dead_king_color();
    bool                      is_king_dead();
    std::optional<int>        is_piece_promoting();
    void                      promote_piece(int from_position, char promote_to);
    std::optional<Move>       get_last_move() const;

    void load_game_from_fen(const std::string& fen);

    void             play_game();
    void             update_possible_moves(Piece*& current_square);
    std::vector<int> get_possible_moves();
    Piece*           get_selected_square();
    void             clear_possible_moves();
    void             on_square_clicked(int i, GameManager& game);
    void             deselect_square();

    void new_game(GameManager& game);

    GameManager()
    {
        load_game_from_fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    }

    void setMode(GameMode mode)
    {
        m_current_mode = mode;
    }
    GameMode get_mode() const
    {
        return m_current_mode;
    }

    void        trigger_mutation_popup(const std::string& message);
    void        set_show_mutation_popup(bool is_mutation_showing);
    bool        get_show_mutation_popup() const;
    std::string get_mutation_message();

    void        trigger_dodge_popup(const std::string& message);
    void        set_show_dodge_popup(bool is_dodge_showing);
    bool        get_show_dodge_popup() const;
    std::string get_dodge_message();

    void trigger_skip_popup();
    void set_show_skip_popup(bool is_skip_showing);
    bool get_show_skip_popup() const;

    static constexpr double TURN_TIME_LIMIT = 20.0;
    void                    skip_turn();
    bool                    is_time_over() const;
    void                    reset_turn_timer();
    double                  get_current_turn_elapsed_time() const;
    void                    toggle_pause();
    double                  get_current_turn_limit() const
    {
        return m_current_turn_limit;
    }
    bool is_paused() const
    {
        return m_is_paused;
    }

    void   start_popup_timer(double duration_in_seconds);
    double get_popup_remaining_time() const;
    bool   is_popup_time_over() const;
};