#include <imgui.h>
#include "GameManager.hpp"
#include "Renderers/Renderer.hpp"

int main()
{
    Renderer    renderer{};
    GameManager game{};

    renderer.draw(game);
}