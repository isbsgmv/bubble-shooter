#include "board.hpp"

#include <algorithm>
#include <deque>
#include <ostream>
#include <stdexcept>

Board::Board(std::size_t rows, std::size_t cols, Bubble::ColorManager &colorManager) : m_rows(rows),
                                                                                      m_cols(cols),
                                                                                      m_board(rows * cols, Bubble::Color::None),
                                                                                      m_colorManager(colorManager)
{
    // Only the top rows so projectiles have room to travel.
    const std::size_t init_filled_rows = std::min<std::size_t>(3, m_rows);

    for (std::size_t row = 0; row < init_filled_rows; ++row)
    {
        for (std::size_t col = 0; col < m_cols; ++col)
        {
            m_board[index(row, col)] = m_colorManager.randomColor();
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
        if ((row % 2) != 0)
        {
            out << ' ';
        }

        out << row << ' ';
        for (std::size_t col = 0; col < m_cols; ++col)
        {
            out << colorToChar(m_board[index(row, col)]) << ' ';
        }
        out << '\n';
    }
}

std::size_t Board::rows() const noexcept
{
    return m_rows;
}

std::size_t Board::cols() const noexcept
{
    return m_cols;
}

bool Board::inBounds(int row, int col) const noexcept
{
    return row >= 0 && col >= 0 && static_cast<std::size_t>(row) < m_rows && static_cast<std::size_t>(col) < m_cols;
}

bool Board::isEmpty(int row, int col) const noexcept
{
    if (!inBounds(row, col))
    {
        return false;
    }

    return m_board[index(static_cast<std::size_t>(row), static_cast<std::size_t>(col))] == Bubble::Color::None;
}

Bubble::Color Board::get(std::size_t row, std::size_t col) const
{
    return m_board[index(row, col)];
}

void Board::set(std::size_t row, std::size_t col, Bubble::Color color)
{
    m_board[index(row, col)] = color;
}

std::vector<std::pair<int, int>> Board::hexNeighbors(int row, int col) const
{
    // Define the relative offsets for neighbors in even and odd rows.
    // Hexagonal grids have different neighbor layouts depending on row parity.
    static constexpr std::pair<int, int> kEvenRowOffsets[] = {
        {-1, -1}, {-1, 0}, {0, -1}, {0, 1}, {1, -1}, {1, 0}};

    static constexpr std::pair<int, int> kOddRowOffsets[] = {
        {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, 0}, {1, 1}};

    // Select the appropriate offset pattern based on whether the row is even or odd.
    const auto &offsets = ((row % 2) == 0) ? kEvenRowOffsets : kOddRowOffsets;

    std::vector<std::pair<int, int>> result;
    result.reserve(6); // Hexagonal grids always have up to 6 neighbors.

    // Compute the absolute coordinates of each neighbor and check if it's in bounds.
    for (const auto &[dRow, dCol] : offsets)
    {
        const int nextRow = row + dRow;
        const int nextCol = col + dCol;

        if (inBounds(nextRow, nextCol))
        {
            result.emplace_back(nextRow, nextCol);
        }
    }

    return result;
}

int Board::clearConnectedGroup(std::size_t startRow, std::size_t startCol, std::size_t minGroupSize)
{
    const Bubble::Color target = get(startRow, startCol);
    // Nothing to clear when the selected cell is already empty.
    if (target == Bubble::Color::None)
    {
        return 0;
    }

    // Flood-fill all same-colored bubbles connected to the start cell.
    const std::size_t total = m_rows * m_cols;
    std::vector<bool> visited(total, false);
    std::vector<std::size_t> group;
    std::deque<std::pair<int, int>> queue;

    queue.emplace_back(static_cast<int>(startRow), static_cast<int>(startCol));
    visited[index(startRow, startCol)] = true;

    while (!queue.empty())
    {
        const auto [row, col] = queue.front();
        queue.pop_front();

        const std::size_t currentIndex = index(static_cast<std::size_t>(row), static_cast<std::size_t>(col));
        group.push_back(currentIndex);

        for (const auto &[nRow, nCol] : hexNeighbors(row, col))
        {
            const std::size_t neighborIndex = index(static_cast<std::size_t>(nRow), static_cast<std::size_t>(nCol));
            if (visited[neighborIndex])
            {
                continue;
            }

            if (m_board[neighborIndex] != target)
            {
                continue;
            }

            visited[neighborIndex] = true;
            queue.emplace_back(nRow, nCol);
        }
    }

    // Do not pop groups smaller than the match threshold.
    if (group.size() < minGroupSize)
    {
        return 0;
    }

    // Remove the matched component, then let unsupported bubbles fall.
    for (const std::size_t cellIndex : group)
    {
        m_colorManager.removeColor(m_board[cellIndex]);
    }

    clearDetachedBubbles();
    return static_cast<int>(group.size());
}

int Board::clearDetachedBubbles()
{
    // Mark all bubbles reachable from the top row; those are structurally supported.
    const std::size_t total = m_rows * m_cols;
    std::vector<bool> connectedToTop(total, false);
    std::deque<std::pair<int, int>> queue;

    // Seed BFS with every non-empty top-row bubble (the only anchors).
    for (std::size_t col = 0; col < m_cols; ++col)
    {
        const std::size_t topIndex = index(0, col);
        if (m_board[topIndex] == Bubble::Color::None)
        {
            continue;
        }

        connectedToTop[topIndex] = true;
        queue.emplace_back(0, static_cast<int>(col));
    }

    // Traverse through non-empty hex neighbors to mark all anchored bubbles.
    while (!queue.empty())
    {
        const auto [row, col] = queue.front();
        queue.pop_front();

        for (const auto &[nRow, nCol] : hexNeighbors(row, col))
        {
            const std::size_t neighborIndex = index(static_cast<std::size_t>(nRow), static_cast<std::size_t>(nCol));
            if (connectedToTop[neighborIndex] || m_board[neighborIndex] == Bubble::Color::None)
            {
                continue;
            }

            connectedToTop[neighborIndex] = true;
            queue.emplace_back(nRow, nCol);
        }
    }

    // Any remaining non-empty bubble is detached and should be cleared.
    int removed = 0;
    for (std::size_t row = 0; row < m_rows; ++row)
    {
        for (std::size_t col = 0; col < m_cols; ++col)
        {
            const std::size_t cellIndex = index(row, col);
            if (m_board[cellIndex] != Bubble::Color::None && !connectedToTop[cellIndex])
            {
                m_colorManager.removeColor(m_board[cellIndex]);
                ++removed;
            }
        }
    }

    return removed;
}

std::size_t Board::index(std::size_t row, std::size_t col) const
{
    if (row >= m_rows || col >= m_cols)
    {
        throw std::out_of_range("Board index out of range");
    }

    return row * m_cols + col;
}

char Board::colorToChar(Bubble::Color color)
{
    switch (color)
    {
    case Bubble::Color::Red:
        return 'A';
    case Bubble::Color::Green:
        return 'B';
    case Bubble::Color::Blue:
        return 'C';
    case Bubble::Color::Yellow:
        return 'D';
    case Bubble::Color::Purple:
        return 'E';
    case Bubble::Color::None:
    default:
        return '.';
    }
}

// The relative offsets for neighbors are derived based on the staggered layout of hexagonal grids.
// In even rows, the top-left and bottom-left neighbors shift one column left.
// In odd rows, the top-right and bottom-right neighbors shift one column right.
// This ensures that each cell has up to 6 neighbors, and the layout alternates by row parity.
