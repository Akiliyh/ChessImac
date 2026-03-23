#include "Renderer.hpp"
#include <imgui.h>
#include <algorithm>
#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include "Chessboard.hpp"
#include "GameManager.hpp"
#include "Pieces.hpp"
#include "quick_imgui/quick_imgui.hpp"

void Renderer::draw(GameManager& game)
{
    quick_imgui::loop(
        "ChessImac",
        {
            .init = [&]() {},
            .loop =

                [&]() {
                    ImGui::ShowDemoWindow(); // This opens a window which shows tons of examples of
                                             // what you can do with ImGui. You should check it out!
                                             // Also, you can use the "Item Picker" in the top menu
                                             // of that demo window: then click on any widget and it
                                             // will show you the corresponding code directly in
                                             // your IDE!
                    ImGui::SetNextWindowSize({200, 200}, ImGuiCond_Once);
                    ImGui::Begin("Play history");
                    ImGui::Text("%s", ("Move: " + std::to_string(game.get_full_move())).c_str());

                    const std::vector<int>& possible_moves  = game.get_possible_moves();
                    Piece*                  selected_square = game.get_selected_square();

                    if (game.is_white_turn())
                    {
                        ImGui::Text("%s", ("White to move"));
                    }
                    else
                    {
                        ImGui::Text("%s", ("Black to move"));
                    }

                    const auto& moves = game.get_move_history();
                    for (size_t i = 0; i < moves.size(); i++)
                    {
                        if (i % 2 == 0)
                        {
                            int full_move =
                                i / 2 + 1; // we don't want the full move to change everytime,
                                           // what we'll get with game.get_full_move

                            ImGui::Text("%s", ("." + std::to_string(full_move)).c_str());
                            ImGui::SameLine();
                        }
                        ImGui::Text("%c", (moves[i].first));
                        ImGui::SameLine();
                        ImGui::Text("%s", (moves[i].second).c_str());

                        if (i % 2 == 0)
                        {
                            ImGui::SameLine();
                        }
                    }

                    ImGui::End();

                    // we want to deselect everything if we right click

                    if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
                    {
                        game.deselect_square();
                    }

                    ImGui::Begin("Debug");

                    if (selected_square != nullptr)
                    {
                        ImGui::Text(
                            "%s", ("Previous position: "
                                   + game.board.to_alg_position(selected_square->get_position()))
                                      .c_str()
                        );
                    }

                    for (int i = 0; i < possible_moves.size(); i++)
                    {
                        ImGui::Text(
                            "%s", ("Possible moves: " + std::to_string(possible_moves[i])).c_str()
                        );
                    }

                    ImGui::End();

                    ImGui::Begin("ChessImac");

                    const int BOARD_SIZE{game.board.get_size()}; // 8x8

                    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));

                    for (int i = 0; i < BOARD_SIZE * BOARD_SIZE; i++)
                    {
                        int row = i / BOARD_SIZE;
                        int col = i % BOARD_SIZE;

                        auto it = std::find(possible_moves.begin(), possible_moves.end(), i);

                        // Check if we should show the border
                        bool should_border = (it != possible_moves.end());

                        if (should_border)
                        {
                            ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 5.0f);
                        }

                        bool isDark = (row + col) % 2 == 1;
                        if (isDark)
                        {
                            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{1.f, 0.f, 0.f, 1.f});
                        }
                        else
                        {
                            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{1.f, 0.5f, 0.f, 1.f});
                        }

                        ImGui::PushID(i);

                        std::string label{};
                        Piece*      current_square = game.board.get_board_data(i).get();

                        if (current_square != nullptr)
                        {
                            label = current_square->get_label();

                            if (current_square->get_color() == PieceColor::White)
                            {
                                ImGui::PushStyleColor(
                                    ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f)
                                );
                            }
                            else
                            {
                                ImGui::PushStyleColor(
                                    ImGuiCol_Text, ImVec4(0.0f, 0.0f, 0.0f, 1.0f)
                                );
                            }
                        }

                        if (ImGui::Button(label.c_str(), ImVec2{50.f, 50.f}))
                        {
                            std::cout << "Clicked button " << i << "\n";
                            game.on_square_clicked(i);
                        }

                        if (should_border)
                        {
                            ImGui::PopStyleVar();
                        }

                        if (current_square != nullptr)
                        {
                            ImGui::PopStyleColor(); // corresponds to the change of color of the
                                                    // button label
                        }

                        if ((i % BOARD_SIZE) != (BOARD_SIZE - 1))
                        {
                            ImGui::SameLine(0, 0); // we smash the tiles together
                        }
                        else
                        {
                            ImGui::SameLine(0, 10);

                            float textHeight = ImGui::CalcTextSize("1").y;
                            ImGui::SetCursorPosY(
                                ImGui::GetCursorPosY() + (50.0f - textHeight) * 0.5f
                            ); // we center vertically
                            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.f, 1.f, 1.f, 1.f));
                            ImGui::Text("%i", (BOARD_SIZE - row));
                            ImGui::PopStyleColor();
                        }

                        ImGui::PopID();
                        ImGui::PopStyleColor();
                    }

                    // col letters

                    ImGui::Dummy(ImVec2(0.f, 10.0f));

                    char  col_c     = 'a';
                    float tile_size = 50.f; // to later set to the square size

                    float text_width = ImGui::CalcTextSize("A").x;

                    float start_x = ImGui::GetCursorPosX() + (tile_size - text_width) / 2.0f;

                    for (int i = 0; i < BOARD_SIZE; i++)
                    {
                        ImGui::SetCursorPosX(
                            start_x + i * tile_size
                        ); // we offset to the enter of each square every col

                        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.f, 1.f, 1.f, 1.f));
                        ImGui::Text("%c", col_c + i); // we increment
                        ImGui::PopStyleColor();
                        ImGui::SameLine(0, 0);
                    }

                    ImGui::PopStyleVar();
                    ImGui::End();

                    if (game.is_king_dead())
                    {
                        ImGui::Begin("Game end");
                        if (game.get_dead_king_color() == White)
                        {
                            ImGui::Text("%s", ("Black win !"));
                        }
                        if (game.get_dead_king_color() == Black)
                        {
                            ImGui::Text("%s", ("White win !"));
                        }

                        if (ImGui::Button(("New Game"), ImVec2{70.f, 50.f}))
                        {
                            std::cout << "New Game button " << "\n";
                            game.new_game();
                        }

                        ImGui::SameLine(0, 5.0f);

                        if (ImGui::Button(("Exit Game"), ImVec2{70.f, 50.f}))
                        {
                            std::cout << "Exit Game button " << "\n";
                            std::exit(0);
                            // put glfwwindow close when we'll do 3d
                        }
                        ImGui::End();
                    }

                    std::optional<int> promoting_pawn = game.is_piece_promoting();

                    if (promoting_pawn.has_value())
                    {
                        int promote_pos = promoting_pawn.value();

                        ImGui::Begin("Promote Piece");

                        if (ImGui::Button(("Queen"), ImVec2{70.f, 50.f}))
                        {
                            game.promote_piece(promote_pos, 'Q');
                        }

                        ImGui::SameLine(0, 5.0f);

                        if (ImGui::Button(("Rook"), ImVec2{70.f, 50.f}))
                        {
                            game.promote_piece(promote_pos, 'R');
                        }

                        ImGui::SameLine(0, 5.0f);

                        if (ImGui::Button(("Bishop"), ImVec2{70.f, 50.f}))
                        {
                            game.promote_piece(promote_pos, 'B');
                        }

                        ImGui::SameLine(0, 5.0f);

                        if (ImGui::Button(("Knight"), ImVec2{70.f, 50.f}))
                        {
                            game.promote_piece(promote_pos, 'N');
                        }

                        ImGui::End();
                    }
                },
        }
    );
}