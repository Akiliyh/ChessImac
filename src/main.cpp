#include <imgui.h>
#include "GameManager.hpp"
#include "Renderers/Renderer.hpp"

int main()
{
    Renderer    renderer{};
    GameManager game{};

    game.reset_turn_timer();
    renderer.draw(game);
}