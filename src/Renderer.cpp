#include "Renderer.hpp"
#include <algorithm>
#include <charconv>
#include <cstddef>
#include <iostream>
#include <string>
#include <vector>
#include "Chessboard.hpp"
#include "Pieces.hpp"
#include "quick_imgui/quick_imgui.hpp"

void Renderer::display_possible_moves(Chessboard& board, Piece* current_square, Piece* previous_square, std::vector<int>& possible_moves)
{
    if (current_square != nullptr)
    {
        if (current_square->on_focus)
        {
            current_square->on_focus = false;
            possible_moves.clear();
        }
        else
        {
            possible_moves.clear();
            possible_moves = current_square->get_moves(board.board_data);
            for (int possible_move : possible_moves)
            {
                std::cout << possible_move << '\n';
            }
            current_square->on_focus = true;

            if (previous_square != nullptr)
            {
                previous_square->on_focus = false;
            }
        }

        previous_square = current_square;
    }
    else
    {
        if (previous_square != nullptr)
        {
            previous_square->on_focus = false;
        }
        possible_moves.clear();
    }
}

void Renderer::draw(Chessboard& board)
{
    std::vector<int> possible_moves{};
    Piece*           previous_square{};

    quick_imgui::loop(
        "ChessImac",
        {
            .init = [&]() {},
            .loop =

                [&]() {
                    ImGui::ShowDemoWindow(); // This opens a window which shows tons of examples of what you can do with ImGui. You should check it out! Also, you can use the "Item Picker" in the top menu of that demo window: then click on any widget and it will show you the corresponding code directly in your IDE!

                    ImGui::Begin("ChessImac");

                    const int BOARD_SIZE{board.get_size()}; // 8x8

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
                        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));

                        std::string label{};
                        Piece*      current_square{board.board_data[i]};

                        if (current_square != nullptr)
                        {
                            label = current_square->m_label;

                            if (current_square->m_color == PieceColor::White)
                            {
                                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
                            }
                            else
                            {
                                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
                            }
                        }

                        if (ImGui::Button(label.c_str(), ImVec2{50.f, 50.f}))
                        {
                            std::cout << "Clicked button " << i << "\n";

                            Renderer::display_possible_moves(board, current_square, previous_square, possible_moves);
                        }

                        // ImGui::PushStyleColor(ImGuiCol_Text, <your color>);
                        // ImGui::LabelText("##LabelID", Text_to_Display, Text_Size);
                        // ImGui::PopStyleColor();

                        if (should_border)
                        {
                            ImGui::PopStyleVar();
                        }

                        ImGui::PopStyleVar();
                        ImGui::PopID();
                        ImGui::PopStyleColor();

                        if (current_square != nullptr) {
                            ImGui::PopStyleColor(); // corresponds to the change of color of the button label
                        }

                        if ((i % BOARD_SIZE) != (BOARD_SIZE - 1))
                        {
                            ImGui::SameLine(0, 0); // we smash the tiles together
                        }
                    }

                    ImGui::End();
                },
        }
    );
}