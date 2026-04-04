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

    quick_imgui::loop(
        "ChessImac",
        {
            .init =
                [&]() {
                    // renderer_3d.init();
                    window = glfwGetCurrentContext();
                },
            .loop =

                [&]() {
                    renderer_2d.draw(game);


                    int width  = 800;
                    int height = 800;
                    renderer_3d.draw(width, height, game);
                },

            .key_callback =
                [&](int key, int scancode, int action, int mods) {
                    if (key == GLFW_KEY_A && action == GLFW_PRESS)
                        glfwSetWindowShouldClose(window, GLFW_TRUE);
                    if (key == GLFW_KEY_Q && action == GLFW_PRESS)
                        renderer_3d.camera.moveFront(1.0f);
                    if (key == GLFW_KEY_H && action == GLFW_PRESS)
                        renderer_3d.camera.rotateLeft(1.0f);
                },
            .mouse_button_callback =
                [&](int button, int action, int mods) {
                    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
                    {
                        renderer_3d.is_panning = !renderer_3d.is_panning;
                    }
                    else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
                    {
                        renderer_3d.is_panning = false;
                    }
                },
            .cursor_position_callback =
                [&](double xpos, double ypos) {
                    const double deltaX = xpos - renderer_3d.lastX;
                    const double deltaY = ypos - renderer_3d.lastY;

                    const float intensity = 0.1f;

                    if (renderer_3d.is_panning)
                    {
                        renderer_3d.camera.rotateLeft(deltaX * intensity);
                        renderer_3d.camera.rotateUp(deltaY * intensity);
                    }

                    renderer_3d.lastX = xpos;
                    renderer_3d.lastY = ypos;
                },
            .scroll_callback = [&](double xoffset,
                                   double yoffset) { renderer_3d.camera.moveFront(-yoffset); },
        }

    );
}

void Renderer::drawStart(GameManager& game)
{
    quick_imgui::loop(
        "ChessImac",
        {
            .init = [&]() {},
            .loop =

                [&]() {
                    ImGui::ShowDemoWindow(); // This opens a window which shows tons of examples
                                             // of what you can do with ImGui. You should check
                                             // it out! Also, you can use the "Item Picker" in
                                             // the top menu of that demo window: then click on
                                             // any widget and it will show you the
                                             // corresponding code directly in your IDE!
                    ImGui::SetNextWindowSize({200, 200}, ImGuiCond_Once);
                    if (m_current_state == GameState::MainMenu)
                    {
                        if (ImGui::Button(("Normal Mode"), ImVec2{70.f, 50.f}))
                        {
                            m_current_state = GameState::NormalGame;
                        }
                        if (ImGui::Button(("Chaos Mode"), ImVec2{70.f, 50.f}))
                        {
                            m_current_state = GameState::NormalGame;
                        }
                        if (ImGui::Button("Exit Game", ImVec2{150.f, 50.f}))
                        {
                            std::exit(0);
                        }
                    }
                    else if (m_current_state == GameState::NormalGame
                             || m_current_state == GameState::ChaosGame)
                    {
                        if (ImGui::Button("Exit Game", ImVec2{150.f, 50.f}))
                        {
                            std::exit(0);
                        }
                    }
                },
        }
    );
}