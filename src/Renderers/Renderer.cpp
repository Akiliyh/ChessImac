#include "Renderer.hpp"
#include <imgui.h>
#include <algorithm>
#include <array>
#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include "2DRenderer.hpp"
#include "3DRenderer.hpp"
#include "Camera.hpp"
#include "Chessboard.hpp"
#include "GLFW/glfw3.h"
#include "GameManager.hpp"
#include "Pieces.hpp"
#include "quick_imgui/quick_imgui.hpp"

void Renderer::draw(GameManager& game)
{
    GLFWwindow* window = nullptr;
    float       width{800.0f};
    float       height{800.0f};

    quick_imgui::loop(
        "ChessImac",
        {
            .init =
                [&]() {
                    m_renderer_3d.init(width, height);
                    window = glfwGetCurrentContext();
                },
            .loop =

                [&]() {
                    if (game.get_mode() == GameMode::Chaos && !game.is_paused())
                    {
                        if (game.is_time_over())
                        {
                            game.skip_turn();
                            game.trigger_skip_popup();
                        }
                        game.update_random_spawns();
                    }

                    m_renderer_2d.draw(game);

                    // ==========================================
                    // Modes Modal
                    // ==========================================

                    ImGui::Begin("Game Settings");
                    ImGui::Text("Select a Game Mode :");

                    if (ImGui::Button("Classic Mode"))
                    {
                        game.setMode(GameMode::Classic);
                        game.new_game(game);
                    }
                    ImGui::SameLine();
                    if (ImGui::Button("Chaos Mode"))
                    {
                        game.setMode(GameMode::Chaos);
                        game.new_game(game);
                    }

                    if (ImGui::Button("Exit Game", ImVec2(178, 0)))
                    {
                        std::exit(0);
                    }

                    ImGui::Separator();
                    ImGui::Text(
                        "Current Mode : %s",
                        (game.get_mode() == GameMode::Classic) ? "Classic" : "Chaos"
                    );

                    // ==========================================
                    // Chrono & Time Limit
                    // ==========================================
                    ImGui::Separator();

                    ImGui::Text("Turn: %s", game.is_white_turn() ? "White" : "Black");

                    double elapsed = game.get_current_turn_elapsed_time();
                    ImGui::Text("Thinking time : %.1f s", elapsed);

                    if (game.get_mode() == GameMode::Chaos)
                    {
                        double current_limit = game.get_current_turn_limit();
                        double remaining     = std::max(0.0, current_limit - elapsed);
                        auto   progress      = (float)(remaining / current_limit);

                        bool color_pushed = false;
                        if (game.is_paused())
                        {
                            ImGui::PushStyleColor(
                                ImGuiCol_PlotHistogram, ImVec4(0.5f, 0.5f, 0.5f, 1.0f)
                            );
                            color_pushed = true;
                        }
                        else if (remaining < 3.0)
                        {
                            ImGui::PushStyleColor(
                                ImGuiCol_PlotHistogram, ImVec4(1.0f, 0.0f, 0.0f, 1.0f)
                            );
                            color_pushed = true;
                        }

                        ImGui::Text("Time left : %.1f s", remaining);
                        ImGui::ProgressBar(progress, ImVec2(-1.0f, 0.0f), "");

                        if (color_pushed)
                        {
                            ImGui::PopStyleColor();
                        }

                        if (game.is_paused())
                        {
                            ImGui::Text("GAME IN PAUSE");
                        }
                        else if (remaining < 3.0)
                        {
                            ImGui::TextColored(ImVec4(1, 0, 0, 1), "HURRY !");
                        }

                        ImGui::Separator();

                        if (ImGui::Button(game.is_paused() ? "RESUME" : "PAUSE", ImVec2(178, 0)))
                        {
                            game.toggle_pause();
                        }
                    }

                    ImGui::End();

                    // ==========================================

                    // ==========================================
                    // Mutation Pop-up
                    // ==========================================

                    if (game.get_show_mutation_popup())
                    {
                        ImGui::OpenPopup("Random Mutation !");
                        game.set_show_mutation_popup(false);
                        game.start_popup_timer(2.5);
                        m_renderer_3d.is_panning = false;
                    }

                    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
                    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

                    if (ImGui::BeginPopupModal(
                            "Random Mutation !", NULL, ImGuiWindowFlags_AlwaysAutoResize
                        ))
                    {
                        ImGui::Text("%s", game.get_mutation_message().c_str());
                        ImGui::Separator();

                        std::array<char, 32> button_text;
                        double               remaining = game.get_popup_remaining_time();

                        std::snprintf(
                            button_text.data(), button_text.size(), "OK (%.1fs)###MutBtn", remaining
                        );

                        if (ImGui::Button(button_text.data(), ImVec2(120, 0))
                            || game.is_popup_time_over())
                        {
                            game.toggle_pause();
                            ImGui::CloseCurrentPopup();
                        }

                        ImGui::SetItemDefaultFocus();
                        ImGui::EndPopup();
                    }
                    // ==========================================

                    // ==========================================
                    // Dodge Attack Pop-up
                    // ==========================================

                    if (game.get_show_dodge_popup())
                    {
                        ImGui::OpenPopup("Attack Missed !");
                        game.set_show_dodge_popup(false);
                        game.start_popup_timer(2.5);
                        m_renderer_3d.is_panning = false;
                    }

                    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

                    if (ImGui::BeginPopupModal(
                            "Attack Missed !", nullptr, ImGuiWindowFlags_AlwaysAutoResize
                        ))
                    {
                        ImGui::Text("%s", game.get_dodge_message().c_str());
                        ImGui::Separator();

                        std::array<char, 32> button_text;
                        double               remaining = game.get_popup_remaining_time();

                        // On met à jour le texte du bouton avec le temps restant
                        std::snprintf(
                            button_text.data(), button_text.size(), "Oh okay...(%.1fs)###DodgeBtn",
                            remaining
                        );

                        if (ImGui::Button(button_text.data(), ImVec2(120, 0))
                            || game.is_popup_time_over())
                        {
                            game.toggle_pause();
                            ImGui::CloseCurrentPopup();
                        }

                        ImGui::SetItemDefaultFocus();
                        ImGui::EndPopup();
                    }

                    // ==========================================
                    // Skip Turn Pop-up
                    // ==========================================

                    if (game.get_show_skip_popup())
                    {
                        ImGui::OpenPopup("Time's Up !");
                        game.set_show_skip_popup(false);
                        game.start_popup_timer(2.0);
                        m_renderer_3d.is_panning = false;
                    }

                    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

                    if (ImGui::BeginPopupModal(
                            "Time's Up !", nullptr, ImGuiWindowFlags_AlwaysAutoResize
                        ))
                    {
                        ImGui::Text("Too late. Turn skipped !");
                        ImGui::Separator();

                        std::array<char, 32> button_text;
                        double               remaining = game.get_popup_remaining_time();

                        std::snprintf(
                            button_text.data(), button_text.size(), "No ! (%.1fs)###SkipBtn",
                            remaining
                        );

                        if (ImGui::Button(button_text.data(), ImVec2(120, 0))
                            || game.is_popup_time_over())
                        {
                            game.toggle_pause();
                            ImGui::CloseCurrentPopup();
                        }

                        ImGui::SetItemDefaultFocus();
                        ImGui::EndPopup();
                    }
                    // ==========================================

                    // ==========================================
                    // Pawn Spawn Pop-up
                    // ==========================================

                    if (game.get_show_spawn_popup())
                    {
                        ImGui::OpenPopup("New challenger !");
                        game.set_show_spawn_popup(false);
                        game.start_popup_timer(1.5);
                        m_renderer_3d.is_panning = false;
                    }

                    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

                    if (ImGui::BeginPopupModal(
                            "New challenger !", nullptr, ImGuiWindowFlags_AlwaysAutoResize
                        ))
                    {
                        ImGui::Text("Pawn joins the battle !");
                        ImGui::Separator();

                        std::array<char, 32> button_text;
                        double               remaining = game.get_popup_remaining_time();

                        std::snprintf(
                            button_text.data(), button_text.size(), "Wow ! (%.1fs)###SpawnBtn",
                            remaining
                        );

                        if (ImGui::Button(button_text.data(), ImVec2(120, 0))
                            || game.is_popup_time_over())
                        {
                            game.toggle_pause();
                            ImGui::CloseCurrentPopup();
                        }

                        ImGui::SetItemDefaultFocus();
                        ImGui::EndPopup();
                    }
                    // ==========================================

                    // ==========================================
                    // Game Over Pop-up
                    // ==========================================

                    if (game.is_king_dead())
                    {
                        ImGui::OpenPopup("Game Over !");

                        // On fige le chronomètre de fin de partie
                        if (!game.is_paused())
                        {
                            game.toggle_pause();
                        }
                        m_renderer_3d.is_panning = false;
                    }

                    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

                    if (ImGui::BeginPopupModal(
                            "Game Over !", NULL, ImGuiWindowFlags_AlwaysAutoResize
                        ))
                    {
                        if (game.get_dead_king_color() == PieceColor::White)
                        {
                            ImGui::Text("Black win !");
                        }
                        else if (game.get_dead_king_color() == PieceColor::Black)
                        {
                            ImGui::Text("White win !");
                        }

                        ImGui::Separator();

                        if (ImGui::Button("New Game", ImVec2(100, 0)))
                        {
                            game.new_game(game);

                            if (game.is_paused())
                            {
                                game.toggle_pause();
                            }

                            ImGui::CloseCurrentPopup();
                        }

                        ImGui::SameLine();

                        if (ImGui::Button("Exit Game", ImVec2(100, 0)))
                        {
                            std::exit(0);
                        }

                        ImGui::SetItemDefaultFocus();
                        ImGui::EndPopup();
                    }
                    // ==========================================

                    ImGui::Begin("3D Controls");
                    ImGui::Text("Camera");
                    ImGui::SliderFloat("FOV", &m_renderer_3d.fov, 10.f, 100.0f);
                    ImGui::SliderFloat("Light Postion x", &m_renderer_3d.light_pos.x, -10.f, 10.0f);
                    ImGui::SliderFloat("Light Postion y", &m_renderer_3d.light_pos.y, -10.f, 10.0f);
                    ImGui::SliderFloat("Light Postion z", &m_renderer_3d.light_pos.z, -10.f, 10.0f);

                    ImGui::PushItemWidth(100);
                    ImGui::ColorPicker3("Light Color", &m_renderer_3d.light_color[0]);
                    ImGui::PopItemWidth();

                    ImGui::Checkbox("Panning", &m_renderer_3d.is_panning);
                    ImGui::Checkbox("Skybox", &m_renderer_3d.is_skybox_active);
                    if (ImGui::Checkbox("Trackball camera", &m_renderer_3d.use_trackball_camera))
                    {
                        m_renderer_3d.change_camera();
                    }
                    ImGui::End();

                    ImVec2 size = ImGui::GetMainViewport()->Size;
                    m_renderer_3d.resize_window(
                        static_cast<float>(size.x), static_cast<float>(size.y)
                    );
                    m_renderer_3d.draw(
                        static_cast<float>(size.x), static_cast<float>(size.y), game
                    );
                },

            .key_callback =
                [&](int key, int scancode, int action, int mods) {
                    if (key == GLFW_KEY_A && action == GLFW_PRESS)
                        glfwSetWindowShouldClose(window, GLFW_TRUE);
                    if (key == GLFW_KEY_Q && action == GLFW_PRESS)
                        m_renderer_3d.camera->moveFront(0.5f);
                    if (key == GLFW_KEY_H && action == GLFW_PRESS)
                        m_renderer_3d.camera->rotateLeft(1.0f);
                    if (key == GLFW_KEY_L && action == GLFW_PRESS)
                        m_renderer_3d.use_trackball_camera = !m_renderer_3d.use_trackball_camera;
                    m_renderer_3d.change_camera();
                },
            .mouse_button_callback =
                [&](int button, int action, int mods) {
                    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
                    {
                        m_renderer_3d.is_panning = !m_renderer_3d.is_panning;
                    }
                    else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
                    {
                        m_renderer_3d.is_panning = false;
                    }

                    double mouseX, mouseY;
                    glfwGetCursorPos(window, &mouseX, &mouseY);

                    int tile = m_renderer_3d.click_square(mouseX, mouseY);

                    if (tile != -1)
                    {
                        if (m_renderer_3d.use_trackball_camera)
                        {
                            game.on_square_clicked(tile, game);
                        }
                    }
                },
            .cursor_position_callback =
                [&](double xpos, double ypos) {
                    const double deltaX = xpos - m_renderer_3d.lastX;
                    const double deltaY = ypos - m_renderer_3d.lastY;

                    const float intensity = 0.1f;

                    if (m_renderer_3d.is_panning)
                    {
                        m_renderer_3d.camera->rotateLeft(deltaX * intensity);
                        m_renderer_3d.camera->rotateUp(deltaY * intensity);
                    }

                    m_renderer_3d.lastX = xpos;
                    m_renderer_3d.lastY = ypos;
                },
            .scroll_callback = [&](
                                   double xoffset, double yoffset
                               ) { m_renderer_3d.camera->moveFront(-(yoffset / 5.0)); },
        }

    );
    m_renderer_3d.terminate(); // here we debind everything
}