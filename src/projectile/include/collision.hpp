/**
 * @file collision.hpp
 * @brief Collision detection and bubble attachment mechanics for projectiles.
 *
 * Handles detection of projectile-bubble collisions and determines where
 * a newly launched bubble should attach to the array based on impact location
 * and hexagonal neighborhood relationships.
 */
#pragma once

#include "board.hpp"
#include "bubble/include/bubble_color.hpp"

#include <cstddef>
#include <optional>
#include <utility>

/**
 * @brief Detects projectile-bubble collisions and manages bubble attachment.
 *
 * Provides collision detection for determining which bubble (if any) a projectile
 * strikes, and methods for placing the projectile's bubble at appropriate locations
 * either at the ceiling (if no collision) or near the hit bubble using hexagonal neighbors.
 */
class ProjectileCollision
{
public:
    /**
     * @brief Attaches a bubble to the ceiling at the nearest empty cell.
     *
     * Used when a projectile reaches the top of the board without hitting any bubbles.
     * Finds the nearest empty column at row 0 to the given x position.
     *
     * @param board The game board to modify (the bubble is placed here).
     * @param x The x position where attachment is desired (in game coordinates).
     * @param color The color of the bubble to place.
     * @return Optional pair (row, col) of the attachment position, or std::nullopt if no empty cells at ceiling.
     */
    static std::optional<std::pair<std::size_t, std::size_t>> attachAtCeiling(
        Board &board,
        double x,
        Bubble::Color color);

    /**
     * @brief Detects if a projectile at the given position collides with any bubble.
     *
     * Scans all board cells for bubbles within the collision radius of the projectile.
     * Uses distance from projectile center to each bubble's cell center. If multiple
     * bubbles are in range, returns the closest one.
     *
     * Note: This method respects hexagonal grid geometry through the ProjectileGeometry
     * helper functions that account for row offsets in cell center calculations.
     *
     * @param board The game board to check collisions against.
     * @param x The x coordinate of the projectile (in game coordinates).
     * @param y The y coordinate of the projectile (in game coordinates).
     * @return Optional pair (row, col) of the hit bubble, or std::nullopt if no collision detected.
     */
    static std::optional<std::pair<int, int>> findHitBubble(const Board &board, double x, double y);

    /**
     * @brief Attaches a bubble adjacent to a hit bubble.
     *
     * Places a new bubble in one of the hexagonal neighbors of the hit bubble.
     * Selects the empty neighbor cell closest to the projectile's impact position.
     *
     * @param board The game board to modify.
     * @param hitRow Row index of the bubble that was hit.
     * @param hitCol Column index of the bubble that was hit.
     * @param x The x position of the projectile impact (used to choose nearest neighbor).
     * @param y The y position of the projectile impact.
     * @param color The color of the bubble to place.
     * @return Optional pair (row, col) of attachment position, or std::nullopt if all neighbors are occupied.
     */
    static std::optional<std::pair<std::size_t, std::size_t>> attachNearHit(
        Board &board,
        int hitRow,
        int hitCol,
        double x,
        double y,
        Bubble::Color color);
};
