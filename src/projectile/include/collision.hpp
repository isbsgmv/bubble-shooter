#pragma once

#include "board.hpp"
#include "bubble/include/bubble_color.hpp"

#include <cstddef>
#include <optional>
#include <utility>

class ProjectileCollision
{
public:
    /**
     * @brief Attaches a bubble at the ceiling at the given x position.
     * @param board The game board.
     * @param x The x position.
     * @param color The color of the bubble.
     * @return Optional pair of (row, col) where the bubble was attached, or std::nullopt if not attached.
     */
    static std::optional<std::pair<std::size_t, std::size_t>> attachAtCeiling(
        Board &board,
        double x,
        Bubble::Color color);

    /**
     * @brief Finds the bubble hit by a projectile at (x, y).
     * @param board The game board.
     * @param x The x position.
     * @param y The y position.
     * @return Optional pair of (row, col) of the hit bubble, or std::nullopt if none.
     */
    static std::optional<std::pair<int, int>> findHitBubble(const Board &board, double x, double y);

    /**
     * @brief Attaches a bubble near the hit bubble.
     * @param board The game board.
     * @param hitRow Row of the hit bubble.
     * @param hitCol Column of the hit bubble.
     * @param x The x position.
     * @param y The y position.
     * @param color The color of the bubble.
     * @return Optional pair of (row, col) where the bubble was attached, or std::nullopt if not attached.
     */
    static std::optional<std::pair<std::size_t, std::size_t>> attachNearHit(
        Board &board,
        int hitRow,
        int hitCol,
        double x,
        double y,
        Bubble::Color color);
};
