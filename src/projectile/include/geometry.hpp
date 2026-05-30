/**
 * @file geometry.hpp
 * @brief Geometric utilities for hexagonal grid positioning and collision calculations.
 *
 * Provides coordinate transformations and mathematical constants for converting between
 * hexagonal grid cell indices and cell center positions, as well as collision detection radius.
 * Accounts for hexagonal grid offset pattern where odd-numbered rows are shifted horizontally.
 */
#pragma once

#include <cstddef>

/**
 * @namespace ProjectileGeometry
 * @brief Geometric calculations specific to projectile physics and hexagonal grid layout.
 *
 * Inline utility functions for:
 * - Converting grid cell indices (row, col) to continuous center coordinates
 * - Angle unit conversions
 * - Collision radius calculations
 *
 * All coordinate calculations account for the hexagonal grid's row offset pattern:
 * even rows have standard columnar alignment, while odd rows are shifted right by 0.5 units.
 */
namespace ProjectileGeometry
{    
    /// Collision detection radius in game coordinate units.
    /// Projectiles collide with bubbles when distance to cell center <= kCollisionRadius.
    inline constexpr double kCollisionRadius = 0.8;

    /**
     * @brief Calculates the x-coordinate of a hexagonal grid cell's center.
     *
     * In a hexagonal grid, alternating rows are offset horizontally to create
     * the characteristic honeycomb pattern. Even rows have columns at integer x,
     * while odd rows are offset by 0.5 units to the right.
     *
     * This offset is critical for proper collision detection and neighbor calculations.
     *
     * @param row The row index (0-based). Even vs. odd determines offset.
     * @param col The column index (0-based).
     * @return The x-coordinate of the cell's center in game units.
     */
    inline double cellCenterX(std::size_t row, std::size_t col)
    {
        // Odd rows (1, 3, 5, ...) are offset by 0.5; even rows (0, 2, 4, ...) are not.
        return static_cast<double>(col) + ((row % 2U) != 0U ? 0.5 : 0.0);
    }

    /**
     * @brief Calculates the y-coordinate of a hexagonal grid cell's center.
     *
     * In the game coordinate system, rows are evenly spaced vertically.
     * Each row's y-position simply corresponds to its row index.
     *
     * @param row The row index (0-based).
     * @return The y-coordinate of the cell's center in game units.
     */
    inline double cellCenterY(std::size_t row)
    {
        return static_cast<double>(row);
    }

    /**
     * @brief Converts an angle from degrees to radians.
     *
     * Standard degree-to-radian conversion used for trigonometric calculations
     * in physics simulation (e.g., computing velocity components from launch angle).
     *
     * @param angleDegrees Angle in degrees
     * @return Equivalent angle in radians.
     */
    inline double toRadians(double angleDegrees)
    {
        return angleDegrees * (M_PI / 180.0);
    }
} // namespace ProjectileGeometry
