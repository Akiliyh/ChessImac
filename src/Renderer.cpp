#include "Renderer.hpp"
#include <charconv>
#include <cstddef>
#include <iostream>
#include <string>
#include "Chessboard.hpp"
#include "Pieces.hpp"
#include "quick_imgui/quick_imgui.hpp"

void Renderer::draw(Chessboard& board)
{
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

                        std::string label {};
                        if (board.board_data[i] != nullptr) label=board.board_data[i]->m_label;

                        if (ImGui::Button(label.c_str(), ImVec2{50.f, 50.f}))
                        {
                            std::cout << "Clicked button " << i << "\n";
                        }

                        ImGui::PopStyleVar();
                        ImGui::PopID();
                        ImGui::PopStyleColor();

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