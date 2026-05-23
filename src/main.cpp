#include "game.h"

#include <iostream>

int main() {
    Game game;

    std::cout << "Simple Candy Crush CLI (Phase 1-2)\n";
    std::cout << "Initial board:\n";
    game.printBoard(std::cout);

    return 0;
}
