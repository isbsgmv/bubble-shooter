#ifndef GAME_H
#define GAME_H

#include "board.hpp"
#include "bubble/include/bubble_color.hpp"


class Game {
public:
    static constexpr std::size_t kRows = 8;
    static constexpr std::size_t kCols = 8;

    Game();

    void printBoard(std::ostream& out) const;
    void loop();

private:
    Board m_board;
    Bubble::ColorManager m_colorManager;
};

#endif
