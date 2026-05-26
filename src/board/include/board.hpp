
#pragma once
#include <cstddef>
#include <iosfwd>
#include <utility>
#include <vector>

#include "bubble/include/bubble_color.hpp"

class Board
{
public:
    /**
     * @brief Constructs a new Board with given dimensions and color count.
     * @param rows Number of rows.
     * @param cols Number of columns.
     * @param colorManager Reference to a ColorManager for tracking color counts.
     */
    Board(std::size_t rows, std::size_t cols, Bubble::ColorManager &colorManager);

    /**
     * @brief Prints the board to the given output stream.
     * @param out Output stream.
     */
    void print(std::ostream &out) const;

    /**
     * @brief Returns the number of rows.
     * @return Number of rows.
     */
    std::size_t rows() const noexcept;

    /**
     * @brief Returns the number of columns.
     * @return Number of columns.
     */
    std::size_t cols() const noexcept;

    /**
     * @brief Checks if the given cell is within bounds.
     * @param row Row index.
     * @param col Column index.
     * @return True if in bounds, false otherwise.
     */
    bool inBounds(int row, int col) const noexcept;

    /**
     * @brief Checks if the given cell is empty.
     * @param row Row index.
     * @param col Column index.
     * @return True if empty, false otherwise.
     */
    bool isEmpty(int row, int col) const noexcept;

    /**
     * @brief Gets the color of the bubble at the given cell.
     * @param row Row index.
     * @param col Column index.
     * @return Bubble color.
     */
    Bubble::Color get(std::size_t row, std::size_t col) const;

    /**
     * @brief Sets the color of the bubble at the given cell.
     * @param row Row index.
     * @param col Column index.
     * @param color Bubble color to set.
     */
    void set(std::size_t row, std::size_t col, Bubble::Color color);
    /**
     * @brief Returns the hexagonal neighbors of a cell.
     * @param row Row index.
     * @param col Column index.
     * @return Vector of neighbor coordinates.
     */
    std::vector<std::pair<int, int>> hexNeighbors(int row, int col) const;

    /**
     * @brief Clears a connected group of bubbles starting from a cell.
     * @param startRow Starting row.
     * @param startCol Starting column.
     * @param minGroupSize Minimum group size to clear.
     * @return Number of bubbles cleared.
     */
    int clearConnectedGroup(std::size_t startRow, std::size_t startCol, std::size_t minGroupSize);

private:
    /**
     * @brief Clears bubbles not connected to the top row.
     * @return Number of detached bubbles cleared.
     */
    int clearDetachedBubbles();
    std::size_t index(std::size_t row, std::size_t col) const;
    static char colorToChar(Bubble::Color color);

    std::size_t m_rows;
    std::size_t m_cols;
    std::vector<Bubble::Color> m_board;
    Bubble::ColorManager &m_colorManager;
};