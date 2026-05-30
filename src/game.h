#pragma once

#include "board.hpp"
#include "bubble/include/bubble_color.hpp"
#include "include/game_settings.hpp"
class SDL2Renderer;  // Forward declaration

class Game {
public:

    Game();

    void printBoard(std::ostream& out) const;
    void loop(SDL2Renderer* renderer = nullptr);

private:
    bool isGameOver() const;
    bool hasWon() const;
    Bubble::ColorManager m_colorManager;
    Board m_board;
    std::size_t m_missCount{0};
};

