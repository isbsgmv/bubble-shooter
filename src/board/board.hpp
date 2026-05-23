
#pragma once

#include <cstddef>
#include <cstdint>
#include <iosfwd>
#include <random>
#include <vector>

enum class BubbleColor : std::uint8_t
{
    None,
    Red,
    Green,
    Blue,
    Yellow,
    Purple
};

class Board
{
public:
    Board(std::size_t rows, std::size_t cols, std::size_t nColors, std::mt19937 &rng);

    void print(std::ostream &out) const;

private:
    char at(std::size_t row, std::size_t col) const;
    std::size_t index(std::size_t row, std::size_t col) const;
    static char colorToChar(BubbleColor color);

    std::size_t m_rows;
    std::size_t m_cols;
    std::vector<BubbleColor> m_board;
};