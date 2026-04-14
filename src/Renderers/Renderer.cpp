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
                    m_renderer_2d.draw(game);

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
                    ImGui::Checkbox("Second Light", &m_renderer_3d.is_second_light_active);
                    ImGui::Checkbox("Skybox", &m_renderer_3d.is_skybox_active);
                    ImGui::End();

                    m_renderer_3d.draw(width, height, game);
                },

            .key_callback =
                [&](int key, int scancode, int action, int mods) {
                    if (key == GLFW_KEY_A && action == GLFW_PRESS)
                        glfwSetWindowShouldClose(window, GLFW_TRUE);
                    if (key == GLFW_KEY_Q && action == GLFW_PRESS)
                        m_renderer_3d.camera.moveFront(0.5f);
                    if (key == GLFW_KEY_H && action == GLFW_PRESS)
                        m_renderer_3d.camera.rotateLeft(1.0f);
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
                        m_renderer_3d.camera.rotateLeft(deltaX * intensity);
                        m_renderer_3d.camera.rotateUp(deltaY * intensity);
                    }

                    m_renderer_3d.lastX = xpos;
                    m_renderer_3d.lastY = ypos;
                },
            .scroll_callback = [&](
                                   double xoffset, double yoffset
                               ) { m_renderer_3d.camera.moveFront(-(yoffset / 5.0)); },
        }

    );
    m_renderer_3d.terminate(); // here we debind everything
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