#ifndef GAME_H
#define GAME_H

#include "board/board.hpp"

#include <cstddef>
#include <iosfwd>
#include <random>

class Game {
public:
    static constexpr std::size_t kRows = 8;
    static constexpr std::size_t kCols = 8;
    static constexpr std::size_t kNColors= 5;

    Game();

    void printBoard(std::ostream& out) const;

private:
    Board m_board;
    std::mt19937 m_rng;
};

#endif
