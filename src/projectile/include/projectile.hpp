#pragma once

#include "board.hpp"
#include "bubble/include/bubble_color.hpp"

#include <cstddef>
#include <optional>
#include <utility>

class Projectile
{
public:
    /**
     * @brief Shoots a projectile at the given angle and color, attempting to attach it to the board.
     * @param board The game board.
     * @param angleDegrees The launch angle in degrees.
     * @param color The color of the projectile.
     * @return Optional pair of (row, col) where the projectile was attached, or std::nullopt if not attached.
     */
    static std::optional<std::pair<std::size_t, std::size_t>> shoot(
        Board &board,
        double angleDegrees,
        Bubble::Color color);
};
