#include "Renderer.hpp"
#include <imgui.h>
#include <algorithm>
#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include "2DRenderer.hpp"
#include "3DRenderer.hpp"
#include "Chessboard.hpp"
#include "GLFW/glfw3.h"
#include "GameManager.hpp"
#include "Pieces.hpp"
#include "quick_imgui/quick_imgui.hpp"

void Renderer::draw(GameManager& game)
{
    GLFWwindow* window = nullptr;
    const int   width{800};
    const int   height{800};

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
                        }
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
                        double remaining = std::max(0.0, GameManager::TURN_TIME_LIMIT - elapsed);
                        auto   progress  = (float)(remaining / GameManager::TURN_TIME_LIMIT);

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
                        ImGui::ProgressBar(progress, ImVec2(-1.0f, 0.0f));

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

                        if (ImGui::Button(game.is_paused() ? "RESUME" : "PAUSE"))
                        {
                            game.toggle_pause();
                        }
                    }

                    ImGui::End();

                    // ==========================================
                    // Mutation Pop-up
                    // ==========================================

                    if (game.get_show_mutation_popup())
                    {
                        ImGui::OpenPopup("Random Mutation !");
                        game.set_show_mutation_popup(false);
                    }

                    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
                    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

                    if (ImGui::BeginPopupModal(
                            "Random Mutation !", NULL, ImGuiWindowFlags_AlwaysAutoResize
                        ))
                    {
                        ImGui::Text("%s", game.get_mutation_message().c_str());
                        ImGui::Separator();

                        if (ImGui::Button("OK", ImVec2(120, 0)))
                        {
                            ImGui::CloseCurrentPopup();
                        }

                        ImGui::SetItemDefaultFocus();
                        ImGui::EndPopup();
                    }

                    // ==========================================
                    // Dodge Attack Pop-up
                    // ==========================================

                    if (game.get_show_dodge_popup())
                    {
                        ImGui::OpenPopup("Attack Missed !");
                        game.set_show_dodge_popup(false);
                    }

                    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

                    if (ImGui::BeginPopupModal(
                            "Attack Missed !", NULL, ImGuiWindowFlags_AlwaysAutoResize
                        ))
                    {
                        ImGui::Text("%s", game.get_dodge_message().c_str());
                        ImGui::Separator();

                        if (ImGui::Button("Oh okay...", ImVec2(120, 0)))
                        {
                            ImGui::CloseCurrentPopup();
                        }

                        ImGui::SetItemDefaultFocus();
                        ImGui::EndPopup();
                    }

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

                    m_renderer_3d.draw(width, height, game);
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