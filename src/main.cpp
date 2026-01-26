#include <imgui.h>
#include <iostream>
#include <string>
#include "Renderer.hpp"
#include "Chessboard.hpp"

int main()
{
    float value{0.f};
    Renderer renderer{};
    Chessboard board{};
    renderer.draw(board);
}