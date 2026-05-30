
#pragma once
#include <cstddef>
#include <iosfwd>
#include <utility>
#include <vector>
#include <deque>

#include "bubble/include/bubble_color.hpp"

/**
 * @brief Manages the game board state as a hexagonal grid of bubbles.
 *
 * The Board class represents the play area as a 2D grid where each cell can contain
 * a bubble of a specific color or be empty. Key features:
 * - Hexagonal grid with proper neighbor relationships accounting for row offsets
 * - Connected group detection and clearing (match-three and larger groups)
 * - Gravity system that removes bubbles not connected to the top row
 * - Dynamic row addition at the top with downward shifting
 * - Parity offset tracking for hexagonal alignment
 *
 * The grid uses a standard even-odd row offset hexagonal layout where:
 * - Even rows (0, 2, 4...) have standard columnar alignment
 * - Odd rows (1, 3, 5...) are offset by 0.5 units horizontally
 */
class Board
{
public:
    /**
     * @brief Constructs a new Board with specified dimensions.
     *
     * Initializes an empty hexagonal grid with the given number of rows and columns.
     * The board maintains a reference to a ColorManager for updating color inventory.
     *
     * @param rows Number of rows in the hexagonal grid.
     * @param cols Number of columns in the hexagonal grid.
     * @param colorManager Reference to the color manager for tracking color usage.
     */
    Board(std::size_t rows, std::size_t cols, Bubble::ColorManager &colorManager);

    /**
     * @brief Outputs a text representation of the board to a stream.
     *
     * Useful Gets the number of rows in the board.
     * @return Row count for iteration bounds validation.
     */
    std::size_t rows() const noexcept;

    /**
     * @brief Gets the number of columns in the board.
     * @return Column count for iteration bounds validation.
     */
    std::size_t cols() const noexcept;

    /**
     * @brief Validates whether a cell position is within board bounds.
     * @param row Row index.
     * @param col Column index.
     * @return True if within valid bounds, false otherwise.
     */
    bool inBounds(int row, int col) const noexcept;

    /**
     * @brief Checks if a cell contains no bubble.
     *
     * A cell is empty if it's within bounds and contains Bubble::Color::None.
     *
     * @param row Row index.
     * @param col Column index.
     * @return True if empty, false if occupied or out of bounds
    bool inBounds(int row, int col) const noexcept;
Retrieves the color of the bubble at the specified cell.
     * @param row Row index (must be in bounds).
     * @param col Column index (must be in bounds).
     * @return The bubble color at this cell.
     */
    Bubble::Color get(std::size_t row, std::size_t col) const;

    /**
     * @brief Places a bubble at the specified cell.
     *
     * Sets the cell to contain a bubble of the given color.
     *
     * @param row Row index (must be in bounds).
     * @param col Column index (must be in bounds).
     * @param color The bubble color to place.
     */
    void set(std::size_t row, std::size_t col, Bubble::Color color);

    /**
     * @brief Computes the hexagonal neighbor cells of a given cell.
     *
     * In a hexagonal grid with row-offset layout, each cell has up to 6 neighbors.
     * This method accounts for the alternating row offset pattern.
     *
     * @param row Row index of the center cell.
     * @param col Column index of the center cell.
     * @return Vector of (row, col) coordinate pairs for hexagonal neighbors.
     */
    std::vector<std::pair<int, int>> hexNeighbors(int row, int col) const;

    /**
     * @brief Removes a connected group of same-colored bubbles if it meets size threshold.
     *
     * Implements match-three mechanics:
     * - Performs flood fill from the starting cell
     * - If group size >= minGroupSize, removes all bubbles
     * - Automatically triggers gravity to drop unsupported bubbles
     *
     * @param startRow Row of a bubble in the group to clear.
     * @param startCol Column of a bubble in the group to clear.
     * @param minGroupSize Minimum group size required for clearing.
     * @return Total number of bubbles cleared (including gravity-dropped).
     */
    int clearConnectedGroup(std::size_t startRow, std::size_t startCol, std::size_t minGroupSize);

    /**
     * @brief Adds a new row of random-colored bubbles at the top and shifts rows down.
     *
     * Used for game progression difficulty increase.
     *
     * @param colorManager Reference to ColorManager for generating random colors.
     */Removes bubbles that are no longer connected to the top row.
     *
     * Implements gravity: bubbles must remain connected to row 0 to stay on board.
     * Automatically called after clearConnectedGroup() or addNewRow().
     *
     * @return Number of bubbles removed by gravity.
     */
    int clearDetachedBubbles();

    /**
     * @brief Converts a bubble color to a character for text representation.
     * @param color The bubble color to convert.
     * @return Single character representing the color.
     */
    static char colorToChar(Bubble::Color color);

    std::size_t m_rows;  ///< Number of rows in the hexagonal grid.
    std::size_t m_cols;  ///< Number of columns in the hexagonal grid.
    /// 2D grid of bubble colors stored as a deque of vectors for efficient row operations.
    std::deque<std::vector<Bubble::Color>> m_board;
    /// Reference to the color manager for tracking inventory of bubbles placed.
    Bubble::ColorManager &m_colorManager;
    /// Parity flag for hexagonal grid offset tracking during row manipulations.tartRow, std::size_t startCol, std::size_t minGroupSize);

    /**
     * @brief Adds a new row of bubbles at the top and shifts existing rows down.
     * @param colorManager Reference to ColorManager for generating random colors.
     */
    void addNewRow(Bubble::ColorManager &colorManager);

    bool getParityOffset() const;

private:
    /**
     * @brief Clears bubbles not connected to the top row.
     * @return Number of detached bubbles cleared.
     */
    int clearDetachedBubbles();
    static char colorToChar(Bubble::Color color);

    std::size_t m_rows;
    std::size_t m_cols;
    std::deque<std::vector<Bubble::Color>> m_board;
    Bubble::ColorManager &m_colorManager;
    bool m_parity_offset{false};
};