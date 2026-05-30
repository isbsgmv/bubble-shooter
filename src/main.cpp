#include "game.h"
#include "renderer/include/sdl2_renderer.hpp"

#include <iostream>
#include <memory>

int main() {
    // Initialize SDL2 renderer
    auto renderer = std::make_unique<SDL2Renderer>();
    if (!renderer->init(500, 700, "Bubble Shooter")) {
        std::cerr << "Failed to initialize SDL2 renderer\n";
        return 1;
    }

    Game game;

    game.printBoard(std::cout);

    // Pass renderer to game loop
    game.loop(renderer.get());

    renderer->shutdown();
    return 0;
}
