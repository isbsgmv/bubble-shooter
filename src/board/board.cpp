
#include "board.hpp"

#include <array>
#include <ostream>
#include <stdexcept>

namespace Random
{
    uint8_t Color(std::mt19937 &rng, size_t n_colors)
    {
        return static_cast<uint8_t>(1 + (rng() % (n_colors - 1)));
    }
} // namespace

Board::Board(std::size_t rows, std::size_t cols, std::size_t nColors, std::mt19937 &rng) : m_rows(rows),
                                                                                           m_cols(cols),
                                                                                           m_board(rows * cols, BubbleColor::None)
{
    // Fill the board with random colors
    for (std::size_t row = 0; row < m_rows; ++row)
    {
        for (std::size_t col = 0; col < m_cols; ++col)
        {
            m_board[index(row, col)] = static_cast<BubbleColor>(Random::Color(rng, nColors));
        }
    }
}

void Board::print(std::ostream &out) const
{
    out << "  ";
    for (std::size_t col = 0; col < m_cols; ++col)
    {
        out << col << ' ';
    }
    out << '\n';

    for (std::size_t row = 0; row < m_rows; ++row)
    {
        out << row << ' ';
        for (std::size_t col = 0; col < m_cols; ++col)
        {
            out << colorToChar(m_board[index(row, col)]) << ' ';
        }
        out << '\n';
    }
}

char Board::at(std::size_t row, std::size_t col) const
{
    if (row >= m_rows || col >= m_cols)
    {
        return '?';
    }

    return colorToChar(m_board[index(row, col)]);
}


std::size_t Board::index(std::size_t row, std::size_t col) const
{
    if (row >= m_rows || col >= m_cols)
    {
        throw std::out_of_range("Board index out of range");
    }

    return row * m_cols + col;
}

char Board::colorToChar(BubbleColor color)
{
    switch (color)
    {
    case BubbleColor::Red:
        return 'A';
    case BubbleColor::Green:
        return 'B';
    case BubbleColor::Blue:
        return 'C';
    case BubbleColor::Yellow:
        return 'D';
    case BubbleColor::Purple:
        return 'E';
    case BubbleColor::None:
    default:
        return '.';
    }
}
